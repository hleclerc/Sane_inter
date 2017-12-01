#include "System/rcast.h"
#include "Type_Type.h"
#include "Class.h"
#include "Scope.h"
#include "TCI.h"

Type_Type::Type_Type() : Type_BaseBin( "Type" ) {
}

void Type_Type::write_to_stream( std::ostream &os, const char *content ) const {
    Type * const *type = rcast( content );
    os << **type;
}

bool Type_Type::destroy_attrs() const {
    return false;
}

Variable Type_Type::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    Type **type = rcast( self.ptr() );
    return make_inst( scope, *type, args, names, call_ctor );
}

Rc_string Type_Type::checks_type_constraint( Scope *scope, const Variable &self, const Variable &tested_var, TCI &tci ) const {
    // types are equal ?
    Type *nt = *reinterpret_cast<Type **>( self.ptr() );
    if ( nt == tested_var.type )
        return {};

    // try recursively inheritance of tested_var
    if ( Class *cl = tested_var.type->orig_class() )
        for( const Rc_string &inh_name : cl->inheritance_names )
            if ( checks_type_constraint( scope, self, tested_var.find_attribute( scope, inh_name ), tci ).empty() )
                return ++tci.nb_conversions, Rc_string{};

    // try operator "is_also_a"
    if ( Variable op = tested_var.find_attribute( scope, "operator is_also_a", false, false ) )
        if ( double score = op.apply( scope, true, self ).as_FP64( scope ) )
             return tci.nb_conversions += score, Rc_string{};

    // -> fail
    return "not equal nor inherited";
}

Variable Type_Type::make_inst( Scope *scope, Type *type, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    // check that all abstract surdefs are defined
    if ( call_ctor && type->abstract_methods.size() ) {
        std::string am;
        for( const FunctionSignature &fs : type->abstract_methods )
            am += std::string( am.empty() ? "" : ", " ) + fs.name;
        scope->add_error( "{} contains abstract methods ({}) and should not be instantiated", *type, am );
    }

    // make an instance
    Variable res( scope->vm, type, 0 );

    // call constructor if necessary
    if ( call_ctor )
        res.type->construct( scope, res, args, names );
    else
        res.content->flags |= Variable::Content::Flags::NOT_CONSTRUCTED;

    return res;
}
