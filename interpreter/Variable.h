#pragma once

#include "System/EnumFlags.h"
#include "System/Rc_string.h"
#include <set>
class Scope;
class Type;
class TCI;
class Vm;

/**
*/
class Variable {
public:
    enum class Flags: PI64 {
        NONE = 0, CONST = 1
    };
    struct Content : public Rc_obj {
        enum class Flags: PI64 { NONE = 0, NOT_CONSTRUCTED = 1, CONST = 2 };
        enum { PRC = 8 };

        Content( Vm *vm, Type *type, PT rese, Flags flags ) : vm( vm ), type( type ), rese( rese ), flags( flags ) {}
        ~Content();

        static Content *alloc( PT size, Vm *vm = 0, Type *type = 0, Flags flags = Flags::NONE ) { return new ( malloc( sizeof( Content ) - PRC + size ) ) Content( vm, type, size, flags ); }

        Vm   *vm;
        Type *type;
        PT    rese;        ///< to check stuff
        Flags flags;
        char  data[ PRC ];
    };

    Variable( Vm *vm, Type *type, void *value = 0, Content::Flags content_flags = Content::Flags::NONE, Flags flags = Flags::NONE );
    Variable( Rc_ptr<Content,true> content, Flags flags, Type *type, SI32 off );
    Variable( const Variable &var );
    Variable( Variable &&var );
    Variable();

    Variable            &operator=           ( const Variable &var );
    Variable            &operator=           ( Variable &&var );
    operator             bool                () const;

    Rc_string            valid_constraint_for( Scope *scope, const Variable &tested_var, TCI &tci ) const;
    void                 write_to_stream     ( std::ostream &os ) const;
    Variable             find_attribute      ( Scope *scope, const Rc_string &name, bool ret_err = true, bool msg_if_err = false ) const;
    void                 setup_vtables       ();
    bool                 get_PT_value        ( Scope *scope, PT &val ) const;
    Type                *ugs_type            ( Scope *scope, Variable *get_res = 0 ) const; ///< if getset, get type behind. Else, return type
    Variable             sub_part            ( Type *type, SI32 offset );
    bool                 is_false            ( Scope *scope );
    bool                 is_true             ( Scope *scope );
    char                *_reserve            ( Vm *vm, Type *type, Content::Flags content_flags, size_t rese_in_bytes );
    FP64                 as_FP64             ( Scope *scope ) const;
    Variable             chbeba              ( Scope *scope, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} );
    Variable             select              ( Scope *scope, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {} );
    Variable             apply               ( Scope *scope, bool want_ret, const Vec<Variable> &args = {}, const Vec<Rc_string> &names = {}, bool call_ctor = true, const Vec<size_t> &spreads = {} );
    void                 clear               () { content = 0; type = 0; }
    bool                 error               () const;
    SI32                 size                () const;
    SI32                 alig                () const;
    char                *ptr                 () const;
    Variable             ugs                 ( Scope *scope ) const; ///< un_getset

    Rc_ptr<Content,true> content;
    Flags                flags;
    Type                *type;
    SI32                 offB;    ///< in bytes
};
ENUM_FLAGS( Variable::Content::Flags )
ENUM_FLAGS( Variable::Flags )

