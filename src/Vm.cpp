#include <boost/filesystem.hpp>
#include "System/FileReader.h"
#include "System/Stream.h"
#include "Ast/AstMaker.h"
#include "AstVisitorVm.h"
#include "Inst/Cst.h"
#include "KnownRef.h"
#include "Import.h"
#include "Type.h"
#include "Vm.h"

//// nsmake lib_name boost_filesystem

Vm::Vm() : scope( 0 ), main_scope( Scope::ScopeType::ROOT ) {
    init_mode = false;
    error_stream = &std::cerr;

    #define BT( T ) type_##T = new Type;
    #include "BaseTypes.h"
    #undef BT
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
    //    bool disp = true;
    //    for( const Scope *s = this; s; s = s->parent ) {
    //        if ( s->pos.cur_names && disp ) {
    //            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
    //            // disp = false;
    //        }

    //        if ( s->type == Scope::Scope_type::CALL )
    //            disp = true;
    //    }
    return error;
}

void Vm::disp_Error( const Error &error ) const {
    *error_stream << error;
}


Variable Vm::visit( const AstCrepr &ac, bool want_ret ) {
    return visit( ac.names, ac.code, want_ret );
}

Variable Vm::visit( const RcString &names, const RcString &code, bool want_ret ) {
    auto _ = raii_save( pos );

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
