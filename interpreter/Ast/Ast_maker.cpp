#include "../System/va_string.h"
#include "../System/Assert.h"
#include "Ast_maker.h"
#include "Lexer.h"

namespace {

/// != get_children_of_type because it takes left and right arguments
static unsigned nb_child_and( const Lexem *t ) {
    if ( t->type != OPERATOR_and_boolean___NUM ) {
        if ( ( t->type == Lexem::PAREN or t->type == Lexem::BLOCK ) and t->children[ 0 ] )
            return nb_child_and( t->children[ 0 ] );
        return 1;
    }
    return nb_child_and( t->children[ 0 ] ) + nb_child_and( t->children[ 1 ] );
}


static bool is_a_method( const Lexem *t ) {
    while ( t and not t->parent )
        t = t->prev;
    while ( t and t->parent->type == Lexem::BLOCK )
        t = t->parent;
    return t and t->parent->type == OPERATOR___class___NUM;
}

static inline bool hex( char c ) {
    return ( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' );
}

static inline PI8 hex_val( char c ) {
    if ( c >= '0' && c <= '9' )
        return c - '0';
    if ( c >= 'a' && c <= 'f' )
        return 10 + c - 'a';
    return 10 + c - 'A';
}

}

Ast_maker::Ast_maker( Error_list *error_list ) : error_list( error_list ) {
    global_inst    = false;
    export_inst    = false;
    static_inst    = false;
    mixin_inst     = false;
    const_inst     = false;
    private_inst   = false;
    protected_inst = false;
    virtual_inst   = false;
    override_inst  = false;
    inline_inst    = false;
    abstract_inst  = false;
}

void Ast_maker::parse( const char *content, const char *sourcefile, bool display_lexems ) {
    // src => lexem tree
    Lexer lx( error_list );
    size_t old_el_size = error_list->size();
    lx.parse( content, sourcefile );
    if ( error_list->size() != old_el_size )
        return;

    if ( display_lexems )
        lx.display();

    //
    Vec<const Lexem *> lxs;
    for( const Lexem *l = lx.root(); l; l = l->next )
        lxs.push_back( l );

    // write first item
    if ( lxs.size() != 1 )
        aw.write_ss_block( lxs ); // _write_VecLex( lxs );
    else
        make_single( lxs[ 0 ] );

    // pending lexems (indirection)
    while ( ! aw.ql.empty() ) {
        Ast_writer::Waiting p = aw.ql.front();
        aw.ql.pop_front();
        *p.first = aw.bs.size();
        auto _a = raii_save( private_inst  , p.flags & PRIVATE   );
        auto _b = raii_save( protected_inst, p.flags & PROTECTED );
        make_single( p.second );
    }

    // crunch offsets
    aw.bs.crunch();
}

void Ast_maker::make_single( const Lexem *l ) {
    if ( ! l )
        return make_ast_pass( 0 );

    switch( l->type )  {
    case Lexem::VARIABLE                   : return make_ast_variable ( l );
    case Lexem::NUMBER                     : return make_ast_number   ( l );
    case Lexem::STRING                     : return make_ast_string   ( l );
    case Lexem::CR                         : ERROR( "CR should have disapeared" ); // return;
    case Lexem::PAREN                      : return make_ast_paren    ( l ); // case 1: [], else simple block
    case Lexem::CCODE                      : TODO; return;
    case Lexem::BLOCK                      : return make_ast_block    ( l->children[ 0 ] );
    case Lexem::NONE                       : return aw.write_pass();
    case Lexem::APPLY                      : return make_ast_apply    ( l, [&]( const Lexem *f, const Vec<const Lexem *> &v, const Vec<Hpipe::CmString> &n, const Vec<size_t> &s ) { aw.write_apply ( l, f, v, n, s ); } );
    case Lexem::SELECT                     : return make_ast_apply    ( l, [&]( const Lexem *f, const Vec<const Lexem *> &v, const Vec<Hpipe::CmString> &n, const Vec<size_t> &s ) { aw.write_select( l, f, v, n, s ); } );
    case Lexem::CHBEBA                     : return make_ast_apply    ( l, [&]( const Lexem *f, const Vec<const Lexem *> &v, const Vec<Hpipe::CmString> &n, const Vec<size_t> &s ) { aw.write_chbeba( l, f, v, n, s ); } );
    // case OPERATOR___new___NUM           : return make_ast_new      ( l );
    case OPERATOR_assign_NUM               : return make_ast_assign   ( l, 0 );
    case OPERATOR_assign_ref_NUM           : return make_ast_assign   ( l, 1 );
    case OPERATOR_assign_type_NUM          : return make_ast_assign   ( l, 2 );
    case OPERATOR___def___NUM              : return make_ast_callable ( l, true  );
    case OPERATOR___class___NUM            : return make_ast_callable ( l, false );
    case OPERATOR___enum___NUM             : return make_ast_enum     ( l );
    case OPERATOR_pass_NUM                 : return make_ast_pass     ( l );
    case OPERATOR___init_of___NUM          : return make_ast_init_of  ( l );
    case OPERATOR___if___NUM               : return make_ast_if       ( l );
    case OPERATOR___try___NUM              : return make_ast_try      ( l );
    case OPERATOR___while___NUM            : return make_ast_while    ( l );
    case OPERATOR_get_attr_NUM             : return make_ast_get_attr ( l, 0, 0, 0 );
    case OPERATOR_get_attr_ptr_NUM         : return make_ast_get_attr ( l, 1, 0, 0 );
    case OPERATOR_get_attr_ask_NUM         : return make_ast_get_attr ( l, 0, 1, 0 );
    case OPERATOR_get_attr_ptr_ask_NUM     : return make_ast_get_attr ( l, 1, 1, 0 );
    case OPERATOR_doubledoubledot_NUM      : return make_ast_get_attr ( l, 0, 0, 1 );
    case OPERATOR___global___NUM           : return make_ast_global   ( l );
    case OPERATOR___export___NUM           : return make_ast_export   ( l );
    case OPERATOR___static___NUM           : return make_ast_static   ( l );
    case OPERATOR___virtual___NUM          : return make_ast_virtual  ( l );
    case OPERATOR___override___NUM         : return make_ast_override ( l );
    case OPERATOR___inline___NUM           : return make_ast_inline   ( l );
    case OPERATOR___abstract___NUM         : return make_ast_abstract ( l );
    case OPERATOR___mixin___NUM            : return make_ast_mixin    ( l );
    case OPERATOR___const___NUM            : return make_ast_const    ( l );
    case OPERATOR___for___NUM              : return make_ast_for      ( l );
    case OPERATOR___import___NUM           : return make_ast_import   ( l );
    case OPERATOR_double_arrow_NUM         : return make_ast_lambda   ( l );
    case OPERATOR_and_boolean___NUM        :
    case OPERATOR_and_boolean_sgn___NUM    : return make_ast_and      ( l );
    case OPERATOR_or_boolean___NUM         :
    case OPERATOR_or_boolean_sgn___NUM     : return make_ast_or       ( l );
    case OPERATOR___else___NUM             : return make_ast_or       ( l );
    case OPERATOR___alternative___NUM      : return make_ast_alternat ( l );
    case OPERATOR___variable_assembly___NUM: return make_var_assembly ( l );
    default                                : return make_ast_call_op  ( l );
    }
}

Vec<const Lexem *> Ast_maker::make_lex_vec_from( const Lexem *l, const Lexem *end ) {
    Vec<const Lexem *> res;
    if ( l ) {
        while ( l->type == Lexem::BLOCK && l->next == 0 )
            l = l->children[ 0 ];
        for( ; l && l != end; l = l->next )
            res << l;
    }
    return res;
}

void Ast_maker::add_error( const std::string &msg, const Lexem *l ) {
    Error_list::Error &e = error_list->add( msg ).ac( l->beg_src, l->o_beg, l->src );
    std::cerr << e;
}

Hpipe::CmString Ast_maker::inheritance_name( size_t i ) {
    if ( i >= inheritance_name_string_pool.size() )
        inheritance_name_string_pool.resize( i + 1 );
    std::string &str = inheritance_name_string_pool[ i ];
    if ( str.empty() )
        str = i ? "super_" + std::to_string( i ) : "super";
    return { str.data(), str.size() };
}

const Lexem *Ast_maker::parent_class( const Lexem *lex ) {
    while( lex->prev )
        lex = lex->prev;
    while ( lex->parent ) {
        lex = lex->parent;
        if ( lex->type == OPERATOR___class___NUM )
            return lex;
    }
    return 0;
}

void Ast_maker::write_to_stream( std::ostream &os ) const {
    aw.cq.data_visitor( [&]( const PI8 *beg, const PI8 *end ) {
        for( ; beg < end; ++beg )
            os << (int)*beg << " ";
    } );
}

Ast_crepr Ast_maker::to_Ast_crepr() const {
    // code
    Rc_string code( aw.cq.size() );
    char *ptr_code = code.begin();
    aw.cq.data_visitor( [ &ptr_code ]( const PI8 *b, const PI8 *e ) {
        memcpy( ptr_code, b, e - b );
        ptr_code += e - b;
    } );

    // names
    Vec<const std::string *> vn( Size(), aw.src_map.size()  );
    for( const auto &p : aw.src_map )
        vn[ p.second - 1 ] = &p.first;

    Hpipe::CbQueue cq_names;
    Hpipe::BinStream<Hpipe::CbQueue> bq_names( &cq_names );
    bq_names << vn.size();
    for( const std::string *name : vn )
        bq_names << *name;

    Rc_string names( cq_names.size() );
    char *ptr_names = names.begin();
    cq_names.data_visitor( [ &ptr_names ]( const PI8 *b, const PI8 *e ) {
        memcpy( ptr_names, b, e - b );
        ptr_names += e - b;
    } );

    return { std::move( names ), std::move( code ) };
}

void Ast_maker::make_ast_variable( const Lexem *lex ) {
    if ( lex->eq( "continue" ) ) return aw.write_continue( lex );
    if ( lex->eq( "break"    ) ) return aw.write_break   ( lex );
    if ( lex->eq( "false"    ) ) return aw.write_false   ();
    if ( lex->eq( "true"     ) ) return aw.write_true    ();
    if ( lex->eq( "self"     ) ) return aw.write_self    ( lex );
    if ( lex->eq( "this"     ) ) return aw.write_this    ( lex );
    aw.write_variable( lex, lex->beg, lex->len );
}

void Ast_maker::make_ast_number( const Lexem *lex ) {
    aw.write_number( lex, lex->beg, lex->len );
}

void Ast_maker::make_ast_string( const Lexem *lex ) {
    std::string str;
    for( const char *c = lex->beg + bool( lex->num & 1 ), *e = lex->beg + lex->len - bool( lex->num & 2 ); c < e; ++c ) {
        if ( c[ 0 ] == '\\' && c + 1 < e ) {
            if      ( c[ 1 ] == 'a'  ) { str += 0x07; ++c; } // Alert (Beep, Bell) (added in C89)[1]
            else if ( c[ 1 ] == 'b'  ) { str += 0x08; ++c; } // Backspace
            else if ( c[ 1 ] == 'e'  ) { str += 0x1B; ++c; } // Escape
            else if ( c[ 1 ] == 'f'  ) { str += 0x0C; ++c; } // Formfeed
            else if ( c[ 1 ] == 'n'  ) { str += 0x0A; ++c; } // Newline (Line Feed); see notes below
            else if ( c[ 1 ] == 'r'  ) { str += 0x0D; ++c; } // Carriage Return
            else if ( c[ 1 ] == 't'  ) { str += 0x09; ++c; } // Horizontal Tab
            else if ( c[ 1 ] == 'v'  ) { str += 0x0B; ++c; } // Vertical Tab
            else if ( c[ 1 ] == '\\' ) { str += 0x5C; ++c; } // Backslash
            else if ( c[ 1 ] == '\'' ) { str += 0x27; ++c; } // Single quotation mark
            else if ( c[ 1 ] == '"'  ) { str += 0x22; ++c; } // Double quotation mark
            else if ( c[ 1 ] == '?'  ) { str += 0x3F; ++c; } // Question mark (used to avoid trigraphs)
            else if ( c[ 1 ] >= '0' && c[ 1 ] <= '7' ) {
                PI8 val = *( ++c ) - '0';
                if ( c + 1 < e && c[ 1 ] >= '0' && c[ 1 ] <= '7' ) val = 8 * val + *( ++c ) - '0';
                if ( c + 1 < e && c[ 1 ] >= '0' && c[ 1 ] <= '7' ) val = 8 * val + *( ++c ) - '0';
                str += val;
            }
            else if ( c[ 1 ] == 'x' ) {
                ++c;
                PI64 val = 0;
                while( c + 1 < e && hex( c[ 1 ] ) ) val = 16 * val + hex_val( *( ++c ) );
                str += PI8( val );
            }
            else if ( c[ 1 ] == 'u' ) {
                if ( c + 5 >= e || ! hex( c[ 2 ] ) || ! hex( c[ 3 ] ) || ! hex( c[ 4 ] ) || ! hex( c[ 5 ] ) )
                    add_error( "\\u must be followed by 4 hexadecimal values", lex );

                PI32 val = ( PI32( hex_val( c[ 2 ] ) ) << 12 ) +
                           ( PI32( hex_val( c[ 3 ] ) ) <<  8 ) +
                           ( PI32( hex_val( c[ 4 ] ) ) <<  4 ) +
                           ( PI32( hex_val( c[ 5 ] ) ) <<  0 );

                if ( val <= 0x7F ) {
                    str += val;
                } else if ( val <= 0x7FF ) {
                    str += 0xC0 | ( ( val >>  6 ) & 0x1F );
                    str += 0x80 | ( ( val       ) & 0x3F );
                } else if ( val <= 0xFFFF ) {
                    str += 0xE0 | ( ( val >> 12 ) & 0xF  );
                    str += 0x80 | ( ( val >>  6 ) & 0x3F );
                    str += 0x80 | ( ( val       ) & 0x3F );
                } else {
                    str += 0xF0 | ( ( val >> 18 ) & 0x7  );
                    str += 0x80 | ( ( val >> 12 ) & 0x3F );
                    str += 0x80 | ( ( val >>  6 ) & 0x3F );
                    str += 0x80 | ( ( val       ) & 0x3F );
                }

                c += 5;
            }
            else {
                add_error( va_string( "unknown escape sequence (\\{})", c[ 1 ] ), lex );
                str += *c;
            }
        }
        else {
            str += c[ 0 ];
        }
    }
    aw.write_string( lex, str.data(), str.size() );
}

void Ast_maker::make_ast_paren( const Lexem *l ) {
    // []
    if ( *l->beg == '[' ) {
        Vec<size_t> sizes;
        Vec<const Lexem *> lst;
        const Lexem *c = l->children[ 0 ];
        if ( c->type == Lexem::BLOCK )
            c = c->children[ 0 ];
        for( const Lexem *b = c; b; b = b->next ) {
            const Lexem *v = b;
            if ( v->type == Lexem::BLOCK )
                v = v->children[ 0 ];
            if ( v->type == Lexem::CR )
                continue;
            Vec<const Lexem *> chs;
            get_children_of_type( v, OPERATOR_comma_NUM, chs );
            sizes << chs.size();
            lst.append( chs );
        }

        //        if ( sizes.size() > 1 )
        //            return aw.write_matrix( lst, sizes );
        return aw.write_vec( lst );
    }

    // {}
    if ( *l->beg == '{' ) {
        Vec<const Lexem *> lst;
        const Lexem *c = l->children[ 0 ];
        if ( c->type == Lexem::BLOCK )
            c = c->children[ 0 ];
        for( const Lexem *b = c; b; b = b->next ) {
            const Lexem *v = b;
            if ( v->type == Lexem::BLOCK )
                v = v->children[ 0 ];
            if ( v->type == Lexem::CR )
                continue;
            Vec<const Lexem *> chs;
            get_children_of_type( v, OPERATOR_comma_NUM, chs );
            lst.append( chs );
        }

        Vec<const Lexem *> names;
        for( size_t i = 0; i < lst.size(); ++i ) {
            if ( lst[ i ]->type == OPERATOR_doubledot_NUM ) {
                names << lst[ i ]->children[ 0 ];
                lst [ i ] = lst[ i ]->children[ 1 ];
            } else {
                Lexem *n = lexem_pool.new_back_item();
                n->type = Lexem::STRING;
                n->beg     = lst[ i ]->beg;
                n->len     = lst[ i ]->len;
                n->o_beg   = lst[ i ]->o_beg;
                n->o_len   = lst[ i ]->o_len;
                n->beg_src = lst[ i ]->beg_src;
                n->src     = lst[ i ]->src;
                n->num     = 0;

                names << n;
            }
        }

        return aw.write_map( names, lst );
    }

    // else
    return make_ast_block( l->children[ 0 ] );
}

void Ast_maker::make_ast_block( const Lexem *lex ) {
    Vec<const Lexem *> vl;
    for( ; lex; lex = lex->next )
        vl << lex;
    make_ast_block( vl );
}

void Ast_maker::make_ast_assign( const Lexem *lex, int type ) {
    const Lexem *c = lex->children[ 1 ];
    if ( c->type == OPERATOR___ref___NUM ) {
        if ( type == 2 )
            add_error( "ref works on := operators, not no ~= which makes new instances", c );
        c = c->children[ 0 ];
        type = 1;
    }

    if ( lex->children[ 0 ]->type == Lexem::PAREN && lex->children[ 0 ]->beg[ 0 ] == '{' ) {
        Vec<Hpipe::CmString> names;
        Vec<const Lexem *> equals, types, chs;
        get_children_of_type( lex->children[ 0 ]->children[ 0 ], OPERATOR_comma_NUM, chs );
        for( const Lexem *ch : chs ) {
            const Lexem *eq = 0, *ty = 0;
            if ( ch->type == OPERATOR_doubledot_NUM ) {
                ty = ch->children[ 1 ];
                ch = ch->children[ 0 ];
            } else if ( ch->type == OPERATOR_reassign_NUM ) {
                if ( ch->children[ 0 ]->type == OPERATOR_doubledot_NUM ) {
                    eq = ch->children[ 1 ];
                    ty = ch->children[ 0 ]->children[ 1 ];
                    ch = ch->children[ 0 ]->children[ 0 ];
                } else if ( ch->children[ 1 ]->type == OPERATOR_doubledot_NUM ) {
                    eq = ch->children[ 1 ]->children[ 0 ];
                    ty = ch->children[ 1 ]->children[ 1 ];
                    ch = ch->children[ 0 ];
                } else {
                    eq = ch->children[ 1 ];
                    ch = ch->children[ 0 ];
                }
            }

            if ( ch->type != Lexem::VARIABLE )
                return add_error( "target of destructuring assignment must be variables", ch );
            names.push_back( ch->beg, ch->len );
            equals << eq;
            types << ty;

        }
        return aw.write_destruct_assign( lex, names, equals, types, c,
                static_inst    * ASSIGN_FLAG_static +
                const_inst     * ASSIGN_FLAG_const +
                ( type == 2 )  * ASSIGN_FLAG_type +
                export_inst    * ASSIGN_FLAG_export +
                global_inst    * ASSIGN_FLAG_global +
                private_inst   * ASSIGN_FLAG_private   +
                protected_inst * ASSIGN_FLAG_protected +
                ( type == 1 )  * ASSIGN_FLAG_ref );
    }

    PI8 nb_scopes_rec = 0;
    const Lexem *name = lex->children[ 0 ];
    for( ; name->type == OPERATOR_pointer_on_NUM; name = name->children[ 0 ] )
        ++nb_scopes_rec;
    const Lexem *cname = 0;
    if ( name->type != Lexem::VARIABLE ) {
        cname = name;

        // hum
        std::function<void(Lexem *)> repl_var_ass = [&]( Lexem *l ) {
            if ( l->type == OPERATOR___variable_assembly___NUM ) {
                l->type = OPERATOR___string_assembly___NUM;
                repl_var_ass( l->children[ 0 ] );
                repl_var_ass( l->children[ 1 ] );
            }
        };
        repl_var_ass( const_cast<Lexem *>( cname ) );

    }

    aw.write_assign( lex, name->beg, cname ? 0 : name->len, cname, nb_scopes_rec, c,
            static_inst    * ASSIGN_FLAG_static    +
            const_inst     * ASSIGN_FLAG_const     +
            ( type == 2 )  * ASSIGN_FLAG_type      +
            export_inst    * ASSIGN_FLAG_export    +
            global_inst    * ASSIGN_FLAG_global    +
            private_inst   * ASSIGN_FLAG_private   +
            protected_inst * ASSIGN_FLAG_protected +
            ( type == 1 )  * ASSIGN_FLAG_ref );
}

void Ast_maker::make_ast_callable( const Lexem *lex, bool def ) {
    // parsed data
    Vec<const Lexem *>   inheritance;
    Vec<Hpipe::CmString> inheritance_names;
    Vec<Hpipe::CmString> arg_names;
    Vec<const Lexem *>   args;
    Vec<const Lexem *>   arg_constraints;
    Vec<const Lexem *>   arg_def_vals;
    Vec<size_t>          arg_spreads;
    bool                 self_as_arg;
    SI32                 default_pertinence_num, default_pertinence_exp;
    const Lexem         *return_type;
    const Lexem         *pertinence;
    const Lexem         *condition;
    const Lexem         *block;
    const Lexem         *name;
    Vec<const Lexem *>   wpc;
    Vec<Hpipe::CmString> with_names;
    Vec<const Lexem *>   with_constraints;
    Vec<const Lexem *>   with_def_vals;
    Vec<size_t>          with_spreads;
    bool                 named_converter = false;


    // block
    block = lex->children[ 1 ]; // lex->children[ 1 ]->type == Lexem::PAREN or lex->children[ 1 ]->type == Lexem::BLOCK ?
    // lex->children[ 1 ]->children[ 0 ] : lex->children[ 1 ];

    // beginning of arguments
    const Lexem *c = lex->children[ 0 ];

    pertinence  = 0;
    condition   = 0;
    return_type = 0;
    while ( c ) {
        if ( c->type == OPERATOR___extends___NUM     ) {
            get_children_of_type( c->children[ 1 ], OPERATOR_comma_NUM, inheritance );
            for( size_t i = 0; i < inheritance.size(); ++i ) {
                if ( inheritance[ i ]->type == OPERATOR_doubledot_NUM ) {
                    const Lexem *n = inheritance[ i ]->children[ 0 ];
                    if ( n->type != Lexem::VARIABLE )
                        return add_error( "in 'extends a: A', a must be a variable (attribute name of the super class).", n );
                    inheritance_names.push_back( n->beg, n->len );
                    inheritance[ i ] = inheritance[ i ]->children[ 1 ];
                } else
                    inheritance_names << inheritance_name( i );
            }

            c = c->children[ 0 ];
            continue;
        }
        if ( c->type == OPERATOR___with___NUM     ) {
            Vec<const Lexem *> wl;
            get_children_of_type( c->children[ 1 ], OPERATOR_comma_NUM, wl );

            for( const Lexem *t : wl ) {
                const Lexem *arg = t;
                if ( t->type == OPERATOR___alternative___NUM ) {
                    arg = t->children[ 0 ];
                    if ( t->children[ 1 ]->type == OPERATOR_doubledot_NUM ) {
                        with_def_vals    << t->children[ 1 ]->children[ 0 ];
                        with_constraints << t->children[ 1 ]->children[ 1 ];
                    } else {
                        with_def_vals    << t->children[ 1 ];
                        with_constraints << 0;
                    }
                } else if ( t->type == OPERATOR_doubledot_NUM ) {
                    arg = t->children[ 0 ];
                    if ( t->children[ 1 ]->type == OPERATOR___alternative___NUM ) {
                        with_def_vals    << t->children[ 1 ]->children[ 1 ];
                        with_constraints << t->children[ 1 ]->children[ 0 ];
                    } else {
                        with_def_vals    << 0;
                        with_constraints << t->children[ 1 ];
                    }
                } else {
                    with_def_vals    << 0;
                    with_constraints << 0;
                }

                if ( arg->type == OPERATOR_triple_dots_NUM ) {
                    with_spreads << with_names.size();
                    arg = arg->children[ 0 ];
                }

                if ( arg->type != Lexem::VARIABLE )
                    return add_error( "expected here an argument name.", arg );
                with_names.push_back( arg->beg, arg->len );
            }

            c = c->children[ 0 ];
            continue;
        }
        if ( c->type == OPERATOR_ret_type_NUM        ) { return_type = c->children[ 1 ]; c = c->children[ 0 ]; continue; }
        if ( c->type == OPERATOR___pertinence___NUM  ) { pertinence  = c->children[ 1 ]; c = c->children[ 0 ]; continue; }
        if ( c->type == OPERATOR___when___NUM        ) { condition   = c->children[ 1 ]; c = c->children[ 0 ]; continue; }
        break;
    }

    // name and arguments
    if ( c->type == Lexem::APPLY or c->type == Lexem::SELECT ) {
        name = c->children[ 0 ];

        Vec<const Lexem *> tl;
        get_children_of_type( c->children[ 1 ], OPERATOR_comma_NUM, tl );

        for( size_t i = 0; i < tl.size(); ++i ) {
            const Lexem *t = tl[ i ];
            if ( not t )
                continue;

            const Lexem *arg = t;
            if ( t->type == OPERATOR___alternative___NUM ) {
                arg = t->children[ 0 ];
                if ( t->children[ 1 ]->type == OPERATOR_doubledot_NUM ) {
                    arg_def_vals    << t->children[ 1 ]->children[ 0 ];
                    arg_constraints << t->children[ 1 ]->children[ 1 ];
                } else {
                    arg_def_vals    << t->children[ 1 ];
                    arg_constraints << 0;
                }
            } else if ( t->type == OPERATOR_doubledot_NUM ) {
                arg = t->children[ 0 ];
                if ( t->children[ 1 ]->type == OPERATOR___alternative___NUM ) {
                    arg_def_vals    << t->children[ 1 ]->children[ 1 ];
                    arg_constraints << t->children[ 1 ]->children[ 0 ];
                } else {
                    arg_def_vals    << 0;
                    arg_constraints << t->children[ 1 ];
                }
            } else {
                arg_def_vals    << 0;
                arg_constraints << 0;
            }

            if ( arg->type == OPERATOR_triple_dots_NUM ) {
                arg_spreads << arg_names.size();
                arg = arg->children[ 0 ];
            }

            if ( arg->type != Lexem::VARIABLE )
                return add_error( "expected here an argument name.", arg );
            arg_names.push_back( arg->beg, arg->len );
            args.push_back( arg );
        }
    } else {
        name = c;
    }

    // default_pertinence
    if ( pertinence ) {
        if ( pertinence->type == Lexem::NUMBER ) {
            pertinence->to_num_exp( default_pertinence_num, default_pertinence_exp );
            pertinence = 0;
        } else if ( pertinence->type == OPERATOR_sub_NUM and pertinence->children[ 1 ] == 0 and pertinence->children[ 0 ]->type == Lexem::NUMBER ) {
            pertinence->children[0]->to_num_exp( default_pertinence_num, default_pertinence_exp );
            default_pertinence_num *= -1;
            pertinence = 0;
        } else {
            default_pertinence_num = 0;
            default_pertinence_exp = 0;
        }
    } else {
        default_pertinence_num = condition ? nb_child_and( condition ) : 0;
        for( size_t i = 0; i < arg_constraints.size(); ++i )
            default_pertinence_num += bool( arg_constraints[ i ] );
        default_pertinence_num += is_a_method( lex );
        default_pertinence_exp = 0;
    }

    // self_as_arg
    const Lexem *self_constraint = 0;
    self_as_arg = ( arg_names.size() && arg_names[ 0 ].equal( "self" ) );
    if ( self_as_arg ) {
        self_constraint = *arg_constraints.begin();
        if ( arg_def_vals[ 0 ] )
            return add_error( "'self' as first argument cannot have a default value.", lex );
        args           .erase( 0 );
        arg_names      .erase( 0 );
        arg_def_vals   .erase( 0 );
        arg_constraints.erase( 0 );
        if ( arg_spreads.size() && arg_spreads[ 0 ] == 0 )
            return add_error( "'self' can't be the name of an arg spread.", lex );
        for( size_t &s : arg_spreads )
            --s;
    }

    // basic assertions
    for( size_t i = 0; i < arg_names.size(); ++i ) {
        if ( arg_names[ i ].equal( "self" ) )
            add_error( "'self' as non first argument name of a def is not legal.", lex );
    }

    // abstract
    //    if( block->type == OPERATOR___abstract___NUM ) {
    //        block = block->children[ 0 ];
    //        abstract = true;
    //    }

    // wpc
    while ( block->type == Lexem::BLOCK && block->next == 0 )
        block = block->children[ 0 ];
    if ( block->type == OPERATOR___wpc___NUM ) {
        wpc = make_lex_vec_from( block->children[ 0 ] );
        block = block->next;
    } else if ( def && name->eq( "construct" ) ) {
        const Lexem *last_init_of = 0;
        for( const Lexem *l = block; l; l = l->next )
            if ( l->has_child_type( OPERATOR___init_of___NUM ) )
                last_init_of = l;
        if ( last_init_of ) {
            wpc = make_lex_vec_from( block, last_init_of->next );
            block = last_init_of->next;
            const_cast<Lexem *>( last_init_of )->next = 0;
            if ( block )
                const_cast<Lexem *>( block )->prev = 0;
        }
    }

    // convert method
    if ( name->eq( "convert" ) ) {
        if ( args.size() != 1 )
            add_error( "a 'converter' method needs exactly 1 argument", lex );
        else if ( arg_constraints[ 0 ] )
            named_converter = true;
        else if ( arg_def_vals[ 0 ] )
            add_error( "a 'converter' method must contain a constraint as first argument (not a variable with a default value)", lex );
        else {
            arg_constraints[ 0 ] = args[ 0 ];
            arg_names[ 0 ] = Hpipe::CmString();
            args[ 0 ] = 0;
        }
    }

    // name
    int nb_scope_rec = 0;
    for( ; name->type == OPERATOR_pointer_on_NUM; name = name->children[ 0 ] )
        ++nb_scope_rec;
    bool var_name = name->type == Lexem::VARIABLE;

    // hum
    std::function<void(Lexem *)> repl_var_ass = [&]( Lexem *l ) {
        if ( l->type == OPERATOR___variable_assembly___NUM ) {
            l->type = OPERATOR___string_assembly___NUM;
            repl_var_ass( l->children[ 0 ] );
            repl_var_ass( l->children[ 1 ] );
        }
    };
    repl_var_ass( const_cast<Lexem *>( name ) );

    // output --------------------------------------------------------------
    if ( def ) {
        aw.write_def(
            lex,
            name->beg,
            var_name ? name->len : 0,
            var_name ? 0 : name,
            nb_scope_rec,
            arg_names,
            arg_constraints,
            arg_def_vals,
            arg_spreads,
            with_names,
            with_constraints,
            with_def_vals,
            with_spreads,
            condition,
            pertinence,
            default_pertinence_num,
            default_pertinence_exp,
            CALLABLE_FLAG_self_as_arg     * self_as_arg     +
            CALLABLE_FLAG_static          * static_inst     +
            CALLABLE_FLAG_virtual         * virtual_inst    +
            CALLABLE_FLAG_abstract        * abstract_inst   +
            CALLABLE_FLAG_override        * override_inst   +
            CALLABLE_FLAG_inline          * inline_inst     +
            CALLABLE_FLAG_export          * export_inst     +
            CALLABLE_FLAG_named_converter * named_converter +
            CALLABLE_FLAG_private         * private_inst    +
            CALLABLE_FLAG_protected       * protected_inst  +
            CALLABLE_FLAG_global          * global_inst ,
            make_lex_vec_from( block ),
            self_constraint,
            return_type,
            wpc
        );
    } else {
        this->inheritance_names[ lex ] = inheritance_names;

        aw.write_class(
            lex,
            name->beg,
            var_name ? name->len : 0,
            var_name ? 0 : name,
            nb_scope_rec,
            arg_names,
            arg_constraints,
            arg_def_vals,
            arg_spreads,
            with_names,
            with_constraints,
            with_def_vals,
            with_spreads,
            condition,
            pertinence,
            default_pertinence_num,
            default_pertinence_exp,
            CALLABLE_FLAG_self_as_arg * self_as_arg    +
            CALLABLE_FLAG_static      * static_inst    +
            CALLABLE_FLAG_virtual     * virtual_inst   +
            CALLABLE_FLAG_abstract    * abstract_inst  +
            CALLABLE_FLAG_override    * override_inst  +
            CALLABLE_FLAG_inline      * inline_inst    +
            CALLABLE_FLAG_export      * export_inst    +
            CALLABLE_FLAG_private     * private_inst   +
            CALLABLE_FLAG_protected   * protected_inst +
            CALLABLE_FLAG_global      * global_inst ,
            make_lex_vec_from( block ),
            inheritance_names,
            inheritance
        );
    }
}

void Ast_maker::make_ast_enum( const Lexem *lex ) {
    if ( lex->children[ 0 ]->type != Lexem::VARIABLE )
        return add_error( "enum name must be a variable name", lex );
    const Lexem *name = lex->children[ 0 ];

    Vec<const Lexem *> items;
    if ( lex->children[ 1 ]->type == Lexem::BLOCK ) {
        for( const Lexem *i = lex->children[ 1 ]->children[ 0 ]; i; i = i->next )
            items << i;
    } else
        get_children_of_type( lex->children[ 1 ], OPERATOR_comma_NUM, items );

    Vec<Hpipe::CmString> item_names;
    for( const Lexem *item : items )
        item_names.push_back( item->beg, item->len );

    aw.write_enum( lex, name->beg, name->len, item_names );
}

void Ast_maker::make_ast_call_op( const Lexem *lex ) {
    Vec<const Lexem *> args;
    for( int i = 0; i < 2; ++i )
        if ( lex->children[ i ] )
            args << lex->children[ i ];
    std::string name = operators_met_name( lex->type );
    if ( name.empty() )
        name = operators_cpp_name( lex->type );
    name = std::string( "operator " ) + name;
    aw.write_apply_op( lex, name.data(), name.size(), args );
}

void Ast_maker::make_ast_pass( const Lexem *lex ) {
    aw.write_pass();
}

void Ast_maker::make_ast_if( const Lexem *lex ) {
    if ( lex->children[ 1 ]->type == OPERATOR___else___NUM )
        return aw.write_if_else( lex->children[ 0 ], make_lex_vec_from( lex->children[ 1 ]->children[ 0 ] ), make_lex_vec_from( lex->children[ 1 ]->children[ 1 ] ) );
    aw.write_if( lex->children[ 0 ], make_lex_vec_from( lex->children[ 1 ] ) );
}

void Ast_maker::make_ast_try( const Lexem *lex ) {
    if ( lex->children[ 1 ]->type != OPERATOR___catch___NUM )
        return add_error( "a try must be followed by a catch", lex );
    const Lexem *arg_name = 0;
    if ( lex->children[ 1 ]->children[ 0 ]->type != Lexem::VARIABLE )
        TODO;
    arg_name = lex->children[ 1 ]->children[ 0 ];

    aw.write_try_catch( lex, lex->children[ 0 ], arg_name->beg, arg_name->len, 0, lex->children[ 1 ]->children[ 1 ], {}, {}, {}, {} );
}

void Ast_maker::make_ast_while( const Lexem *lex ) {
    aw.write_while( lex->children[ 0 ], make_lex_vec_from( lex->children[ 1 ] ) );
}

void Ast_maker::make_ast_static( const Lexem *lex ) {
    auto r = raii_save( static_inst, true );
    make_single( lex->children[ 0 ] );
}

void Ast_maker::make_ast_mixin( const Lexem *lex ) {
    auto r = raii_save( mixin_inst, true );
    make_single( lex->children[ 0 ] );
}

void Ast_maker::make_ast_const( const Lexem *lex ) {
    auto r = raii_save( const_inst, true );
    make_single( lex->children[ 0 ] );
}

void Ast_maker::make_ast_export( const Lexem *lex ) {
    auto r = raii_save( export_inst, true );
    make_single( lex->children[ 0 ] );
}

void Ast_maker::make_ast_global( const Lexem *lex ) {
    auto r = raii_save( global_inst, true );
    make_single( lex->children[ 0 ] );
}

void Ast_maker::make_ast_virtual( const Lexem *lex ) {
    auto r = raii_save( virtual_inst, true );
    make_single( lex->children[ 0 ] );
}
void Ast_maker::make_ast_override( const Lexem *lex ) {
    auto r = raii_save( override_inst, true );
    make_single( lex->children[ 0 ] );
}
void Ast_maker::make_ast_inline( const Lexem *lex ) {
    auto r = raii_save( inline_inst, true );
    make_single( lex->children[ 0 ] );
}
void Ast_maker::make_ast_abstract( const Lexem *lex ) {
    auto r = raii_save( abstract_inst, true );
    make_single( lex->children[ 0 ] );
}

void Ast_maker::make_ast_for( const Lexem *lex ) {
    if ( lex->children[ 0 ]->type != OPERATOR_in_NUM )
        return add_error( "syntax of 'for' is: 'for ... in ... ...'.", lex->children[ 0 ] );

    // names
    Vec<const Lexem *> names;
    get_children_of_type( lex->children[ 0 ]->children[ 0 ], OPERATOR_comma_NUM, names );
    if ( names.size() != 1 )
        return add_error( "TODO: for with complex assignation schemes", lex );

    Vec<const Lexem *> ch;
    get_children_of_type( lex->children[ 0 ]->children[ 1 ], OPERATOR_comma_NUM, ch );
    if ( ch.size() != 1 )
        return add_error( "TODO: for with zipped lists", lex );

    //
    aw.write_for( lex, names[ 0 ]->beg, names[ 0 ]->len, ch[ 0 ], make_lex_vec_from( lex->children[ 1 ] ) );
}

const Lexem *rightmost_filename_child( const Lexem *l ) {
    if ( l->type == OPERATOR_div_NUM || l->type == OPERATOR_div_int_NUM )
        return rightmost_filename_child( l->children[ 1 ] );
    if ( l->type == OPERATOR_dot_dot_slash_NUM || l->type == OPERATOR_dot_slash_NUM )
        return rightmost_filename_child( l->children[ 0 ] );
    return l;
}

void Ast_maker::make_ast_import( const Lexem *lex ) {
    Vec<Hpipe::CmString> names;
    Vec<Hpipe::CmString> attrs;
    const Lexem *filename = 0;

    const Lexem *ch = lex->children[ 0 ];
    Vec<const Lexem *> nl;
    get_children_of_type( ch, OPERATOR_comma_NUM, nl );
    if ( nl[ 0 ]->type == OPERATOR_double_arrow_NUM ) {
        filename = nl[ 0 ]->children[ 0 ];
        nl[ 0 ] = nl[ 0 ]->children[ 1 ];
    }

    if ( filename ) {
        for( const Lexem *n : nl ) {
            if ( n->type != Lexem::VARIABLE )
                add_error( "in 'import ... => foo, bar', 'foo' and 'bar' must be variable names", lex );
            names.push_back( n->o_beg, n->o_len );
            attrs.push_back( n->o_beg, n->o_len );
        }
    } else {
        if ( nl.size() != 1 )
            add_error( "import ..., ... without =>", lex );
        filename = nl[ 0 ];
        const Lexem *rc = rightmost_filename_child( filename );
        names.push_back( rc->o_beg, rc->o_len );
        attrs.push_back( rc->o_beg, rc->o_len );
    }

    aw.write_import( lex, filename->o_beg, filename->o_len, names, attrs );
}

//void Ast_maker::make_ast_typeof( const Lexem *lex ) {
//    aw.write_typeof( lex, lex->children[ 0 ] );
//}

//void Ast_maker::make_ast_sizeof( const Lexem *lex ) {
//    aw.write_sizeof( lex, lex->children[ 0 ] );
//}
//void Ast_maker::make_ast_aligof( const Lexem *lex ) {
//    aw.write_aligof( lex, lex->children[ 0 ] );
//}

void Ast_maker::make_ast_lambda( const Lexem *lex ) {
    const Lexem         *body = lex->children[ 1 ];
    Vec<size_t>          spreads;
    Vec<Hpipe::CmString> arg_names;
    Vec<const Lexem *>   arg_def_vals;
    Vec<const Lexem *>   arg_constraints;

    if ( body ) {
        const Lexem *args = lex->children[ 0 ];
        if ( args->type == Lexem::PAREN )
            args = args->children[ 0 ];
        Vec<const Lexem *> ch;
        get_children_of_type( args, OPERATOR_comma_NUM, ch );

        for( size_t i = 0; i < ch.size(); ++i ) {
            const Lexem *t = ch[ i ];
            if ( not t )
                continue;

            // look for a = ... ( default_value )
            if ( t->type == OPERATOR_reassign_NUM ) {
                Lexem *dv = t->children[ 1 ];
                if ( dv->type == OPERATOR_doubledot_NUM )
                    return add_error( "Syntax for argument with type constraint and default value is 'name_arg : type constraint = default_value' (in this specific order).", dv );
                arg_def_vals << dv;
                t = t->children[ 0 ];
            } else
                arg_def_vals << 0;

            // look for a : Toto
            if ( t->type == OPERATOR_doubledot_NUM ) {
                arg_constraints << t->children[ 1 ];
                t = t->children[ 0 ];
            } else
                arg_constraints << 0;

            if ( t->type == OPERATOR_triple_dots_NUM ) {
                spreads << arg_names.size();
                t = t->children[ 0 ];
            }

            // name
            arg_names.push_back( t->beg, t->len );
        }
    } else
        body = lex->children[ 0 ];

    aw.write_lambda( lex, arg_names, arg_constraints, arg_def_vals, spreads, body );
}

void Ast_maker::make_ast_get_attr( const Lexem *lex, bool ptr, bool ask, bool ddo ) {
    ASSERT( ask == false, "..." );

    if ( ! lex->children[ 1 ] )
        add_error( "...", lex );
    if ( lex->children[ 1 ]->type == Lexem::VARIABLE ) {
        if ( ptr )
            aw.write_get_attr_ptr( lex->children[ 0 ], lex->children[ 1 ]->beg, lex->children[ 1 ]->len );
        else if ( ddo )
            aw.write_scope_resolution( lex->children[ 0 ], lex->children[ 1 ]->beg, lex->children[ 1 ]->len );
        else
            aw.write_get_attr( lex->children[ 0 ], lex->children[ 1 ]->beg, lex->children[ 1 ]->len );
    } else if ( lex->children[ 1 ]->type == OPERATOR___variable_assembly___NUM ) {
        Vec<const Lexem *> ch;
        get_children_of_type( lex->children[ 1 ], OPERATOR___variable_assembly___NUM, ch );

        Vec<const Lexem *> items;
        for( const Lexem *c : ch )
            if ( c->type != Lexem::STRING || c->len - bool( c->num & 1 ) - bool( c->num & 2 ) )
                items << c;
        if ( items.empty() )
            add_error( "void var assembly", lex->children[ 1 ] );

        if ( ptr )
            aw.write_get_attr_ptr_calc( lex->children[ 0 ], items );
        else if ( ddo )
            aw.write_scope_resolution_calc( lex->children[ 0 ], items );
        else
            aw.write_get_attr_calc( lex->children[ 0 ], items );
    } else
        add_error( "unhandled lexem type for get_attr", lex->children[ 1 ] );
}

void Ast_maker::make_ast_init_of( const Lexem *lex ) {
    // get nb_..._children and ch list
    Vec<const Lexem *> ch;
    get_children_of_type( next_if_CR( lex->children[ 0 ] ), OPERATOR_comma_NUM, ch );
    if ( ch.empty() || ch[ 0 ]->type != Lexem::VARIABLE )
        return add_error( "first argument of an 'init_of' must be a variable name", ch.empty() ? lex : ch[ 0 ] );
    const Lexem *f = ch[ 0 ];
    ch.remove( 0 );

    // spreads
    Vec<size_t> spreads;
    for( size_t i = 0; i < ch.size(); ++i ) {
        if ( ch[ i ]->type == OPERATOR_triple_dots_NUM ) {
            ch[ i ] = ch[ i ]->children[ 0 ];
            spreads << i;
        }
    }
    // get nb_unnamed_children
    size_t nb_unnamed_children = 0, nb_named_children = 0;
    for( ; nb_unnamed_children < ch.size() and ch[ nb_unnamed_children ]->type != OPERATOR_reassign_NUM; ++nb_unnamed_children );
    for( size_t i = nb_unnamed_children; i < ch.size(); ++i, ++nb_named_children ) {
        if ( ch[ i ]->type != OPERATOR_reassign_NUM )
            return add_error( "after a named argument, all arguments must be named", ch[ i ] );
    }

    // unnamed arguments
    Vec<const Lexem *> args;
    Vec<Hpipe::CmString> names;
    for( size_t i = 0; i < nb_unnamed_children; ++i )
        args.push_back( ch[ i ] );

    // named arguments
    for( size_t i = 0; i < nb_named_children; ++i ) {
        const Lexem *n = ch[ nb_unnamed_children + i ]->children[ 0 ];
        names.push_back( n->beg, n->len );
        args.push_back( ch[ nb_unnamed_children + i ]->children[ 1 ] );
    }

    aw.write_init_of( lex, f->beg, f->len, args, names, spreads );
}

void Ast_maker::make_ast_and( const Lexem *lex ) {
    return aw.write_and( lex, lex->children[ 0 ], lex->children[ 1 ] );
}

void Ast_maker::make_ast_or( const Lexem *lex ) {
    return aw.write_or( lex, lex->children[ 0 ], lex->children[ 1 ] );
}


void Ast_maker::make_ast_alternat( const Lexem *lex ) {
    if ( lex->children[ 1 ]->type != OPERATOR_doubledot_NUM ) {
        if ( lex->children[ 1 ]->type > OPERATOR_doubledot_NUM )
            return add_error( "'?' is expected to be followed by a ':' (sub-expression seems to have an higher precedence operator", lex->children[ 1 ] );
        return add_error( "'?' is expected to be followed by a ':'", lex->children[ 1 ] );
    }
    aw.write_if_else( lex->children[ 0 ], lex->children[ 1 ]->children[ 0 ], lex->children[ 1 ]->children[ 1 ] );
}

void Ast_maker::make_var_assembly( const Lexem *lex ) {
    Vec<const Lexem *> ch;
    get_children_of_type( lex, OPERATOR___variable_assembly___NUM, ch );

    Vec<const Lexem *> items;
    for( const Lexem *c : ch )
        if ( c->type != Lexem::STRING || c->len - bool( c->num & 1 ) - bool( c->num & 2 ) )
            items << c;
    if ( items.empty() )
        add_error( "void var assembly", lex );
    aw.write_var_assembly( lex, items );
}

//void Ast_maker::make_ast_new( const Lexem *lex ) {
//    aw.write_new( lex, lex->children[ 0 ], {}, {} );
//}

void Ast_maker::make_ast_apply( const Lexem *lex, const std::function<void (const Lexem *, const Vec<const Lexem *> &, const Vec<Hpipe::CmString> &, const Vec<size_t> &)> &cb ) {
    // private/protected
    if ( lex->children[ 0 ]->eq( "private" ) ) {
        auto r = raii_save( private_inst, true );
        return lex->children[ 1 ]->type == Lexem::BLOCK ? aw.write_ss_block( make_lex_vec_from( lex->children[ 1 ]->children[ 0 ] ), PRIVATE ) : make_single( lex->children[ 1 ] );
    }
    if ( lex->children[ 0 ]->eq( "protected" ) ) {
        auto r = raii_save( protected_inst, true );
        return lex->children[ 1 ]->type == Lexem::BLOCK ? aw.write_ss_block( make_lex_vec_from( lex->children[ 1 ]->children[ 0 ] ), PROTECTED ) : make_single( lex->children[ 1 ] );
    }


    // get nb_..._children and ch list
    Vec<size_t> spreads;
    Vec<const Lexem *> ch;
    size_t nb_unnamed_children = 0, nb_named_children = 0;
    if ( lex->children[ 1 ] ) {
        if ( lex->children[ 1 ]->type == Lexem::BLOCK ) {
            for( const Lexem *a = lex->children[ 1 ]->children[ 0 ]; a; a = a->next )
                ch << a;
        } else
            get_children_of_type( lex->children[ 1 ], OPERATOR_comma_NUM, ch );
        // spreads
        for( size_t i = 0; i < ch.size(); ++i ) {
            if ( ch[ i ]->type == OPERATOR_triple_dots_NUM ) {
                ch[ i ] = ch[ i ]->children[ 0 ];
                spreads << i;
            }
        }
        // get nb_unnamed_children
        for( ; nb_unnamed_children < ch.size() and ch[ nb_unnamed_children ]->type != OPERATOR_doubledot_NUM; ++nb_unnamed_children );
        for( size_t i = nb_unnamed_children; i < ch.size(); ++i, ++nb_named_children ) {
            if ( ch[ i ]->type != OPERATOR_doubledot_NUM )
                return add_error( "after a named argument, all arguments must be named", ch[ i ] );
        }
    }

    // particular case : assertion
    const Lexem *f = lex->children[ 0 ] ? lex->children[ 0 ] : lex;
    if ( nb_unnamed_children == 2 and nb_named_children == 0 and f->type == Lexem::VARIABLE and f->len == 6 and strncmp( f->beg, "assert", 6 ) == 0 ) {
        TODO;
        // -> make an if
    }

    // break n
    if ( f->eq( "break" ) ) {
        TODO;
        //        if ( ch.size() > 1 )
        //            return add_error( "break accepts at most 1 parameter", f );

        //        delete res;

        //        Ast_Break *nres = new Ast_Break( f->src, f->off() );
        //        if ( ch.size() == 1 ) {
        //            if ( not ch[ 0 ]->is_an_int() )
        //                return add_error( "break expects a known integer value", f );
        //            nres->n = ch[ 0 ]->to_int();
        //        }
        //        return nres;
    }

    if ( f->eq( "info" ) ) {
        Vec<const Lexem *> code;
        Vec<Hpipe::CmString> str;
        for( size_t i = 0; i < ch.size(); ++i ) {
            str << Hpipe::CmString( ch[ i ]->o_beg, ch[ i ]->o_len );
            code << ch[ i ];
        }
        return aw.write_info( lex, str, code );
    }

    if ( f->eq( "assert" ) ) {
        if ( ch.size() > 1 )
            return add_error( "assert expects exactly one argument", ch[ 1 ] );
        return aw.write_assert( lex, ch[ 0 ]->o_beg, ch[ 0 ]->o_len, ch[ 0 ] );
    }

    if ( f->eq( "return" ) ) {
        if ( ch.size() != 1 )
            TODO;
        return aw.write_return( lex, ch[ 0 ] );
    }

    // unnamed arguments
    Vec<const Lexem *> args;
    Vec<Hpipe::CmString> names;
    for( size_t i = 0; i < nb_unnamed_children; ++i )
        args.push_back( ch[ i ] );

    // named arguments
    for( size_t i = 0; i < nb_named_children; ++i ) {
        const Lexem *n = ch[ nb_unnamed_children + i ]->children[ 0 ];
        names.push_back( n->beg, n->beg + n->len );
        args.push_back( ch[ nb_unnamed_children + i ]->children[ 1 ] );
    }

    // typeof
    if ( f->eq( "typeof" ) ) {
        if ( nb_named_children )
            return add_error( "'typeof' expects exactly 1 unnamed argument", lex );
        if ( args.size() != 1 )
            return add_error( "'typeof' expects exactly 1 argument", lex );
        return aw.write_typeof( lex, args[ 0 ] );
    }

    // sizeof
    if ( f->eq( "sizeof" ) ) {
        if ( nb_named_children )
            return add_error( "'sizeof' expects exactly 1 unnamed argument", lex );
        if ( args.size() != 1 )
            return add_error( "'sizeof' expects exactly 1 argument", lex );
        return aw.write_sizeof( lex, args[ 0 ] );
    }

    // aligof
    if ( f->eq( "aligof" ) ) {
        if ( nb_named_children )
            return add_error( "'aligof' expects exactly 1 unnamed argument", lex );
        if ( args.size() != 1 )
            return add_error( "'aligof' expects exactly 1 argument", lex );
        return aw.write_aligof( lex, args[ 0 ] );
    }

    // sizeof_bits
    if ( f->eq( "sizeof_bits" ) ) {
        if ( nb_named_children )
            return add_error( "'sizeof_bits' expects exactly 1 unnamed argument", lex );
        if ( args.size() != 1 )
            return add_error( "'sizeof_bits' expects exactly 1 argument", lex );
        return aw.write_sizeof_bits( lex, args[ 0 ] );
    }

    // aligof_bits
    if ( f->eq( "aligof_bits" ) ) {
        if ( nb_named_children )
            return add_error( "'aligof_bits' expects exactly 1 unnamed argument", lex );
        if ( args.size() != 1 )
            return add_error( "'aligof_bits' expects exactly 1 argument", lex );
        return aw.write_aligof_bits( lex, args[ 0 ] );
    }

    // raii
    if ( f->eq( "raii" ) ) {
        if ( nb_named_children )
            return add_error( "the goal of raii is to avoid naming variable. Named args are not taken into account.", lex );
        return aw.write_raii( lex, args );
    }

    cb( f, args, names, spreads );
}

void Ast_maker::make_ast_block( const Vec<const Lexem *> &vl ) {
    aw.write_block( vl );
}

