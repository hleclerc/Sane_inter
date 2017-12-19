#pragma once

#include "System/RcString.h"
#include "ApplyFlags.h"
#include "Ref.h"
class TCI;

/**
*/
class Variable {
public:
    enum class Flags: PI32 { NONE = 0, CONST = 1 };
    Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset = 0, Flags flags = Flags::NONE );
    Variable( const Value &value ); // make a RefLeaf from a value
    Variable() {} // void Variable

    Variable   &operator=           ( const Variable &value );

    operator    bool                () const { return ref; }
    bool        error               () const;
    bool        is_shared           () const { return ref->is_shared(); }

    void        write_to_stream     ( std::ostream &os ) const;

    RcString    valid_constraint_for( const Variable &tested_var, TCI &tci ) const;
    Variable    find_attribute      ( const RcString &name, bool ret_err = true, bool msg_if_err = false ) const;
    void        setup_vtables       ();
    Variable    chbeba              ( bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {} );
    Variable    select              ( bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {} );
    void        clear               () { ref = 0; type = 0; }
    Variable    apply               ( bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {}, ApplyFlags apply_flags = ApplyFlags::NONE, const Vec<size_t> &spreads = {} );

    String      as_String           () const;
    SI32        as_SI32             () const;

    template<class T>
    T          *rcast               () const { return (T *)ref->rcast(); }

    RcPtr<Ref>  ref;
    Type       *type;
    Flags       flags;
    SI32        offset;
};
ENUM_FLAGS( Variable::Flags )
