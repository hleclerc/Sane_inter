#include "Ast_visitor_Vm.h"
#include "System/rcast.h"
#include "Type_Block.h"
#include "Scope.h"
#include "Vm.h"

Type_Block::Type_Block() : Type_BaseBin( "Block" ) {
}

Variable Type_Block::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    if ( args.size() != 1 )
        return scope->add_error( "block() is expecting only 1 argument" ), scope->vm->ref_error;

    Block *block = rcast( self.ptr() );
    if ( ! block )
        return scope->add_error( "A Block inst was expected" ), scope->vm->ref_error;

    // find the calling scope
    Scope *scope_for_beg = 0;
    int nb_for = 1;
    for( const Scope *s = scope; s; s = s->parent ) {
        if ( s->type == Scope::Scope_type::FOR_BEG ) {
            if ( --nb_for == 0 ) {
                scope_for_beg = s->parent;
                break;
            }
        } else if ( s->type == Scope::Scope_type::FOR_EXE )
            ++nb_for;
    }

    Scope new_scope( Scope::Scope_type::FOR_EXE, scope );
    new_scope.reg_var( block->name, args[ 0 ] );
    if ( scope_for_beg ) {
        for( Scope *s = scope_for_beg; s; s = s->parent_for_vars() ) {
            if ( s->self ) {
                new_scope.self = s->self;
                break;
            }
        }
    }

    // catched variables
    for( const CatchedVariable &cv : block->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );
    // new_scope.valid_scope_ptr = block->valid_scope_ptr.get_scope();

    // execute
    new_scope.visit( block->code, false );

    //    // handle breaks (actually handle only the continue(s))
    //    for( size_t i = old_breaks_size; i < scope->vm->breaks.size(); ++i )
    //        if ( scope->vm->breaks[ i ].nb_l == 0 )
    //            scope->vm->breaks.remove( i-- );

    return scope->vm->ref_error;
}
