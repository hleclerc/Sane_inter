#include "Variable.h"
#include "Class.h"
#include "Type.h"
#include "gvm.h"

Type::Type( const LString &name ) {
    content.data.name = name;
}

bool Type::has_vtable_at_the_beginning() const {
    if ( content.data.has_new_vtable )
        return true;
    if ( Class *cl = orig_class() ) {
        for( const RcString &name : cl->inheritance_names ) {
            Type *inh_type = content.data.attributes.find( name )->second.type;
            if ( inh_type->has_vtable_at_the_beginning() )
                return true;
            if ( inh_type->content.data.size )
                break;
        }
    }
    return false;
}

RcString Type::checks_type_constraint( const Variable &self, const Variable &tested_var, TCI &tci ) const {
    gvm->add_error( "checks_type_constraint for type {}", *this );
    PE( content.data.name );
    TODO;
    return "not equal nor inherited";
}

unsigned Type::get_nb_conversions( const Variable &self ) const {
    PE( content.data.name );
    TODO;
    return 0;
}

void Type::write_to_stream( std::ostream &os ) const {
    os << content;
}

void Type::write_cst( std::ostream &os, const PI8 *data, int offset_mod_8 ) const {
    os << content;
}

void Type::destroy( const Variable &self, bool use_virtual ) {
}

void Type::spread_in( const Variable &self, Vec<Variable> &res, Vec<RcString> &names ) {
    TODO;
}

void Type::construct( const Variable &self, const Vec<Variable> &args, const Vec<RcString> &names ) {
    Scope new_scope( Scope::ScopeType::BLOCK );
    new_scope.ctor_self = self;

    if ( Variable constructor = self.find_attribute( "construct", false ) )
        constructor.apply( false, args, names );
    else
        gvm->add_error( "Found no 'construct' method" );

}

double Type::get_pertinence( const Variable &self ) const {
    PE( content.data.name );
    TODO;
    return 0;
}

bool Type::get_condition( const Variable &self ) const {
    PE( content.data.name );
    TODO;
    return 0;
}

void Type::get_fail_info( const Variable &self, size_t &offset, RcString &source, RcString &msg ) const {
    PE( content.data.name );
    TODO;
}

bool Type::destroy_attrs() const {
    return true;
}

void Type::add_attribute( const RcString &name, SI32 off, Type *type, Variable::Flags flags ) {
    TypeContent::Attribute *attr = &content.data.attributes.emplace( name, TypeContent::Attribute{ name, type, off, flags, content.data.last_attribute, 0 } ).first->second;
    ( content.data.last_attribute ? content.data.last_attribute->next : content.data.first_attribute ) = attr;
    content.data.last_attribute = attr;
}

Variable Type::make_sl_trial( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) const {
    PE( content.data.name );
    TODO;
    return {};
}

Variable Type::use_sl_trial( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags, const Variable &trial ) const {
    PE( content.data.name );
    TODO;
    return {};
}

Variable Type::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) {
    TODO; return {};
}

bool Type::error() const {
    return false;
}

Variable Type::find_attribute( const RcString &name, const Variable &self, Variable::Flags flags, SI32 off ) const {
    // instance attributes
    auto iter_attr = content.data.attributes.find( name );
    if ( iter_attr != content.data.attributes.end() )
        return { self.ref, iter_attr->second.type, off + iter_attr->second.off, flags };

    // methods
    auto iter_sattr = content.data.methods.find( name );
    if ( iter_sattr != content.data.methods.end() )
        return self ? iter_sattr->second->type->with_self( *iter_sattr->second, self ) : *iter_sattr->second;

    // static attributes
    iter_sattr = content.data.static_attributes.find( name );
    if ( iter_sattr != content.data.static_attributes.end() )
        return *iter_sattr->second;

    // template arguments
    if ( Class *cl = orig_class() )
        for( size_t i = 0; i < cl->arg_names.size(); ++i )
            if ( cl->arg_names[ i ] == name )
                return *content.data.parameters[ i ];

    // try in inheritance (if not looking for a destructor)
    if ( Class *cl = orig_class() ) {
        if ( name != "destroy" ) {
            for( const RcString &inh_name : cl->inheritance_names ) {
                auto iter_attr = content.data.attributes.find( inh_name );
                ASSERT( iter_attr != content.data.attributes.end(), "" );
                if ( Variable res = iter_attr->second.type->find_attribute( name, self, flags | iter_attr->second.flags, off + iter_attr->second.off ) )
                    return res;
            }
        }
    }

    return {};
}

Variable Type::with_self( Variable &orig, const Variable &new_self ) const {
    PE( content.data.name );
    TODO;
    return {};
}

Class *Type::orig_class() const {
    return content.data.orig_class;
}
