#include "Type_GetSetter.h"
#include "System/rcast.h"

Type_GetSetter::Type_GetSetter() : Type_BaseBin( "GetSetter" ) {
}

Rc_string Type_GetSetter::checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->checks_type_constraint( scope, get, tested_var, tci );
}

Variable Type_GetSetter::find_attribute( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off ) {
    gs = rcast( content->data + off / 8 );

    if ( name == "operator =" && gs->set )
        return gs->set;

    Variable get;
    Type *type = gs->get_type( scope, &get );
    return type->find_attribute( scope, name, orig, gs, get ? get.content.ptr() : 0, flags, get ? get.offB : 0 );

}

FP64 Type_GetSetter::get_pertinence( Scope *scope, const Variable &self ) const {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->get_pertinence( scope, get );
}

bool Type_GetSetter::get_condition( Scope *scope, const Variable &self ) const {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->get_condition( scope, get );
}

void Type_GetSetter::get_fail_info( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->get_fail_info( scope, get, offset, source, msg );
}

Variable Type_GetSetter::make_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->make_sl_trial( scope, want_ret, func, get, sl_args, sl_names, args, names, call_ctor );
}

Variable Type_GetSetter::use_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->use_sl_trial( scope, want_ret, func, get, sl_args, sl_names, args, names, call_ctor, trial );
}

void Type_GetSetter::spread_in( Scope *scope, const Variable &self, Vec<Variable> &res, Vec<Rc_string> &names ) {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->spread_in( scope, get, res, names );
}

Variable Type_GetSetter::with_self( Scope *scope, const Variable &orig, const Variable &new_self ) {
    GetSetter *gs = rcast( orig.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->with_self( scope, get, new_self );
}

bool Type_GetSetter::getsetter() const {
    return true;
}

bool Type_GetSetter::is_false( Scope *scope, const Variable &self ) {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->is_false( scope, get );
}

bool Type_GetSetter::is_true( Scope *scope, const Variable &self ) {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->is_true( scope, get );
}

Variable Type_GetSetter::chbeba( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->chbeba( scope, get, want_ret, args, names );
}

Variable Type_GetSetter::select( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->select( scope, get, want_ret, args, names );
}

Variable Type_GetSetter::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    GetSetter *gs = rcast( self.ptr() );
    Variable get = gs->get_var( scope );
    return get.type->apply( scope, get, want_ret, args, names, call_ctor );
}

