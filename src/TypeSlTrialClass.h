#pragma once

#include "Type.h"

/**
*/
class TypeSlTrialClass : public Type {
public:
    TypeSlTrialClass();

    virtual bool     get_condition     ( const Variable &self ) const override;
    virtual void     get_fail_info     ( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const override;
    virtual unsigned get_nb_conversions( const Variable &self ) const override;
};

