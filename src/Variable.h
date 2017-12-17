#pragma once

#include "System/RcString.h"
#include "ApplyFlags.h"
#include "Ref.h"

/**
*/
class Variable {
public:
    Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset = 0 );
    Variable( const Value &value ); // make a RefLeaf from a value
    Variable() {} // void Variable

    Variable   &operator=      ( const Variable &value );

    operator    bool           () const { return ref; }
    bool        error          () const;

    void        write_to_stream( std::ostream &os ) const;

    Variable    find_attribute ( const RcString &name, bool ret_err = true, bool msg_if_err = false ) const;
    Variable    chbeba         ( bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {} );
    Variable    select         ( bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {} );
    Variable    apply          ( bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {}, ApplyFlags apply_flags = ApplyFlags::NONE, const Vec<size_t> &spreads = {} );
    Variable    ugs            () const; ///< un_getset

    RcPtr<Ref>  ref;
    Type       *type;
    SI32        offset;
};

