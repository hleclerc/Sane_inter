#pragma once

#include "TypeContent.h"
#include "ApplyFlags.h"
#include "KnownRef.h"
class Variable;

/**
*/
class Type {
public:
    Type( const LString &name );

    virtual void        write_to_stream            ( std::ostream &os ) const;
    virtual void        write_cst                  ( std::ostream &os, const PI8 *data, int offset_mod_8 = 0 ) const;
    virtual void        destroy                    ( const Variable &self, bool use_virtual );
    virtual void        spread_in                  ( const Variable &self, Vec<Variable> &res, Vec<RcString> &names );
    virtual Variable    apply                      ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, ApplyFlags apply_flags = ApplyFlags::NONE );

    virtual bool        error                      () const;
    virtual bool        getsetter                  () const;

    KnownRef<TypeContent> content;
};

