#pragma once

#include "Type_BaseBin.h"
#include "SlTrialClass.h"

/**
*/
class Type_SlTrialClass : public Type_BaseBin<SlTrialClass> {
public:
    Type_SlTrialClass();

    virtual bool     get_condition     ( Scope *scope, const Variable &self ) const override;
    virtual void     get_fail_info     ( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const override;
    virtual unsigned get_nb_conversions( Scope *scope, const Variable &self ) const override;
};

