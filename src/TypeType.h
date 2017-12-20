#pragma once

#include "Type.h"

/**
*/
class TypeType : public Type {
public:
    TypeType();

    virtual RcString checks_type_constraint( const Variable &self, const Variable &tested_var, TCI &tci ) const override;
    virtual bool      destroy_attrs         () const;
    virtual Variable  apply                 ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE ) override;
};

