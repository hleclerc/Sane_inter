#include "CallableWithSelf.h"
#include "TypeSurdefList.h"
#include "SurdefList.h"
#include "Varargs.h"
#include "gvm.h"
#include <algorithm>

TypeSurdefList::TypeSurdefList() : Type( "SurdefList" ){
}

Variable TypeSurdefList::with_self( Variable &orig, const Variable &new_self ) const {
    Variable res( new KnownRef<CallableWithSelf>, gvm->type_CallableWithSelf );
    CallableWithSelf *cs = res.rcast<CallableWithSelf>();
    cs->callable = orig;
    cs->self = new_self;

    return res;
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

    // get valid surdefinitions
    constexpr double bwp = - std::numeric_limits<double>::max();
    double working_pertinence = bwp;
    Vec<Variable> sl_trials( Size(), possibilities.size() );
    Vec<size_t> valid;
    for( size_t tr : possibilities ) {
        if ( pertinence[ tr ] < working_pertinence )
            break;
        Variable vr = sl->lst[ tr ].type->make_sl_trial( want_ret, sl->lst[ tr ], with_self, sl->args, sl->names, args, names, with_self, apply_flags );
        sl_trials[ tr ] = vr;

        if ( vr.type->get_condition( vr ) ) {
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

        if ( with_self.type )
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
