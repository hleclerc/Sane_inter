#include <boost/filesystem.hpp>
#include "System/FileReader.h"
#include "System/Stream.h"

#include "Codegen/Codegen.h"
#include "Ast/AstMaker.h"

#include "AstVisitorVm.h"
#include "Interceptor.h"
#include "Primitives.h"
#include "RefLeaf.h"
#include "Import.h"
#include "Vm.h"

#include "Inst/KnownVal.h"
#include "Inst/Void.h"
#include "Inst/Cst.h"
#include "Inst/If.h"


#include "TypeCallableWithSelf.h"
#include "TypeSlTrialClass.h"
#include "TypeSlTrialDef.h"
#include "TypeSurdefList.h"
#include "TypeVarargs.h"
#include "TypeLambda.h"
#include "TypeError.h"
#include "TypeClass.h"
#include "TypeType.h"
#include "TypeDef.h"
#include "TypeBT.h"

//// nsmake lib_name boost_filesystem

Vm::Vm( SI32 sizeof_ptr, bool reverse_endianness ) : main_scope( Scope::ScopeType::ROOT ), sizeof_ptr( sizeof_ptr ), aligof_ptr( sizeof_ptr ), reverse_endianness( reverse_endianness ) {
    error_stream = &std::cerr;
    init_mode    = true;
    nb_breaks    = 0;
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
    type_Type             = new TypeType;
    type_Def              = new TypeDef;

    // feed not initialized type_...
    #define BT( T ) if ( ! type_##T ) type_##T = new Type( #T );
    #include "BaseTypes.h"
    #undef BT

    // base_types correspondance
    #define BT( T ) base_types[ #T ] = type_##T;
    #include "BaseTypes.h"
    #undef BT

    // type of type_...->content
    #define BT( T ) type_##T->content.type = type_Type;
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
    Variable res = make_KnownVal<Import>( type_Import );
    Import *import = res.rcast<Import>();

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
            // disp = false;
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

bool Vm::little_endian() const {
    return ( __BYTE_ORDER ==__LITTLE_ENDIAN ) ^ reverse_endianness;
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

void Vm::mod_fd( RcPtr<Inst> mod_inst, const Value &fd, bool mod_cursor, bool mod_content ) {
    if ( mod_content ) {
        // even if we have != fd, it may point to the same file
        // ->
        auto iter = mod_fds.find( fd );
        if ( iter == mod_fds.end() )
            iter = mod_fds.insert( iter, std::make_pair( fd, ModFd{ mod_inst, mod_cursor, mod_content } ) );
        if ( iter->second.mod_inst != mod_inst ) {
            mod_inst->add_dep( iter->second.mod_inst );
            iter->second.mod_inst = mod_inst;
        }
    } else if ( mod_cursor ) {
        auto iter = mod_fds.find( fd );
        if ( iter == mod_fds.end() )
            iter = mod_fds.insert( iter, std::make_pair( fd, ModFd{ mod_inst, mod_cursor, mod_content } ) );

        Variable fdv( fd, Variable::Flags::CONST );
        for( auto iter : mod_fds ) {
            if ( Variable( iter.first, Variable::Flags::CONST ).equal( fdv ).is_false() == false && iter.second.mod_inst != mod_inst ) {
                mod_inst->add_dep( iter.second.mod_inst );
                iter.second.mod_inst = mod_inst;
            }
        }
    }
}

void Vm::display_graph() {
    Vec<Inst *> to_disp;
    for( std::pair<const Value&, const ModFd &> mfd : mod_fds )
        to_disp << mfd.second.mod_inst.ptr();

    Inst::display_graphviz( to_disp );
}

void Vm::codegen( Codegen &cg ) {
    Vec<Inst *> targets;
    for( std::pair<const Value&, const ModFd &> mfd : mod_fds )
        targets << mfd.second.mod_inst.ptr();

    cg.gen_code_for( targets );
}

void Vm::if_else( const Variable &cond_var, const std::function<void ()> &ok, const std::function<void ()> &ko ) {
    // conversion to bool
    if ( cond_var.type != type_Bool ) {
        Variable n_cond_var = scope->find_variable( "Bool" ).apply( true, cond_var );
        if ( n_cond_var.type != type_Bool ) {
            if ( ! n_cond_var.error() )
                add_error( "conv to Bool should give a Bool" );
            return;
        }
        return if_else( n_cond_var, ok, ko );
    }

    // is value of cond is known
    if ( cond_var.is_true() )
        return ok();
    if ( cond_var.is_false() )
        return ko();

    // else, execute ok and ko codes in sandboxes
    Interceptor inter_ok;
    inter_ok.run( ok );

    Interceptor inter_ko;
    inter_ko.run( ko );

    //
    RcPtr<IfInp> inp_ok = new IfInp;
    RcPtr<IfInp> inp_ko = new IfInp;

    Vec<Value> out_ok, out_ko;
    for( auto &p : inter_ok.mod_refs ) {
        out_ok << p.second.n;
        auto iter_out_ko = inter_ko.mod_refs.find( p.first );
        if ( iter_out_ko == inter_ko.mod_refs.end() )
            out_ko.push_back( p.second.o );
        else
            out_ko.push_back( iter_out_ko->second.n );
    }

    for( auto &p : inter_ko.mod_refs ) {
        if ( inter_ok.mod_refs.count( p.first ) )
            continue;
        int num = out_ok.size();
        out_ok.push_back( Value( inp_ok, num, p.second.o.type ) );
        out_ko.push_back( p.second.n );
    }

    // modify variables to take if outputs (we take inp_iv because int_call_s may have modified the variables)
    Vec<Value> inp_if_inst;
    inp_if_inst << cond_var.get();
    RcPtr<If> inst_if = new If( inp_if_inst, inp_ok, new IfOut( out_ok ), inp_ko, new IfOut( out_ko ) );

    int num = 0;
    for( auto &p : inter_ok.mod_refs )
        p.first->set( Value{ inst_if, num++, p.second.o.type }, -1 );
    for( auto &p : inter_ko.mod_refs )
        if ( inter_ok.mod_refs.count( p.first ) == 0 )
            p.first->set( Value{ inst_if, num++, p.second.o.type }, -1 );

    // update RefLeaf::breaks
    //    if ( breaks_ok.size() || breaks_ko.size() )
    //        ++Ref::inter_date;
    //    for( const Ref::Break &br : breaks_ok )
    //        Ref::breaks << Ref::Break{ br.nb_l, Ref::inter_date, make_And_log( br.cond, cond_val ) };
    //    for( const Ref::Break &br : breaks_ko )
    //        Ref::breaks << Ref::Break{ br.nb_l, Ref::inter_date, make_And_log( br.cond, make_Not_log( cond_val ) ) };

    // externalize variables in common
    externalize_common_insts( inst_if.ptr(), { (Inst *)inst_if->out_ok.ptr(), (Inst *)inst_if->out_ko.ptr() }, { (Inst *)inst_if->inp_ok.ptr(), (Inst *)inst_if->inp_ko.ptr() } );
}

void Vm::externalize_common_insts( Inst *main_inst, const Vec<Inst *> &inst_out, const Vec<Inst *> &inst_inp ) {
    // first traversal: mark op_id
    Inst::dfs( inst_out[ 0 ], []( Inst * ) {} );

    // seconde ones: get instructions in common
    Vec<Inst *> res;
    size_t init_op_id = Inst::cur_op_id;
    for( size_t i = 1; i < inst_out.size(); ++i ) {
        ++Inst::cur_op_id;
        insts_to_externalize_rec( res, inst_out[ i ], init_op_id );
    }

    // common inst cannot depend on an inst_inp
    for( Inst *inst : res )
        inst->op_id = init_op_id - 1;

    // replace parents from inst_a or inst_b by if_inp and if inp
    for( Inst *inst : res ) {
        for( const Inst::Parent p : inst->parents ) {
            if ( p.inst->op_id < init_op_id )
                continue;
            if ( p.ninp < 0 )
                TODO;

            // num input of If
            int ind, nout = p.inst->children[ p.ninp ].nout;
            for( size_t i = 0; ; ++i ) {
                if ( i == main_inst->children.size() ) {
                    ind = main_inst->children.size();
                    main_inst->add_child( Value( inst, nout, inst->out_type( nout ) ) );
                    break;
                }
                if ( main_inst->children[ i ].inst == inst && main_inst->children[ i ].nout == nout ) {
                    ind = i;
                    break;
                }
            }

            // replace (once)
            p.inst->mod_child( p.ninp, Value( inst_inp[ p.inst->op_id - init_op_id ], ind, p.inst->children[ p.ninp ].type, p.inst->children[ p.ninp ].offset ) );
            p.inst->op_id = init_op_id - 1;
        }
    }
    //    // replace

    //    // inst->mod_child( ninp, Value( if_inp, ind ) );

    //    //        for( size_t ninp = 0; ninp < inst->children.size(); ++ninp ) {
    //    //            Value &ch = inst->children[ ninp ];
    //    //            if ( ch.inst->op_id == init_op_id ) {
    //    //                int ind = inst_if->children.index_first( ch );
    //    //                if ( ind >= 0 ) {
    //    //                } else {
    //    //                    if_inst->add_child( ch );
    //    //                    inst->mod_child( ninp, Value( if_inp, if_inst->children.size() - 1 ) );
    //    //                }
    //    //            }
    //    //        }
}

void Vm::insts_to_externalize_rec( Vec<Inst *> &res, Inst *inst, size_t init_op_id ) {
    if ( inst->op_id == Inst::cur_op_id )
        return;
    if ( inst->op_id >= init_op_id ) {
        inst->op_id = Inst::cur_op_id;
        res << inst;
        return;
    }
    inst->op_id = Inst::cur_op_id;

    for( const Value &val : inst->children )
        insts_to_externalize_rec( res, val.inst.ptr(), init_op_id );
    for( const RcPtr<Inst> &inst : inst->deps )
        insts_to_externalize_rec( res, inst.ptr(), init_op_id );
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
