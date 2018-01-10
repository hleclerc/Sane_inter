#pragma once

#include "System/EnumFlags.h"
#include "Variable.h"

/**
*/
class RefGetsetter : public Ref {
public:
    RefGetsetter( const Variable &g, const Variable &s, const Variable &m, const Variable &t );

    virtual Variable intercept_find_attribute( const RcString &name, Type *type, bool is_const, SI32 offset ) const override;
    virtual void     write_to_stream         ( std::ostream &os ) const override;
    virtual bool     is_shared               () const override;
    Variable         variable                () const;
    virtual Value    get                     () const override;
    virtual void     set                     ( const Value &val, int cst = 0 ) override;

    Variable         g;
    Variable         s;
    Variable         m;
    Variable         t;

    mutable Variable value;
    Type            *type;
};
