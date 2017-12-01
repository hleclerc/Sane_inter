#include "Type_SurdefList.h"
#include "System/rcast.h"
#include "Wildcard.h"
#include "Varargs.h"
#include "equal.h"
#include "Class.h"
#include "Def.h"
#include "TCI.h"
#include "Vm.h"
#include <algorithm>

Type_SurdefList::Type_SurdefList() : Type_BaseBin( "SurdefList" ) {
}

Rc_string Type_SurdefList::checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const {
    SurdefList *se = rcast( self.ptr() );
    auto stst = [&]() -> Rc_string {
        if ( ! tested_var.type->orig_class() ) {
            if ( ! tested_var.error() )
                scope->add_error( "no orig_class for type {}", tested_var.type->name );
            return false;
        }

        // look in surdefs for a class == type->orig_class
        for( const Variable &vc : se->lst ) {
            if ( vc.type != scope->vm->type_Class )
                return scope->add_error( "Surdef contains item(s) that are not Class(es)" ), "error";
            Class *cl = rcast( vc.ptr() );
            if ( tested_var.type->orig_class() == cl ) {
                // if no argument, test only the class
                if( se->args.empty() )
                    return {};

                // get a linear list for the arguments
                if ( se->args.size() != tested_var.type->parameters.size() )
                    return scope->add_error( "not the same number of parameters" ), "error";
                if ( se->names.size() )
                    TODO;
                Vec<Variable> se_args = se->args; // TODO: use named arguments and default values

                // check that args are the same
                for( size_t i = 0; i < se_args.size(); ++i ) {
                    if ( se_args[ i ].type == scope->vm->type_Wildcard ) {
                        tci.proposals[ reinterpret_cast<Wildcard *>( se_args[ i ].ptr() )->name ] = *tested_var.type->parameters[ i ];
                        continue;
                    }
                    bool equ = equal( scope, *tested_var.type->parameters[ i ], se_args[ i ] );
                    if ( ! equ )
                        return "has not equal template parameter";
                }

                // seems to be fine :)
                return {};
            }
        }

        return "is not of the same type";
    };

    Rc_string res = stst();
    if ( res ) {
        // try recursively inheritance of tested_var
        if ( Class *cl = tested_var.type->orig_class() )
            for( const Rc_string &inh_name : cl->inheritance_names )
                if ( checks_type_constraint( scope, self, tested_var.find_attribute( scope, inh_name ), tci ).empty() )
                    return ++tci.nb_conversions, Rc_string{};

        // try operator "is_also_a"
        if ( ! se->has_wildcards( scope->vm ) )
            if ( Variable op = tested_var.find_attribute( scope, "operator is_also_a", false, false ) )
                if ( double score = op.apply( scope, true, self ).as_FP64( scope ) )
                    return tci.nb_conversions += score, Rc_string{};

        // -> fail
        return "not equal nor inherited";
    }
    return res;
}

void Type_SurdefList::write_to_stream( std::ostream &os, const char *content ) const {
    const SurdefList *sl = rcast( content );

    std::set<Rc_string> names;
    for( const Variable &v : sl->lst ) {
        if ( v.type == v.content->vm->type_Def ) {
            Def *def = rcast( v.ptr() );
            names.insert( def->name );
        } else if ( v.type == v.content->vm->type_Class ) {
            Class *def = rcast( v.ptr() );
            names.insert( def->name );
        }
    }
    int cpt = 0;
    for( const Rc_string &name : names )
        os << ( cpt++ ? "|" : "" ) << name;

    if ( sl->args.size() ) {
        cpt = 0;
        os << "[";
        for( const Variable &arg : sl->args )
            os << ( cpt++ ? "," : "" ) << arg;
        os << "]";

        if ( sl->names.size() )
            TODO;
    }
    if ( sl->self )
        os << ",self=" << sl->self;
}

Variable Type_SurdefList::with_self( Scope *scope, const Variable &orig, const Variable &new_self ) {
    SurdefList *se = rcast( orig.ptr() );

    Variable sl_var( scope->vm, scope->vm->type_SurdefList );
    SurdefList *sl = rcast( sl_var.ptr() );
    sl->lst   = se->lst;
    sl->self  = new_self;
    sl->args  = se->args;
    sl->names = se->names;

    return sl_var;
}


Variable Type_SurdefList::select( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    if ( ! want_ret )
        return {};
    SurdefList *se = rcast( self.ptr() );

    Variable sl_var( scope->vm, scope->vm->type_SurdefList );
    SurdefList *sl = rcast( sl_var.ptr() );
    sl->lst  = se->lst;
    sl->self = se->self;

    for( size_t i = 0; i < se->args.size() - se->names.size(); ++i )
        sl->args << se->args[ i ];
    for( size_t i = 0; i < args.size() - names.size(); ++i )
        sl->args << args[ i ];
    for( size_t i = 0; i < se->names.size(); ++i ) {
        sl->args  << se->args [ se->args.size() - se->names.size() + i ];
        sl->names << se->names[ i ];
    }
    for( size_t i = 0; i < names.size(); ++i ) {
        sl->args  << args [ args.size() - names.size() + i ];
        sl->names << names[ i ];
    }

    return sl_var;
}

Variable Type_SurdefList::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    // get values
    SurdefList *sl = rcast( self.ptr() );

    // if error, we don't go further
    for( const Variable &arg : args )
        if ( arg.error() )
            return scope->vm->ref_error;
    if ( sl->self && sl->self.error() )
        return scope->vm->ref_error;

    // get possible surdefinitions
    Vec<FP64> pertinence;
    Vec<size_t> possibilities;
    for( const Variable &func : sl->lst ) {
        pertinence << func.type->get_pertinence( scope, func );
        possibilities << possibilities.size();
    }

    // sort by pertinence, decreasing order
    std::sort( possibilities.begin(), possibilities.end(), [&]( size_t a, size_t b ) {
        return pertinence[ a ] > pertinence[ b ];
    } );

    // get valid surdefinitions
    constexpr double bwp = - std::numeric_limits<double>::max();
    double working_pertinence = bwp;
    Vec<Variable> sl_trials( Size(), possibilities.size() );
    Vec<size_t> valid;
    for( size_t tr : possibilities ) {
        if ( pertinence[ tr ] < working_pertinence )
            break;
        Variable vr = sl->lst[ tr ].type->make_sl_trial( scope, want_ret, sl->lst[ tr ], sl->self, sl->args, sl->names, args, names, call_ctor );
        sl_trials[ tr ] = vr;

        if ( vr.type->get_condition( scope, vr ) ) {
            working_pertinence = pertinence[ tr ];
            valid << tr;
        }
    }

    // check that at least one definition is valid
    if ( valid.empty() ) {
        // if it failed because of no valid constructor
        if ( sl->self && sl->only_contains_defs_named( scope->vm, "construct" ) && args.size() == 1 ) {
            // try with a converter
            if ( Variable cnv = args[ 0 ].find_attribute( scope, "convert", false ) ) {
                // if the converter returns something, it's the argument for a constructor
                if ( Variable res = cnv.apply( scope, true, sl->self ).ugs( scope ) ) {
                    Vec<String> n_names;
                    Vec<Variable> n_args;
                    if ( res.type == scope->vm->type_Varargs ) {
                        Varargs *va = rcast( res.ptr() );
                        n_names = va->names;
                        n_args = va->values;
                    } else
                        n_args << res;
                    // call again the constructor, with args from the converted
                    apply( scope, self, want_ret, n_args, n_names, call_ctor );
                    return scope->vm->ref_void;
                }
                // else, we assume that the constructor has already been called
                return scope->vm->ref_void;
            }
        }

        std::ostringstream ss;
        ss << "no valid surdefinition for "; // arg type" << ( args.size() > 1 ? "s" : "" ) << " [";

        if ( sl->self )
            ss << *sl->self.ugs_type( scope ) << ".";
        ss << sl->possible_names( scope->vm );
        ss << "(" << ( args.size() ? " " : "" );
        for( size_t i = 0; i < args.size(); ++i )
            ss << ( i ? ", " : "" ) << *args[ i ].ugs_type( scope );
        ss << ( args.size() ? " " : "" ) << ")";

        Error_list::Error &err = scope->prep_error( ss.str() );
        for( const Variable &tr : sl_trials ) {
            size_t offset;
            Rc_string source, msg;
            tr.type->get_fail_info( scope, tr, offset, source, msg );
            if ( ! source.empty() )
                err.ap( source, offset, "(" + msg + ")" );
        }
        scope->disp_error( err );
        return scope->vm->ref_error;
    }

    // check that (potentially) valid surdefinitions are with != { pertinence, nb_conversions }
    if ( valid.size() > 1 ) {
        unsigned min_nb_conversions = std::numeric_limits<unsigned>::max();
        for( size_t i : valid ) {
            unsigned nb_conversions = sl_trials[ i ].type->get_nb_conversions( scope, sl_trials[ i ] );
            if ( min_nb_conversions > nb_conversions )
                min_nb_conversions = nb_conversions;
        }
        valid.remove_if( [&]( unsigned i ) { return sl_trials[ i ].type->get_nb_conversions( scope, sl_trials[ i ] ) > min_nb_conversions; } );
    }

    if ( valid.size() > 1 ) {
        Error_list::Error &err = scope->prep_error( "Ambiguous overload" );
        for( size_t i : valid ) {
            size_t offset;
            Rc_string source, msg;
            sl_trials[ i ].type->get_fail_info( scope, sl_trials[ i ], offset, source, msg );
            if ( ! source.empty() )
                err.ap( source, offset );
        }
        scope->disp_error( err );
        return scope->vm->ref_error;
    }

    // launch
    return sl->lst[ valid[ 0 ] ].type->use_sl_trial( scope, want_ret, sl->lst[ valid[ 0 ] ], self, sl->args, sl->names, args, names, call_ctor, sl_trials[ valid[ 0 ] ] );
}
