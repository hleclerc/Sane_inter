#pragma once

#include "Value.h"
class Variable;
class RcString;

/**
*/
class Ref : public RcObj {
public:
    virtual ~Ref();

    virtual Variable intercept_find_attribute( const RcString &name, Type *type, bool is_const, SI32 offset ) const;
    virtual void     write_to_stream         ( std::ostream &os ) const = 0;
    virtual bool     is_shared               () const;
    virtual void    *rcast                   ();
    virtual Value    get                     () const = 0;
    virtual void     set                     ( const Value &val, SI32 offset ) = 0;
};

