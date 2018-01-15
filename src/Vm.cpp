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

#include "Inst/RessourceInst.h"
#include "Inst/KnownVal.h"
#include "Inst/Gatherer.h"
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
    interceptor  = 0;
    inter_date   = 0;
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
        res->content.data.parameters << main_scope.add_static_variable( init_mode ? arg : arg.find_attribute( "operator :=" ).apply( true ).constify( true ) );
    return res;
}

void Vm::mod_fd( RcPtr<Inst> mod_inst ) {
    // get ressource descriptors directly modified by mod_inst
    std::map<Ressource *,bool> modifications;
    mod_inst->get_mod_ressources( [&]( Ressource *rs, bool write ) {
        auto insert = modifications.insert( std::make_pair( rs, 0 ) );
        insert.first->second = std::max( insert.first->second, write );
    } );

    // for each ressource descriptor directly modified by mod_inst
    for( std::pair<Ressource *,int> mod : modifications ) {
        Ressource *rs = mod.first;
        if ( ! rs->state.last_writer )
            rs->state.last_writer = make_RessourceInst( rs );

        //
        if ( interceptor ) {
            auto iter = interceptor->mod_ressources.find( rs );
            if ( iter == interceptor->mod_ressources.end() )
                iter = interceptor->mod_ressources.emplace_hint( iter, rs, Interceptor::RessourceChange{ rs->state, { rs->state.last_writer, {} } } );
        }

        // write ?
        int nout = mod_inst->nb_outputs();
        if ( mod.second ) {
            mod_inst->ressource_writers.insert( mod_inst->children.size() );

            if ( rs->state.last_readers.size() ) {
                RcPtr<Inst> ga = new Gatherer;
                for( const Value &reader : rs->state.last_readers )
                    ga->add_child( reader );
                rs->state.last_readers.clear();
                mod_inst->add_child( Value( ga, 0, gvm->type_Ressource ) );
                rs->state.last_writer = Value( mod_inst, nout, gvm->type_Ressource );
            } else {
                mod_inst->add_child( rs->state.last_writer );
                rs->state.last_writer = Value( mod_inst, nout, gvm->type_Ressource );
            }
        } else {
            mod_inst->add_child( rs->state.last_writer );
            rs->state.last_readers << Value( mod_inst, nout, gvm->type_Ressource );
        }
    }
}

void Vm::display_graph( const char *fn ) {
    Vec<Inst *> to_disp;
    ressource_map.visit( [&]( Ressource *rs ) {
        if ( rs->state.last_writer )
            to_disp << rs->state.last_writer.inst.ptr() ;
    } );

    Inst::display_graphviz( to_disp, [](std::ostream &, const Inst *) {}, fn );
}

void Vm::codegen( Codegen &cg ) {
    Vec<Inst *> targets;
    ressource_map.visit( [&]( Ressource *rs ) {
        if ( rs->state.last_writer )
            targets << rs->state.last_writer.inst.ptr() ;
    } );

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

    // save new values in out instructions
    Vec<Value> out_ok, out_ko;
    for( auto &p : inter_ok.mod_refs ) {
        out_ok << p.second.n;
        auto iter_out_ko = inter_ko.mod_refs.find( p.first );
        if ( iter_out_ko == inter_ko.mod_refs.end() )
            out_ko << p.second.o;
        else
            out_ko << iter_out_ko->second.n;
    }

    for( auto &p : inter_ko.mod_refs ) {
        if ( inter_ok.mod_refs.count( p.first ) )
            continue;
        int num = out_ok.size();
        out_ok << Value( inp_ok, num, p.second.o.type );
        out_ko << p.second.n;
    }

    for( const std::pair<Ressource *,Interceptor::RessourceChange> &p : inter_ok.mod_ressources ) {
        out_ok << p.second.n.make_single_inst();
        auto iter_out_ko = inter_ko.mod_ressources.find( p.first );
        if ( iter_out_ko == inter_ko.mod_ressources.end() )
            out_ko << p.second.o.make_single_inst();
        else
            out_ko << iter_out_ko->second.n.make_single_inst();
    }

    for( auto &p : inter_ko.mod_ressources ) {
        if ( inter_ok.mod_ressources.count( p.first ) )
            continue;
        int num = out_ok.size();
        out_ok << Value( inp_ok, num, gvm->type_Ressource );
        out_ko << p.second.n.make_single_inst();
    }

    // new If inst
    RcPtr<If> inst_if = new If( cond_var.get(), inp_ok, new IfOut( out_ok ), inp_ko, new IfOut( out_ko ) );

    // variables modified by the the if instruction
    int num = 0;
    for( auto &p : inter_ok.mod_refs )
        p.first->set( Value{ inst_if, num++, p.second.o.type }, -1 );
    for( auto &p : inter_ko.mod_refs )
        if ( inter_ok.mod_refs.count( p.first ) == 0 )
            p.first->set( Value{ inst_if, num++, p.second.o.type }, -1 );

    // ressources modified by the the if instruction
    gvm->mod_fd( inst_if );

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

    //    ++Inst::cur_op_id;
    //    insts_to_externalize_rec( res, main_inst, init_op_id );

    // common inst cannot depend on an inst_inp
    for( Inst *inst : res )
        inst->op_id = init_op_id - 1;

    // replace parents from inst_a or inst_b by if_inp and if inp
    for( Inst *inst : res ) {
        Vec<Inst::Parent> parents = inst->parents;
        for( const Inst::Parent &p : parents ) {
            if ( p.inst->op_id < init_op_id )
                continue;

            // num input of If (we want raw outputs, without offset or subtyping)
            int ind, nout = p.inst->children[ p.ninp ].nout;
            Type *base_out_type = inst->out_type( nout );
            for( size_t i = 0; ; ++i ) {
                if ( i == main_inst->children.size() ) {
                    ind = main_inst->children.size();
                    main_inst->add_child( Value( inst, nout, base_out_type ) );
                    break;
                }
                const Value &ch = main_inst->children[ i ];
                if ( ch.inst == inst && ch.nout == nout && ch.type == base_out_type && ch.offset == 0 ) {
                    ind = i;
                    break;
                }
            }

            // replace (once)
            if ( p.inst->op_id - init_op_id < inst_inp.size() )
                p.inst->mod_child( p.ninp, Value( inst_inp[ p.inst->op_id - init_op_id ], ind, p.inst->children[ p.ninp ].type, p.inst->children[ p.ninp ].offset ) );
        }
    }
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
