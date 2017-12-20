#include <boost/filesystem.hpp>
#include "System/FileReader.h"
#include "System/Stream.h"
#include "Ast/AstMaker.h"
#include "AstVisitorVm.h"
#include "Primitives.h"
#include "Inst/Void.h"
#include "Inst/Cst.h"
#include "KnownRef.h"
#include "RefLeaf.h"
#include "Import.h"
#include "Vm.h"

#include "TypeCallableWithSelf.h"
#include "TypeSlTrialClass.h"
#include "TypeSlTrialDef.h"
#include "TypeSurdefList.h"
#include "TypeVarargs.h"
#include "TypeLambda.h"
#include "TypeError.h"
#include "TypeClass.h"
#include "TypeDef.h"
#include "TypeBT.h"

//// nsmake lib_name boost_filesystem

Vm::Vm( SI32 sizeof_ptr, bool reverse_endianness ) : main_scope( Scope::ScopeType::ROOT ), sizeof_ptr( sizeof_ptr ), reverse_endianness( reverse_endianness ) {
    nb_breaks    = 0;
    init_mode    = true;
    error_stream = &std::cerr;
    nb_calls     = 0;

    main_scope.parent = 0;
    scope = &main_scope;

    #define BT( T ) type_##T = 0;
    #include "BaseTypes.h"
    #undef BT

    // arythmetic types
    #define BT( T ) type_##T = reverse_endianness ? (Type *)new TypeBT<T,true>( #T ) : (Type *)new TypeBT<T,false>( #T );
    #include "ArythmeticTypes.h"
    #undef BT

    type_CallableWithSelf = new TypeCallableWithSelf;
    type_SlTrialClass     = new TypeSlTrialClass;
    type_SlTrialDef       = new TypeSlTrialDef;
    type_SurdefList       = new TypeSurdefList;
    type_Varargs          = new TypeVarargs;
    type_Lambda           = new TypeLambda;
    type_Error            = new TypeError;
    type_Class            = new TypeClass;
    type_Def              = new TypeDef;

    // feed not initialized type_...
    #define BT( T ) if ( ! type_##T ) type_##T = new Type( #T );
    #include "BaseTypes.h"
    #undef BT

    // base_types correspondance
    #define BT( T ) base_types[ #T ] = type_##T;
    #include "BaseTypes.h"
    #undef BT

    for( Primitive_decl *pd = last_Primitive_decl; pd; pd = pd->prev )
        predefs[ RcString( "__primitive_" ) + pd->name ] = make_Void( types.push_back_val( pd->func() ) );

    for( Primitive_func *pd = last_Primitive_func; pd; pd = pd->prev )
        predeffs[ pd->name ] = pd->func;
}

Variable Vm::import( const String &filename, const String &import_dir, bool display_lexems ) {
    String abso = path( filename, import_dir );
    if ( abso.empty() )
        return add_error( "file '{}' not found", filename ), ref_error;

    // already imported ?
    auto iter = imported.find( abso );
    if ( iter != imported.end() )
        return iter->second;

    // prepare a result variable
    KnownRef<Import> *ref_import = new KnownRef<Import>;
    Variable res( ref_import, type_Import );
    Import *import = &ref_import->data;

    imported.insert( iter, std::make_pair( abso, res ) );

    import->finished = false;
    import->filename = abso;

    // parse
    FileReader fr( abso );
    AstMaker am( &error_list );
    size_t old_el_size = error_list.size();
    am.parse( fr.data, abso.c_str(), display_lexems );
    AstCrepr ac = am.to_AstCrepr();

    if ( display_lexems ) {
        AstVisitorDisplay ad( std::cout );
        ast_visit( ad, ac.code );
    }

    // if parse error, return an Error var
    if ( old_el_size != error_list.size() ) {
        imported[ abso ] = ref_error;
        return ref_error;
    }

    // exec
    Scope loc( Scope::ScopeType::BLOCK );
    loc.import = import;
    visit( ac, false );

    //
    //    for( Scope::NV *nv = loc.last_var; nv; nv = nv->prev )
    //        if ( nv->flags & Scope::VariableFlags::EXPORT )
    //            import->exports << Import::Export{ String( nv->name ), nv->var };
    //    std::reverse( import->exports.begin(), import->exports.end() );

    import->finished = true;
    return res;
}

String Vm::path( const String &filename, const String &import_dir ) {
    // absolute filename
    boost::filesystem::path path = filename;
    if ( import_dir.empty() || path.is_absolute() ) {
        boost::system::error_code ec;
        boost::filesystem::path cano = boost::filesystem::canonical( path, import_dir, ec );
        return ec ? String{} : cano.string();
    }

    // ./... -> relative to import_dir
    if ( import_dir.size() ) {
        auto starts_with = [&]( const char *s ) {
            return filename.size() >= strlen( s ) && strncmp( filename.data(), s, strlen( s ) ) == 0;
        };
        if ( starts_with( "./" ) || starts_with( "../" ) || starts_with( ".\\" ) || starts_with( "..\\" ) ) {
            boost::system::error_code ec;
            boost::filesystem::path cano = boost::filesystem::canonical( path, import_dir, ec );
            return ec ? String{} : cano.string();
        }
    }

    // -> include lookup
    for( const String &include : includes ) {
        boost::system::error_code ec;
        boost::filesystem::path cano = boost::filesystem::canonical( path, include, ec );
        if ( ! ec )
            return cano.string();
    }

    return {};
}

//void Vm::disp_pos( const String &msg ) const {
//    Error_list::Error error;
//    //    bool disp = true;
//    //    for( const Scope *s = this; s; s = s->parent ) {
//    //        if ( s->pos.cur_names && disp ) {
//    //            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
//    //            disp = false;
//    //        }

//    //        if ( s->type == Scope::Scope_type::CALL )
//    //            disp = true;
//    //    }
//    std::cerr << msg << ": " << error << std::endl;
//}

ErrorList::Error &Vm::prep_Error( int nb_calls_to_skip, const String &msg ) {
    ErrorList::Error &error = error_list.add( msg );
    bool disp = true;
    for( const Scope *s = scope; s; s = s->parent ) {
        if ( s->pos.cur_names && disp ) {
            if ( nb_calls_to_skip ) {
                --nb_calls_to_skip;
                continue;
            }
            error.ac( s->pos.src_name(), s->pos.cur_off );
            disp = false;
        }

        if ( s->type == Scope::ScopeType::CALL )
            disp = true;
    }
    return error;
}

void Vm::disp_Error( const Error &error ) const {
    *error_stream << error;
}


Variable Vm::visit( const AstCrepr &ac, bool want_ret ) {
    return visit( ac.names, ac.code, want_ret );
}

Variable Vm::new_Type( Type *type ) {
    TODO;
    return {};
}

Variable Vm::make_inst( Type *type, const Vec<Variable> &ctor_args, const Vec<RcString> &ctor_names, ApplyFlags apply_flags ) {
    // check that all abstract surdefs are defined
    if ( ! ( apply_flags & ApplyFlags::DONT_CALL_CTOR ) && type->content.data.abstract_methods.size() ) {
        std::string am;
        for( const FunctionSignature &fs : type->content.data.abstract_methods )
            am += std::string( am.empty() ? "" : ", " ) + fs.name;
        gvm->add_error( "{} contains abstract methods ({}) and should not be instantiated", *type, am );
    }

    // make an instance
    Variable res( new RefLeaf( make_Cst( type ), apply_flags & ApplyFlags::DONT_CALL_CTOR ? RefLeaf::Flags::NOT_CONSTRUCTED : RefLeaf::Flags::NONE ), type );

    // call constructor if necessary
          if ( ! ( apply_flags & ApplyFlags::DONT_CALL_CTOR ) )
        type->construct( res, ctor_args, ctor_names );

    return res;
}

Type *Vm::type_ptr_for( const RcString &name, const Vec<Variable> &args ) {
    if ( init_mode ) {
        auto iter = base_types.find( name );
        if ( iter != base_types.end() )
            return iter->second;
    }
    Type *res = types.push_back_val( new Type( name ) );
    for( const Variable &arg : args )
        res->content.data.parameters << main_scope.add_static_variable( init_mode ? arg : arg.find_attribute( "operator :=" ).apply( true ) ); // TODO: make a constified copy
    return res;
}

Variable Vm::visit( const RcString &names, const RcString &code, bool want_ret ) {
    auto _ = raii_save( scope->pos );

    AstVisitorVm av( names, want_ret );
    ast_visit( av, code );
    return av.ret;
}

Variable Vm::visit( const RcString &names, const Vec<RcString> &code, bool want_ret ) {
    if ( code.size() ) {
        for( size_t i = 0; i < code.size() - 1; ++i )
            visit( names, code[ i ], false );
        return visit( names, code.back(), want_ret );
    }
    ASSERT( want_ret == false, "..." );
    return {};
}

Variable Vm::visit( const Vec<AstCrepr> &creps, bool want_ret ) {
    if ( creps.size() ) {
        for( size_t i = 0; i < creps.size() - 1; ++i )
            visit( creps[ i ], false );
        return visit( creps.back(), want_ret );
    }
    ASSERT( want_ret == false, "..." );
    return {};
}
