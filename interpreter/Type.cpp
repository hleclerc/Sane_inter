#include "System/rcast.h"
#include "GetSetter.h"
#include "Scope.h"
#include "Class.h"
#include "Type.h"
#include "Vm.h"

Type::Type( const Rc_string &name, PI32 size, PI32 alig ) : name( name ), size( size ), alig( alig ) {
    first_attribute = 0;
    last_attribute  = 0;
    has_new_vtable  = false;
    orig            = 0;
}

Type::~Type() {
}

bool Type::has_vtable_at_the_beginning() const {
    if ( has_new_vtable )
        return true;
    if ( Class *cl = orig_class() ) {
        for( const Rc_string &name : cl->inheritance_names ) {
            Type *inh_type = attributes.find( name )->second.type;
            if ( inh_type->has_vtable_at_the_beginning() )
                return true;
            if ( inh_type->size )
                break;
        }
    }
    return false;
}

Type *Type::self_or_par_has_orig_class( Class *cl ) {
    if ( Class *oc = orig_class() ) {
        if ( oc == cl )
            return this;
        for( const Rc_string &name : oc->inheritance_names )
            if ( Type *res = attributes.find( name )->second.type->self_or_par_has_orig_class( cl ) )
                return res;
    }
    return 0;
}

Rc_string Type::checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const {
    scope->add_error( "checks_type_constraint for type {}", *this );
    PE( name );
    TODO;
    return "not equal nor inherited";
}

bool Type::base_arythmetic_type() const {
    return false;
}

unsigned Type::get_nb_conversions( Scope *scope, const Variable &self ) const {
    PE( name );
    TODO;
    return 0;
}

void Type::write_to_stream( std::ostream &os, const char *content ) const {
    os << name << "(";
    static const char *c = "0123456789abcdef";
    for( SI32 i = 0; i < ( size + 7 ) / 8; ++i )
        os << ( i ? " " : "" ) << c[ ((unsigned char *)content)[ i ] / 16 ] << c[ ((unsigned char *)content)[ i ] % 16 ];
    os << ")";
}

void Type::write_to_stream( std::ostream &os ) const {
    os << name;
    if ( Class *cl = orig_class() ) {
        if ( cl->arg_names.size() ) {
            os << "[";
            for( size_t i = 0; i < cl->arg_names.size(); ++i )
                os << ( i ? "," : "" ) << *parameters[ i ];
            os << "]";
        }
    }
}

FP64 Type::get_pertinence( Scope *scope, const Variable &self ) const {
    P( name );
    TODO;
    return 0;
}

bool Type::get_condition( Scope *scope, const Variable &self ) const {
    P( name );
    TODO;
    return false;
}

void Type::get_fail_info( Scope *scope, const Variable &self, size_t &offset, Rc_string &source, Rc_string &msg ) const {
    P( name );
    TODO;
}

void Type::add_attribute( const Rc_string &name, SI32 off, Type *type, Variable::Flags flags ) {
    Attribute *attr = &attributes.emplace( name, Attribute{ name, off, type, flags, last_attribute, 0 } ).first->second;
    ( last_attribute ? last_attribute->next : first_attribute ) = attr;
    last_attribute = attr;
}

Variable Type::make_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const {
    P( name );
    TODO;
    return {};
}

bool Type::destroy_attrs() const {
    return true;
}

Variable Type::use_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const {
    P( name );
    TODO;
    return {};
}

bool Type::inherits( Scope *scope, Type *type ) {
    if ( this == type )
        return true;
    if ( ! orig_class() )
        return false;
        //scope->add_error( "ds57 {} {}", name, type->name );
        //ERROR( "..." );
    for( const Rc_string &sup : orig_class()->inheritance_names )
        if ( attributes[ sup ].type->inherits( scope, type ) )
            return true;
    return false;
}

bool Type::has_a_constructor() const {
    return true;
}

void Type::apply_on_vtables( std::function<void (Type *, SI32)> f, SI32 offB ) {
    if ( Class *cl = orig_class() ) {
        for( const Rc_string &name : cl->inheritance_names ) {
            const Attribute &attr = attributes[ name ];
            attr.type->apply_on_vtables( f, offB + attr.off_in_bits / 8 );
        }
    }
    if ( has_new_vtable )
        f( this, offB );
}

void Type::inheritance_rec( std::set<Type *> &res, bool start_with_parents ) {
    if ( ! start_with_parents )
        res.insert( this );
    if ( Class *cl = orig_class() )
        for( const Rc_string &name : cl->inheritance_names )
            attributes.find( name )->second.type->inheritance_rec( res, false );
}

Class *Type::orig_class() const {
    return orig && *orig ? reinterpret_cast<Class *>( orig->ptr() ) : 0;
}

bool Type::getsetter() const {
    return false;
}

void Type::construct( Scope *scope, const Variable &self, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    Scope new_scope( Scope::Scope_type::BLOCK, scope );
    new_scope.ctor_self = self;

    if ( Variable constructor = self.find_attribute( scope, "construct", false ) )
        constructor.apply( scope, false, args, names );
    else
        scope->add_error( "Found no 'construct' method" );
}

void Type::destroy( Scope *scope, const Variable &self, bool use_virtual ) {
    self.find_attribute( scope, "destroy" ).apply( scope, false, {}, {}, use_virtual );
}

void Type::spread_in( Scope *scope, const Variable &self, Vec<Variable> &res, Vec<Rc_string> &names ) {
    P( name );
    TODO;
}

Variable Type::with_self( Scope *scope, const Variable &orig, const Variable &new_self ) {
    return orig;
}

void Type::bin_ctor( Scope *scope, const Variable &self ) {
}

bool Type::is_false( Scope *scope, const Variable &self ) {
    return scope->find_variable( "Bool" ).apply( scope, true, self ).is_false( scope );
}

bool Type::is_true( Scope *scope, const Variable &self ) {
    return scope->find_variable( "Bool" ).apply( scope, true, self ).is_true( scope );
}

Variable Type::chbeba( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    Variable op = self.find_attribute( scope, "operator {}", false );
    if ( ! op )
        return scope->add_error( "class {} has no 'operator {}'", *self.ugs_type( scope ), "{}" ), scope->vm->ref_error;
    return op.apply( scope, want_ret, args, names );
}

Variable Type::select( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    Variable op = self.find_attribute( scope, "operator []", false );
    if ( ! op )
        return scope->add_error( "class {} has no 'operator []'", *self.ugs_type( scope ) ), scope->vm->ref_error;
    return op.apply( scope, want_ret, args, names );
}

Variable Type::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    Variable op = self.find_attribute( scope, "operator ()", false );
    if ( ! op ) {
        // ERROR( "no operator ()" );
        return scope->add_error( "class {} has no 'operator ()'", *self.ugs_type( scope ) ), scope->vm->ref_error;
    }
    return op.apply( scope, want_ret, args, names );
}

bool Type::error() const {
    return false;
}

void Type::disp_attributes( std::ostream &os ) {
    for( Attribute *attr = first_attribute; attr; attr = attr->next )
        if ( attr->name )
            std::cerr << " " << attr->name;
    if ( Class *cl = orig_class() ) {
        for( const Rc_string &name : cl->inheritance_names ) {
            attributes.find( name )->second.type->disp_attributes( os );
        }
    }
}

Variable Type::find_attribute( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off ) {
    //
    if ( error() )
        return scope->vm->ref_error;

    // instance attributes
    auto iter_attr = attributes.find( name );
    if ( iter_attr != attributes.end() ) {
        if ( content )
            return { content, flags, iter_attr->second.type, off + iter_attr->second.off_in_bits / 8 };
        if ( gs ) {
            // this time, we need to execute the get_... method
            Variable get = gs->get_var( scope );
            return { get.content, flags | get.flags, iter_attr->second.type, off + get.offB + iter_attr->second.off_in_bits / 8 };
        }
    }

    // methods
    auto iter_sattr = methods.find( name );
    if ( iter_sattr != methods.end() )
        return orig ? iter_sattr->second->type->with_self( scope, *iter_sattr->second, *orig ) : *iter_sattr->second;

    // static attributes
    iter_sattr = static_attributes.find( name );
    if ( iter_sattr != static_attributes.end() )
        return *iter_sattr->second;

    // template arguments
    if ( Class *cl = orig_class() )
        for( size_t i = 0; i < cl->arg_names.size(); ++i )
            if ( cl->arg_names[ i ] == name )
                return *parameters[ i ];

    // try in inheritance (if not looking for a destructor)
    if ( Class *cl = orig_class() ) {
        if ( name != "destroy" ) {
            for( const Rc_string inh_name : cl->inheritance_names ) {
                auto iter_attr = this->attributes.find( inh_name );
                ASSERT( iter_attr != this->attributes.end(), "" );
                if ( Variable res = iter_attr->second.type->find_attribute( scope, name, orig, gs, content, flags | iter_attr->second.flags, off + iter_attr->second.off_in_bits / 8 ) )
                    return res;
            }
        }
    }

    return {};
}
