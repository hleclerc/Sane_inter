#pragma once

#include "Type_BaseBin.h"
#include "SlTrialDef.h"

/**
*/
class Type_SlTrialDef : public Type_BaseBin<SlTrialDef> {
public:
    Type_SlTrialDef();

    virtual unsigned get_nb_conversions( Scope *scope, const Variable &self ) const override;
    virtual bool     get_condition     ( Scope *scope, const Variable &self ) const override;
    virtual void     get_fail_info     ( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const override;
};

