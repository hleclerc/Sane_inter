#include "Interceptor.h"
#include "RefLeaf.h"
#include "gvm.h"

RefLeaf::RefLeaf( const Value &value, Flags flags ) : creation_inter_date( gvm ? gvm->inter_date : 0 ), value( value ), flags( flags ) {
}

void RefLeaf::write_to_stream( std::ostream &os ) const {
    os << value;
}

bool RefLeaf::is_shared() const {
    return cpt_use > 1;
}

void RefLeaf::constify() {
    flags |= Flags::CONST;
}

Value RefLeaf::get() const {
    return value;
}

void RefLeaf::set( const Value &src_val, int cst ) {
    if ( value == src_val )
        return;

    if ( flags & Flags::CONST )
        gvm->add_error( "a const reference should not be modified" );

    // interception
    if ( gvm->interceptor && gvm->inter_date > creation_inter_date ) {
        auto iter = gvm->interceptor->mod_refs.find( this );
        if ( iter == gvm->interceptor->mod_refs.end() )
            gvm->interceptor->mod_refs.emplace_hint( iter, this, Interceptor::ValueChange{ value, {} } );
    }

    // change value
    value = src_val;

    //    auto mod = [&]() {
    //        if ( creation_inter_date < inter_date && inter_map ) {
    //            //RcPtr<Ref> rp_this( inc_ref( this ) );
    //            auto iter = inter_map->find( this );
    //            if ( iter == inter_map->end() )
    //                inter_map->emplace_hint( iter, this, Interception{ val, {} } );
    //        }
    //    };

    //    //
    //    Value in_a_break;
    //    for( const Break &br : breaks )
    //        if ( creation_inter_date < br.date )
    //            in_a_break = in_a_break ? make_Or_log( in_a_break, br.cond ) : br.cond;

    //    if ( in_a_break ) {
    //        if ( ! in_a_break.is_true() ) {
    //            mod();
    //            if ( in_a_break.is_false() ) {
    //                val.inst = new_val.inst;
    //                val.nout = new_val.nout;
    //            } else {
    //                val = new_If( { in_a_break }, new If_inp, new If_out( val ), new If_inp, new If_out( Value{ new_val.inst, new_val.nout } ) );
    //            }
    //        }
    //    } else {
    //        mod();
    //        val.inst = new_val.inst;
    //        val.nout = new_val.nout;
    //    }
}
