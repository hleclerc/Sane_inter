#pragma once

#include "System/EnumFlags.h"
#include "Variable.h"

/**
*/
class RefGetsetter : public Ref {
public:
    RefGetsetter( const Variable &g, const Variable &s, const Variable &m, const Variable &t );

    virtual void     write_to_stream( std::ostream &os ) const override;
    virtual bool     is_shared      () const override;
    virtual Value    get            () const override;

    Variable         g;
    Variable         s;
    Variable         m;
    Variable         t;

    mutable Variable value;
    Type            *type;
};
