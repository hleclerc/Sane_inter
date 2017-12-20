#pragma once

#include "Type.h"

/**
*/
class TypeSlTrialDef : public Type {
public:
    TypeSlTrialDef();

    virtual unsigned get_nb_conversions( const Variable &self ) const override;
    virtual CondVal  get_condition     ( const Variable &self ) const override;
    virtual void     get_fail_info     ( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const override;
};

