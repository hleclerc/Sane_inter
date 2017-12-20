#include "SurdefList.h"
#include "equal.h"
#include "Class.h"
#include "Type.h"
#include "Def.h"
#include "gvm.h"

bool SurdefList::only_contains_defs_named( const RcString &name ) const {
    if ( lst.size() == 0 )
        return false;
    for( const Variable &var : lst ) {
        if ( var.type != gvm->type_Def )
            return false;
        Def *def = var.rcast<Def>();
        if ( def->name != name )
            return false;
    }
    return true;
}

RcString SurdefList::possible_names() const {
    std::set<RcString> set;
    for( const Variable &var : lst ) {
        if ( var.type == gvm->type_Def ) {
            Def *def = var.rcast<Def>();
            set.insert( def->name );
        } else if ( var.type == gvm->type_Class ) {
            Class *def = var.rcast<Class>();
            set.insert( def->name );
        }
    }

    RcString res;
    for( const RcString &n : set )
        res = res + ( res.empty() ? "" : "|" ) + n;
    return res;
}

bool SurdefList::has_wildcards() const {
    for( const Variable &v : args ) {
        if ( v.type == gvm->type_Wildcard )
            return true;
        if ( v.type == gvm->type_SurdefList ) {
            SurdefList *sl = v.rcast<SurdefList>();
            if ( sl->has_wildcards() )
                return true;
        }
        // TODO: maybe no enough...
    }
    return false;
}

bool SurdefList::eq( SurdefList *sl ) {
    if ( lst.size() != sl->lst.size() )
        return false;
    if ( args.size() != sl->args.size() )
        return false;
    if ( names.size() != sl->names.size() )
        return false;

    auto ne = [&]( const Variable &a, const Variable &b ) {
        return ! equal( a, b );
    };

    for( size_t i = 0; i < lst.size(); ++i )
        if ( ne( lst[ i ], sl->lst[ i ] ) )
            return false;
    for( size_t i = 0; i < args.size(); ++i )
        if ( ne( args[ i ], sl->args[ i ] ) )
            return false;
    for( size_t i = 0; i < names.size(); ++i )
        if ( names[ i ] != sl->names[ i ] )
            return false;

    return true;
}

bool SurdefList::eq( Type *type ) {
    // find the class
    for( const Variable &def : lst ) {
        if ( def.type == gvm->type_Class ) {
            Class *cl = def.rcast<Class>();
            if ( type->orig_class() == cl ) {
                // find specified arguments list
                Vec<Variable> specified_args( Size(), cl->arg_names.size() );
                if ( cl->arg_spreads.size() )
                    TODO;
                for( size_t i = 0; i < args.size() - names.size(); ++i )
                    specified_args[ i ] = args[ i ];
                for( size_t i = 0; i < names.size(); ++i ) {
                    int ind = cl->arg_names.index_first( names[ i ] );
                    if ( ind < 0 ) {
                        gvm->add_error( "name does not exist in args of {}", cl->name );
                        return false;
                    }
                    specified_args[ ind ] = args[ args.size() - names.size() + i ];
                }
                // test specified arguments
                auto ne = [&]( const Variable &a, const Variable &b ) {
                    return ! equal( a, b );
                };
                for( size_t i = 0; i < specified_args.size(); ++i )
                    if ( specified_args[ i ] && ne( *type->content.data.parameters[ i ], specified_args[ i ] ) )
                        return false;
                return true;
            }
        }
    }
    return false;
}
