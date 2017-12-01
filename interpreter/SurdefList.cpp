#include "System/rcast.h"
#include "SurdefList.h"
#include "Class.h"
#include "equal.h"
#include "Def.h"
#include "Vm.h"

SurdefList::SurdefList() {
}

bool SurdefList::only_contains_defs_named( Vm *vm, const Rc_string &name ) const {
    if ( lst.size() == 0 )
        return false;
    for( const Variable &var : lst ) {
        if ( var.type != vm->type_Def )
            return false;
        Def *def = rcast( var.ptr() );
        if ( def->name != name )
            return false;
    }
    return true;
}

Rc_string SurdefList::possible_names( Vm *vm ) const {
    std::set<Rc_string> set;
    for( const Variable &var : lst ) {
        if ( var.type == vm->type_Def ) {
            Def *def = rcast( var.ptr() );
            set.insert( def->name );
        } else if ( var.type == vm->type_Class ) {
            Class *def = rcast( var.ptr() );
            set.insert( def->name );
        }
    }

    Rc_string res;
    for( const Rc_string &n : set )
        res = res + ( res.empty() ? "" : "|" ) + n;
    return res;
}

bool SurdefList::has_wildcards( Vm *vm ) const {
    for( const Variable &v : args ) {
        if ( v.type == vm->type_Wildcard )
            return true;
        if ( v.type == vm->type_SurdefList ) {
            SurdefList *sl = rcast( v.ptr() );
            if ( sl->has_wildcards( vm ) )
                return true;
        }
        // TODO: maybe no enough...
    }
    return false;
}

bool SurdefList::eq( Scope *scope, SurdefList *sl ) {
    if ( lst.size() != sl->lst.size() )
        return false;
    if ( args.size() != sl->args.size() )
        return false;
    if ( names.size() != sl->names.size() )
        return false;

    auto ne = [&]( const Variable &a, const Variable &b ) {
        return ! equal( scope, a, b );
    };

    bool only_classes = true;
    for( size_t i = 0; i < lst.size(); ++i ) {
        if ( ne( lst[ i ], sl->lst[ i ] ) )
            return false;
        only_classes &= lst[ i ].type == scope->vm->type_Class;
    }
    if ( only_classes == false && self && ( bool( self ) != bool( sl->self ) || ne( self, sl->self ) ) )
        return false;
    for( size_t i = 0; i < args.size(); ++i )
        if ( ne( args[ i ], sl->args[ i ] ) )
            return false;
    for( size_t i = 0; i < names.size(); ++i )
        if ( names[ i ] != sl->names[ i ] )
            return false;

    return true;
}

bool SurdefList::eq( Scope *scope, Type *type ) {
    // find the class
    for( const Variable &_def : lst ) {
        Variable def = _def.ugs( scope );
        if ( def.type == scope->vm->type_Class ) {
            Class *cl = rcast( def.ptr() );
            if ( type->orig_class() == cl ) {
                if ( self )
                    return false;
                // find specified arguments list
                Vec<Variable> specified_args( Size(), cl->arg_names.size() );
                if ( cl->arg_spreads.size() )
                    TODO;
                for( size_t i = 0; i < args.size() - names.size(); ++i )
                    specified_args[ i ] = args[ i ];
                for( size_t i = 0; i < names.size(); ++i ) {
                    int ind = cl->arg_names.index_first( names[ i ] );
                    if ( ind < 0 ) {
                        scope->add_error( "name does not exist in args of {}", cl->name );
                        return false;
                    }
                    specified_args[ ind ] = args[ args.size() - names.size() + i ];
                }
                // test specified arguments
                auto ne = [&]( const Variable &a, const Variable &b ) {
                    return ! equal( scope, a, b );
                };
                for( size_t i = 0; i < specified_args.size(); ++i )
                    if ( specified_args[ i ] && ne( *type->parameters[ i ], specified_args[ i ] ) )
                        return false;
                return true;
            }
        }
    }
    return false;
}
