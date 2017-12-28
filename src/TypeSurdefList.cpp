#include "CallableWithSelf.h"
#include "TypeSurdefList.h"
#include "SurdefList.h"
#include "Wildcard.h"
#include "Varargs.h"
#include "Class.h"
#include "equal.h"
#include "TCI.h"
#include "gvm.h"
#include <algorithm>

TypeSurdefList::TypeSurdefList() : Type( "SurdefList" ){
}

RcString TypeSurdefList::checks_type_constraint( const Variable &self, const Variable &tested_var, TCI &tci ) const {
    SurdefList *se = self.rcast<SurdefList>();
    auto stst = [&]() -> RcString {
        if ( ! tested_var.type->orig_class() ) {
            if ( ! tested_var.error() )
                gvm->add_error( "no orig_class for type {}", tested_var.type->content.data.name );
            return false;
        }

        // look in surdefs for a class == type->orig_class
        for( const Variable &vc : se->lst ) {
            if ( vc.type != gvm->type_Class )
                return gvm->add_error( "Surdef contains item(s) that are not Class(es)" ), "error";
            Class *cl = vc.rcast<Class>();
            if ( tested_var.type->orig_class() == cl ) {
                // if no argument, test only the class
                if( se->args.empty() )
                    return {};

                // get a linear list for the arguments
                if ( se->args.size() != tested_var.type->content.data.parameters.size() )
                    return gvm->add_error( "not the same number of parameters" ), "error";
                if ( se->names.size() )
                    TODO;
                Vec<Variable> se_args = se->args; // TODO: use named arguments and default values

                // check that args are the same
                for( size_t i = 0; i < se_args.size(); ++i ) {
                    if ( se_args[ i ].type == gvm->type_Wildcard ) {
                        tci.proposals[ se_args[ i ].rcast<Wildcard>()->name ] = *tested_var.type->content.data.parameters[ i ];
                        continue;
                    }
                    bool equ = equal( *tested_var.type->content.data.parameters[ i ], se_args[ i ] );
                    if ( ! equ )
                        return "has not equal template parameter";
                }

                // seems to be fine :)
                return {};
            }
        }

        return "is not of the same type";
    };

    RcString res = stst();
    if ( res ) {
        // try recursively inheritance of tested_var
        if ( Class *cl = tested_var.type->orig_class() )
            for( const RcString &inh_name : cl->inheritance_names )
                if ( checks_type_constraint( self, tested_var.find_attribute( inh_name ), tci ).empty() )
                    return ++tci.nb_conversions, RcString{};

        // try operator "is_also_a"
        if ( ! se->has_wildcards() )
            if ( Variable op = tested_var.find_attribute( "operator is_also_a", false, false ) )
                if ( double score = op.apply( true, self ).as_FP64() )
                    return tci.nb_conversions += score, RcString{};

        // -> fail
        return "not equal nor inherited";
    }
    return res;
}

Variable TypeSurdefList::with_self( Variable &orig, const Variable &new_self ) const {
    Variable res( MAKE_KV( CallableWithSelf ) );
    CallableWithSelf *cs = res.rcast<CallableWithSelf>();
    cs->callable = orig;
    cs->self = new_self;

    return res;
}

Variable TypeSurdefList::select( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    if ( ! want_ret )
        return {};
    SurdefList *se = self.rcast<SurdefList>();

    Variable sl_var( MAKE_KV( SurdefList ) );
    SurdefList *sl = sl_var.rcast<SurdefList>();
    sl->lst  = se->lst;

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

Variable TypeSurdefList::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) {
    SurdefList *sl = self.rcast<SurdefList>();

    // if error, we don't go further
    for( const Variable &arg : args )
        if ( arg.error() )
            return gvm->ref_error;
    if ( with_self && with_self.error() )
        return gvm->ref_error;

    // get possible surdefinitions
    Vec<FP64> pertinence;
    Vec<size_t> possibilities;
    for( const Variable &func : sl->lst ) {
        pertinence << func.type->get_pertinence( func );
        possibilities << possibilities.size();
    }

    // sort by pertinence, decreasing order
    std::sort( possibilities.begin(), possibilities.end(), [&]( size_t a, size_t b ) {
        return pertinence[ a ] > pertinence[ b ];
    } );

    // get (potentially) valid surdefinitions (using what we know during compile time)
    constexpr double bwp = - std::numeric_limits<double>::max();
    double working_pertinence = bwp;
    Vec<Variable> sl_trials( Size(), possibilities.size() );
    Vec<size_t> valid;
    for( size_t tr : possibilities ) {
        if ( pertinence[ tr ] < working_pertinence )
            break;
        Variable vr = sl->lst[ tr ].type->make_sl_trial( want_ret, sl->lst[ tr ], with_self, sl->args, sl->names, args, names, with_self, apply_flags );
        sl_trials[ tr ] = vr;

        if ( vr.type->get_condition( vr ).kv >= 0 ) {
            working_pertinence = pertinence[ tr ];
            valid << tr;
        }
    }

    // check that at least one definition is valid
    if ( valid.empty() ) {
        // if it failed because of no valid constructor
        if ( with_self && sl->only_contains_defs_named( "construct" ) && args.size() == 1 ) {
            // try with a converter
            if ( Variable cnv = args[ 0 ].find_attribute( "convert", false ) ) {
                // if the converter returns something, it's the argument for a constructor
                if ( Variable res = cnv.apply( true, with_self ) ) {
                    Vec<LString> n_names;
                    Vec<Variable> n_args;
                    if ( res.type == gvm->type_Varargs ) {
                        Varargs *va = res.rcast<Varargs>();
                        n_names = va->names;
                        n_args = va->values;
                    } else
                        n_args << res;
                    // call again the constructor, with args from the converted
                    apply( self, want_ret, n_args, n_names, with_self, apply_flags );
                    return gvm->ref_void;
                }
                // else, we assume that the constructor has already been called
                return gvm->ref_void;
            }
        }

        std::ostringstream ss;
        ss << "no valid surdefinition for "; // arg type" << ( args.size() > 1 ? "s" : "" ) << " [";

        if ( with_self )
            ss << *with_self.type << ".";
        ss << sl->possible_names();
        ss << "(" << ( args.size() ? " " : "" );
        for( size_t i = 0; i < args.size(); ++i )
            ss << ( i ? ", " : "" ) << *args[ i ].type;
        ss << ( args.size() ? " " : "" ) << ")";

        ErrorList::Error &err = gvm->prep_Error( 0, ss.str() );
        for( const Variable &tr : sl_trials ) {
            size_t offset;
            RcString source, msg;
            tr.type->get_fail_info( tr, offset, source, msg );
            if ( ! source.empty() )
                err.ap( source, offset, "(" + msg + ")" );
        }
        gvm->disp_Error( err );
        return gvm->ref_error;
    }

    // check that (potentially) valid surdefinitions are with != { pertinence, nb_conversions }
    if ( valid.size() > 1 ) {
        unsigned min_nb_conversions = std::numeric_limits<unsigned>::max();
        for( size_t i : valid ) {
            unsigned nb_conversions = sl_trials[ i ].type->get_nb_conversions( sl_trials[ i ] );
            if ( min_nb_conversions > nb_conversions )
                min_nb_conversions = nb_conversions;
        }
        valid.remove_if( [&]( unsigned i ) { return sl_trials[ i ].type->get_nb_conversions( sl_trials[ i ] ) > min_nb_conversions; } );
    }

    if ( valid.size() > 1 ) {
        ErrorList::Error &err = gvm->prep_Error( 0, "Ambiguous overload" );
        for( size_t i : valid ) {
            size_t offset;
            RcString source, msg;
            sl_trials[ i ].type->get_fail_info( sl_trials[ i ], offset, source, msg );
            if ( ! source.empty() )
                err.ap( source, offset );
        }
        gvm->disp_Error( err );
        return gvm->ref_error;
    }

    // launch
    return sl->lst[ valid[ 0 ] ].type->use_sl_trial( want_ret, sl->lst[ valid[ 0 ] ], self, sl->args, sl->names, args, names, with_self, apply_flags, sl_trials[ valid[ 0 ] ] );
}
