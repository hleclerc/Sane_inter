#pragma once

#include "../Value.h"
struct AttrClone {};

/**
*/
template<class T,class IH=Inst>
class Clonable : public IH {
public:
    Clonable() {}

    virtual Inst *clone() const {
        if ( this->op_id == Inst::cur_op_id )
            return reinterpret_cast<Inst *>( this->op_mp );

        T *res = new T( AttrClone(), static_cast<const T *>( this ) );
        this->op_id = Inst::cur_op_id;
        this->op_mp = res;

        res->attr_clone( static_cast<const T *>( this ) );

        for( const Value &ch : this->children )
            res->add_child( Value( ch.inst->clone(), ch.nout, ch.type, ch.offset ) );
        for( const RcPtr<Inst> &ch : this->deps )
            res->add_dep( ch->clone() );

        return res;
    }

    void attr_clone( const T *src ) {
    }
};
