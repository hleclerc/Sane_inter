#include "Helpers/Char_type.h"
#include "Lexer.h"
#include <stack>
#include <set>

inline bool is_num     ( char c ) { return char_type( c ) == CHAR_TYPE_number  ; }
inline bool is_letter  ( char c ) { return char_type( c ) == CHAR_TYPE_letter  ; }
inline bool is_operator( char c ) { return char_type( c ) == CHAR_TYPE_operator; }

Lexer::Lexer( Error_list *error_list ) : error_list( error_list ) {
    first_tok.parent_paren = 0;
    first_tok.type         = Lexem::CR;
    first_tok.num          = 0;
    end = &first_tok;
}

void Lexer::parse( const char *beg, const char *src ) {
    approx_line = 0;
    spcr = 0;
    this->beg = beg;
    this->src = src ? src : "inline";
    if ( not beg )
        return;

    //
    for( int i = 0; i < Lexem::NB_BASE_TYPE + NB_GROUPS; ++i ) {
        first_of_group[ i ] = 0;
        last_of_group [ i ] = 0;
    }

    // limit function
    int lif = GROUP_comma_NUM; // beg first phase
    int enf = GROUP_get_attr_ptr_NUM - 1; // end

    // let's go
    s = beg;
    const Lexem *ft = end;
    make_flat_token_list();                      if ( *error_list ) return;
    keyword_operator_assembly();                 if ( *error_list ) return;
    not_in_replacement();                        if ( *error_list ) return;
    make_sibling_lists();                        if ( *error_list ) return;
    parenthesis_hierarchisation( ft );           if ( *error_list ) return;
    make_unit_hierarchy();                       if ( *error_list ) return;
    assemble___str_and_var_assembly__();         if ( *error_list ) return;
    remove_cr();                                 if ( *error_list ) return;
    assemble_par_and_getattr( ft );              if ( *error_list ) return;
    assemble_op( lif, enf );                     if ( *error_list ) return;
    remove_semicolons();                         if ( *error_list ) return;
    make_calls_without_par( first_tok.next, 0 ); if ( *error_list ) return;
    assemble_op( 0, lif - 1 );                   if ( *error_list ) return;
    make_calls_without_par( first_tok.next, 1 ); if ( *error_list ) return;
}

void Lexer::display() {
    if ( first_tok.next )
        display_graph( first_tok.next, "tmp/res.dot", &first_tok );
}

Lexem *Lexer::root() {
    if ( *error_list )
        return 0;
    return first_tok.next;
}

Lexem *Lexer::new_tok( int type, const char *beg, unsigned len, int num ) {
    lexems.emplace_back();

    Lexem *t = &lexems.back();
    t->beg_src = this->beg;
    t->type    = type;
    t->src     = src;
    t->o_beg   = beg;
    t->o_len   = len;
    t->beg     = beg;
    t->len     = len;
    t->num     = num;

    t->approx_line = approx_line;
    t->spcr        = spcr;
    return t;
}

Lexem *Lexer::app_tok( int type, const char *beg, unsigned len, int num ) {
    Lexem *t = new_tok( type, beg, len, num );

    // ordered
    end->next = t;
    t->prev = end;
    t->next = 0;
    end = t;
    return t;
}

void Lexer::write_error( const std::string &msg, const char *beg, const char *pos, const std::string &src, bool due_to_not_ended ) {
    Error_list::Error &error = error_list->add( msg, false, due_to_not_ended );
    error.ac( beg, pos, src );
    std::cerr << error;
}

void Lexer::read_dollar( const char *old_str, int num_operator ) {
    // operator __string_assembly__
    app_tok( num_operator, s, 1, operators_behavior( num_operator ) );

    // tokens
    const char *e = ++s;
    if ( e[ 0 ] == '(' or e[ 0 ] == '[' or e[ 0 ] == '{' ) { // $(...)
        unsigned cpt = 1;
        while( (++e)[ 0 ] ) {
            if ( e[ 0 ]==')' or e[ 0 ]==']' or e[ 0 ]=='}' ) {
                if ( --cpt == 0 ) {
                    ++e;
                    break;
                }
            }
            else if ( e[ 0 ] == '(' or e[ 0 ] == '[' or e[ 0 ] == '{' )
                ++cpt;
        }
        if ( cpt ) {
            write_error( "a ')' is missing, ']' or '}'", beg, s, src );
            return;
        }
    }
    else { // $var
        while ( (++e)[ 0 ] and ( char_type( e[ 0 ] ) == CHAR_TYPE_letter or char_type( e[ 0 ] ) == CHAR_TYPE_number ) ) ;
    }
    read_s<true>( e );

    // operator __string_assembly__
    app_tok( num_operator, s, 1, operators_behavior( num_operator ) );
}


template<bool stop_condition> void Lexer::read_s( const char *limit ) {
    while ( stop_condition == false or s < limit ) {
        switch ( char_type( *s ) ) {
            case CHAR_TYPE_space:
                ++s;
                break;
            case CHAR_TYPE_letter: {
                bool assembly = false;
                const char *old_str = s;
                for( ; is_num( *s ) || is_letter( *s ) || *s == '$'; ++s ) {
                    if ( s[ 0 ] == '$' && ( char_type( s[ 1 ] ) == CHAR_TYPE_letter || char_type( s[ 1 ] ) == CHAR_TYPE_parenthesis ) ) {
                        app_tok( Lexem::STRING, old_str, s - old_str, 0 );
                        read_dollar( old_str, OPERATOR___variable_assembly___NUM );
                        assembly = true;
                        old_str = s--;
                    }
                }
                if ( assembly ) {
                    app_tok( Lexem::STRING, old_str, s - old_str, 0 );
                } else {
                    unsigned si = s - old_str;
                    int num_op = num_operator_le( old_str, si ); // look for an operator
                    app_tok( num_op, old_str, si, operators_behavior( num_op ) );
                }
                break;
            }
            case CHAR_TYPE_operator: {
                const char *old_str = s;
                while ( is_operator( *( ++s ) ) );
                while ( s != old_str ) {
                    for( unsigned i=s-old_str; ; --i ) {
                        if ( not i ) {
                            write_error( "Unknown operator (or operator set) '" + std::string( old_str, s ) + "'.", beg, old_str, src );
                            return;
                        }
                        int num_op = num_operator_op( old_str, i ); // look for an operator
                        if ( num_op >= 0 ) {
                            app_tok( num_op, old_str, i, operators_behavior( num_op ) );
                            old_str += i;
                            /// f <<<< ...
                            if ( num_op == OPERATOR_shift_left_long_str_NUM ) {
                                // get nb_spaces from current line
                                unsigned nb_spaces = 0;
                                const char *b = s;
                                while ( *(--b) and *b!='\n' and *b!='\r' );
                                while ( *(++b)==' ' ) nb_spaces++;

                                // skip spaces after <<<<
                                while ( *old_str == ' ' ) ++old_str;
                                if ( *old_str != '\n' and *old_str != '\r' ) {
                                    write_error( "After a '<<<<' and in the same line, only spaces and carriage returns are allowed.", beg, old_str, src );
                                    return;
                                }
                                old_str += ( *old_str == '\r' );
                                old_str += ( *old_str == '\n' );

                                //
                                s = old_str;
                                while ( true ) {
                                    unsigned new_nb_spaces = 0;
                                    while ( *s==' ' ) { ++s; ++new_nb_spaces; }
                                    if ( new_nb_spaces < nb_spaces + 4 ) {
                                        while ( *s and *s != '\n' and *s != '\r' ) --s; // return to beginning of the line
                                        break;
                                    }
                                    // else
                                    while ( *s and *s!='\n' and *s!='\r' ) ++s; // next line
                                    s += ( *s == '\r' );
                                    s += ( *s == '\n' );
                                }

                                // $...
                                const char *new_s = s;
                                s = old_str;
                                while ( s < new_s ) {
                                    if ( s[ 0 ] == '$' and s[ -1 ] != '\\' and s[ -1 ] != '$' and ( char_type( s[ 1 ] ) == CHAR_TYPE_letter or char_type( s[ 1 ] ) == CHAR_TYPE_parenthesis ) ) {
                                        TODO;
                                        app_tok( Lexem::STRING, old_str, s - old_str, 0 );
                                        read_dollar( old_str ); // nb_spaces + 4
                                        old_str = s - 1;
                                    }
                                    else
                                        s++;
                                }

                                // app_tok
                                TODO;
                                // app_tok( Lexem::STRING, old_str, s-old_str, nb_spaces + 4 );

                                // out loop
                                old_str = s;

                            }
                            break;
                        }
                    }
                }
                break;
            }
            case CHAR_TYPE_cr: {
                ++approx_line;
                spcr = 0;
                unsigned num = 0;
                while ( *( ++s ) ) {
                    if ( s[0] == '\t' ) {
                        write_error( "Tabulations are not allowed at the beginning of a line. Please use spaces only.", beg, s, src );
                        num += 3;
                    } else if ( s[0] != ' ' ) {
                        spcr = num;
                        app_tok( Lexem::CR, s, 0, num );
                        break;
                    }
                    ++num;
                }
                break;
            }
            case CHAR_TYPE_ccode: {
                const char *old_str = s++;
                while ( *s and ( *s != '`' or ( s[ -1 ] == '\\' ) ) ) ++s;
                if ( *s )
                    app_tok( Lexem::CCODE, old_str, ++s - old_str, -1 );
                else
                    write_error( "a closing '`' is missing.", beg, old_str, src, true );
                break;
            }
            case CHAR_TYPE_string: {
                bool assembly = false;
                const char *old_str = s;
                while ( *(++s) and ( *s != '"' or ( s[ -1 ] == '\\' and s[ -2 ] != '\\' ) ) ) {
                    if ( s[ 0 ] == '$' and s[ -1 ] != '$' and s[ -1 ] != '\\' and ( char_type( s[ 1 ] ) == CHAR_TYPE_letter or char_type( s[ 1 ] ) == CHAR_TYPE_parenthesis ) ) {
                        app_tok( Lexem::STRING, old_str, s - old_str, assembly ? 0 : 1 );
                        read_dollar( old_str );
                        assembly = true;
                        old_str = s--;
                    }
                }
                //
                if ( not *s )
                    write_error( "a closing '\"' is missing.", beg, old_str, src, true );
                else {
                    app_tok( Lexem::STRING, old_str, s - old_str + 1, assembly ? 2 : 3 );
                    ++s;
                }
                break;
            }
            case CHAR_TYPE_number: {
                const char *old_str = s;

                // LexemNumber *n = numbers.new_elem(); n->v.init(0);
                if ( s[ 0 ] == '0' and ( s[ 1 ] == 'x' or s[ 1 ] == 'X') ) { // 0xFFE -> hexadecimal
                    for( s += 2; is_num( s[0] == 0 ) or ( s[0]>='a' and s[0]<='f' ) or ( s[0]>='A' and s[0]<='F' ); ++s );
                    app_tok( Lexem::NUMBER, old_str, s - old_str, 0 );
                } else { // decimal number
                    while ( is_num( *( ++s ) ) );
                    if ( s[ 0 ] == '.' and is_num( s[ 1 ] ) )
                        while ( is_num( *( ++s ) ) );
                    if ( s[0] == 'e' or s[0] == 'E' ) { // numbers after [eE]
                        s += ( s[ 1 ] == '-' or s[ 1 ] == '+' );
                        while ( is_num( *( ++s ) ) );
                    }
                    // completion of attributes (unsigned, float, pointer, ...)
                    for( ; s[0] == 'u' or s[0] == 'i' or s[0] == 'p' or s[0] == 'l'; ++s );
                    // else if ( char_type(s[0])==CHAR_TYPE_letter )
                    // error_list->add( "Character '"+std::string(1,*s)+"' is not a known number attribute ('u' for unsigned, 'i' for imaginary ).", s, src );
                    app_tok( Lexem::NUMBER, old_str, s - old_str, 0 );
                }

                break;
            }
            case CHAR_TYPE_parenthesis: {
                if ( s[ 0 ] == '(' and s[ 1 ] == '*' and s[ 2 ] == ')' ) {
                    app_tok( OPERATOR_tensorial_product_NUM, s, 3, operators_behavior( OPERATOR_tensorial_product_NUM ) );
                    s += 3;
                } else // (...
                    app_tok( Lexem::PAREN, s++, 1, 0 );
                break;
            }
            case CHAR_TYPE_comment: {
                const char *old_str = s; // used if error
                if ( s[1] == '#' ) { // ## pouet ##
                    for( s += 2; ; ++s ) {
                        if ( not *s ) {
                            write_error( "Comment is not closed ( Ex: ## toto ## ).", beg, old_str, src, true );
                            return;
                        }
                        if ( s[ -1 ]=='#' and s[0]=='#' ) {
                            ++s;
                            break;
                        }
                    }
                } else if ( s[1] == '~' ) { // #~ pouet ~#
                    for( s += 2; ; ++s ) {
                        if ( not *s ) {
                            write_error( "Comment is not closed ( Ex: #~ toto ~# ).", beg, old_str, src, true );
                            return;
                        }
                        if ( s[ -1 ]=='~' and s[ 0 ]=='#' ) {
                            ++s;
                            break;
                        }
                    }
                } else {
                    while( *( ++s ) and s[ 0 ] != '\n' ) ; // looking for a carriage return
                }
                break;
            }
            case CHAR_TYPE_dos_makes_me_sick:
                ++s;
                break;
            case CHAR_TYPE_end:
                return;
            default:
                // error_list->add("Character is not allowed in this context.", s, src );
                ++s;
        }
    }
}


void Lexer::make_flat_token_list() {
    read_s<false>();
}

void Lexer::keyword_operator_assembly() {
    for( Lexem *o = root(); o; o = o->next ) {
        if ( o->type == OPERATOR___operator___NUM ) {
            if ( o->next == NULL ) {
                write_error( "keyword 'operator' needs a right expression.", beg, o->o_beg, src, is_in_main_block( o ) );
                continue;
            }
            const char *str_lst[] = { "()", "[]", "{}" };
            for( const char *str : str_lst ) {
                if ( o->next->eq( str[ 0 ] ) ) {
                    if ( o->next->next == NULL || o->next->next->beg != o->next->beg + 1 || ! o->next->next->eq( str[ 1 ] ) ) {
                        write_error( "'operator(' must be immediatly followed by a '" + std::to_string( str[ 1 ] ) + "'.", beg, o->o_beg, src, is_in_main_block( o ) );
                        continue;
                    }
                    o->next->len += 1;
                    o->next->o_len += 1;
                    if ( o->next->next->next )
                        o->next->next->next->prev = o->next;
                    o->next->next = o->next->next->next;
                    break;
                }
            }

            // take str data from the next item
            o->o_len = o->next->o_beg + o->next->o_len - o->o_beg;

            // use "operator ..." for { beg, len } of o
            str_pool.emplace_back( "operator " + o->next->str() );
            o->beg = str_pool.back().data();
            o->len = str_pool.back().size();
            o->type = Lexem::VARIABLE;

            // remove the next item
            if ( o->next->next )
                o->next->next->prev = o;
            o->next = o->next->next;
        }
    }
}

static int _not_in_replacement( int type ) {
    switch ( type ) {
    case OPERATOR_in_NUM  : return OPERATOR_not_in_NUM;
    case OPERATOR_is_a_NUM: return OPERATOR_is_not_a_NUM;
    default: return 0;
    }
}

void Lexer::not_in_replacement() {
    for( Lexem *i = root(); i; i = i->next ) {
        if ( i->type == OPERATOR_not_boolean_NUM and i->next ) {
            if ( int nr = _not_in_replacement( i->next->type ) ) {
                i->type = nr;
                i->len = ( i->next->beg - i->beg ) + i->next->len;
                i->num = operators_behavior( nr );
                if ( i->next->next )
                    i->next->next->prev = i;
                i->next = i->next->next;
            }
        }
    }
}

void Lexer::make_sibling_lists() {
    for( Lexem *t = root(); t; t = t->next ) {
        int index = Lexem::NB_BASE_TYPE + ( t->type >= 0 ? group_of_operator( t->type ) : t->type );

        if ( t->type >= 0 && ( t->type >= 0 && right_to_left_associativity( t->type ) ) ) {
            t->sibling = first_of_group[ index ];
            first_of_group[ index ] = t;
        } else {
            if ( last_of_group[ index ] == NULL ) {
                first_of_group[ index ] = t;
                last_of_group [ index ] = t;
            } else {
                last_of_group [ index ]->sibling = t;
                last_of_group [ index ] = t;
            }
        }
    }
}

void Lexer::parenthesis_hierarchisation( const Lexem *ft ) {
    Vec<Lexem *> ps_stack;
    for( Lexem *i = ft->next; i; i = i->next ) {
        i->parent_paren = ps_stack.empty() ? 0 : ps_stack.back();
        if ( i->type == Lexem::PAREN ) {
            if ( i->beg[ 0 ] == '(' or i->beg[ 0 ] == '[' or i->beg[ 0 ] == '{' ) // opening
                ps_stack.push_back( i );
            else { // closing parenthesis
                if ( ps_stack.size() == 0 or ps_stack.back()->beg[ 0 ] != ( i->beg[ 0 ]==')' ? '(' : ( i->beg[ 0 ] == ']' ? '[' : '{' ) ) ) {
                    write_error( "Closing '" + std::string( 1, i->beg[ 0 ] )+"' has no correspondance.", beg, i->o_beg, src );
                    return;
                }
                Lexem *o = ps_stack.back(); // assumed opening correspondance

                o->o_len += ( i->o_beg - o->o_beg );

                ps_stack.pop_back();
                if ( i->prev == o ) { // [nothing]
                    o->next = i->next;
                    if ( i->next ) i->next->prev = o;
                } else { // [something ...]
                    if ( o->next ) {
                        o->next->parent = o;
                        o->next->prev = NULL;
                    }
                    o->children[ 0 ] = o->next;
                    o->next = i->next;

                    if ( i->next ) i->next->prev = o;
                    if ( i->prev ) i->prev->next = NULL;
                }

            }
        }
        //else if ( i->type==Lexem::CR and ps_stack.size() )
        //    i->num = -1; // for [ x y \n z a ] ?
    }

    for( size_t i = 0; i < ps_stack.size(); ++i )
        write_error( "Opening '" + std::string( 1, ps_stack[i]->beg[ 0 ] ) + "' has no correspondance.", beg, ps_stack[i]->beg, src, true );
}

void Lexer::make_unit_hierarchy() {
    for( Lexem *t = first_of_group[ Lexem::NUMBER + Lexem::NB_BASE_TYPE ]; t; t = t->sibling )
        if ( t->next and t->next->type == Lexem::STRING )
            assemble_right_arg( t );
}

void Lexer::remove_cr() {
    //    // remove CR at beginnings
    //    for( Lexem *t = first_of_group[ Lexem::CR + Lexem::NB_BASE_TYPE ]; t; t = t->sibling )
    //        if ( not t->prev )
    //            t->remove();

    // update $preceded_by_a_cr
    for( Lexem *t = first_of_group[ Lexem::CR + Lexem::NB_BASE_TYPE ]; t; t = t->sibling ) {
        if ( t->next ) {
            t->next->nb_preceding_cr += t->nb_preceding_cr + 1;
            t->next->preceded_by_a_cr = true;
        }
    }

    // remove CR for void lines
    for( Lexem *t = first_of_group[ Lexem::CR + Lexem::NB_BASE_TYPE ]; t; t = t->sibling ) {
        if ( t->next == 0 || t->next->type == Lexem::CR ) {
            t->num = -2;
            t->remove();
        }
    }

    // then, we remove \n in a+\nb or a\n+b
    for( Lexem *t = first_of_group[ Lexem::CR + Lexem::NB_BASE_TYPE ]; t; t = t->sibling ) {
        if ( t->num < 0 )
            continue;
        bool rem = (
            need_rarg( t->prev ) and t->prev->children[ 0 ] == NULL &&
            t->prev->type != OPERATOR___def___NUM &&
            t->prev->type != OPERATOR___class___NUM &&
            t->prev->type != OPERATOR___if___NUM &&
            t->prev->type != OPERATOR___else___NUM &&
            t->prev->type != OPERATOR___while___NUM &&
            t->prev->type != OPERATOR___for___NUM &&
            t->prev->type != OPERATOR___try___NUM &&
            t->prev->type != OPERATOR_double_arrow_NUM &&
            t->prev->type != OPERATOR___wpc___NUM
        ) || (
            need_larg( t->next ) &&
            t->next->children[ 0 ] == NULL &&
            t->next->type != OPERATOR___else___NUM
        //        ) || (
        //            t->parent && (
        //                t->parent->type == Lexem::PAREN ||
        //                t->parent->type == Lexem::APPLY ||
        //                t->parent->type == Lexem::SELECT ||
        //                t->parent->type == Lexem::CHBEBA
        //            )
        );
        if ( rem ) {
            t->remove();
            t->num = -2;
        }
    }

    // after what, we can make blocks
    std::stack<Lexem *> pending_blocks;
    pending_blocks.push( &first_tok );
    for( Lexem *t = first_of_group[ Lexem::CR + Lexem::NB_BASE_TYPE ]; t; t = t->sibling ) {
        if ( t->num < 0 )
            continue;

        // we have to make a new block ?
        if ( t->num > pending_blocks.top()->num ) {
            // create a new block
            t->type = Lexem::BLOCK;
            t->children[ 0 ] = t->next;
            if ( t->next ) {
                t->next->parent = t;
                t->next->prev = NULL;
            }
            t->next = NULL;
            pending_blocks.push( t );
            continue;
        }

        // we have to close blocks with higher indentation
        Lexem *ib = pending_blocks.top();
        if ( t->num < ib->num ) {
            // update block span, find corresponding alignment
            while ( t->num < pending_blocks.top()->num ) {
                ib = pending_blocks.top();
                ib->o_len = t->o_beg - ib->o_beg;
                pending_blocks.pop();
            }

            // check alignment
            if ( t->num != pending_blocks.top()->num ) {
                write_error( "columns are not aligned.", beg, t->beg, src );
                break;
            }

            // attach next (if we on the same level of (), [] or {})
            if ( t->parent_paren == ib->parent_paren ) {
                if ( t->prev ) t->prev->next = NULL;
                t->prev = ib;
                ib->next = t;
            }
        }
    }

    // update span of non closed blocks
    for( ; ! pending_blocks.empty(); pending_blocks.pop() ) {
        Lexem *bb = pending_blocks.top();
        // all block should have a children... excepted first_tok
        if ( Lexem *t = bb->children[ 0 ] ) {
            while ( t->next )
                t = t->next;
            bb->o_len = t->o_end() - bb->o_beg;
        }
    }

    // remove the remaining CRs
    for( Lexem *t = first_of_group[ Lexem::CR + Lexem::NB_BASE_TYPE ]; t; t = t->sibling )
        if ( t->type == Lexem::CR )
            t->remove();
}

void Lexer::assemble___str_and_var_assembly__() {
    for( Lexem *o = first_of_group[ GROUP___string_assembly___NUM + Lexem::NB_BASE_TYPE ]; o; o = o->sibling )
        assemble_2_args( o, true, true );
    for( Lexem *o = first_of_group[ GROUP___variable_assembly___NUM + Lexem::NB_BASE_TYPE ]; o; o = o->sibling )
        assemble_2_args( o, true, true );
}

bool elligible_token_for_par( const Lexem *pr ) { // return true if in 'something()', 'something' should become a child of ()
    return ( pr and
            ( pr->type < 0 or
                ( pr->type == Lexem::APPLY or
                  pr->type == Lexem::SELECT  or
                  pr->type == Lexem::CHBEBA  or
                  pr->type == OPERATOR_get_attr_ptr_NUM or
                  pr->type == OPERATOR_get_attr_NUM or
                  pr->type == OPERATOR_get_attr_ptr_ask_NUM or
                  pr->type == OPERATOR_get_attr_ask_NUM or
                  pr->type == OPERATOR___variable_assembly___NUM or
                  pr->type == OPERATOR_doubledoubledot_NUM
                  // pr->type == OPERATOR_calc_name_NUM
                )
            )
            and pr->type != Lexem::CCODE
            and pr->type != Lexem::CR
           );
}

///
void Lexer::assemble_par_and_getattr( const Lexem *ft ) {
    Vec<Lexem *> st;
    st.push_back( ft->next );
    while ( st.size() ) { // while stack is not empty
        Lexem *t = st.back();
        st.pop_back();
        for( ; t; t = t->next ) {
            if ( t->children[ 0 ] )
                st.push_back( t->children[ 0 ] );
            if ( t->children[ 1 ] )
                st.push_back( t->children[ 1 ] );

            // paren must be sticked ('rhs(...)' => OK, 'rhs (...)' => apply done later)
            if ( t->type == Lexem::PAREN && elligible_token_for_par( t->prev ) && t->prev->o_beg + t->prev->o_len == t->o_beg ) {
                Lexem *pr = t->prev, *pa = pr->parent, *pp = pr->prev;

                t->preceded_by_a_cr = pr->preceded_by_a_cr;

                if      ( t->beg[ 0 ] == '(' ) t->type = Lexem::APPLY;
                else if ( t->beg[ 0 ] == '[' ) t->type = Lexem::SELECT;
                else                           t->type = Lexem::CHBEBA;

                t->o_len += t->o_beg - t->prev->o_beg;
                t->o_beg = t->prev->o_beg;
                t->num = 0; // to say that it was an intentionnal (), [] or {}

                if ( pa ) {
                    if ( pa->children[ 0 ] == pr )
                        pa->children[ 0 ] = t;
                    else
                        pa->children[ 1 ] = t;
                    t->parent = pa;
                    pr->parent = NULL;
                }
                if ( pp )
                    pp->next = t;
                t->prev = pp;

                t->children[ 1 ] = t->children[ 0 ];
                t->children[ 0 ] = pr;
                pr->parent = t;
                pr->next = NULL;
                pr->prev = NULL;
            } else if ( ( t->type == OPERATOR_get_attr_NUM     or t->type == OPERATOR_get_attr_ptr_NUM or
                          t->type == OPERATOR_get_attr_ask_NUM or t->type == OPERATOR_get_attr_ptr_ask_NUM or
                          t->type == OPERATOR_doubledoubledot_NUM ) and elligible_token_for_par( t->prev ) ) {
                assemble_2_args( t, true, true );
                if ( t->children[ 1 ] )
                    st.push_back( t->children[ 1 ] );
            }
        }
    }
}

bool Lexer::assemble_2_args( Lexem *o, int need_left, int need_right ) {
    if ( need_right and o->next == NULL ) {
        write_error( "Operator " + std::string( o->beg, o->beg + o->len ) + " needs a right expression.", beg, o->o_beg, src, is_in_main_block( o ) );
        return false;
    }
    if ( need_left and o->prev == NULL ) {
        write_error( "Operator " + std::string( o->beg , o->beg + o->len ) + " needs a left expression.", beg, o->o_beg, src );
        return false;
    }

    // lifting of cr information
    o->preceded_by_a_cr = o->prev->preceded_by_a_cr;

    // prev
    if ( o->prev->parent ) {
        o->parent = o->prev->parent;
        if ( o->prev->parent->children[ 0 ] == o->prev )
            o->prev->parent->children[ 0 ] = o;
        else
            o->prev->parent->children[ 1 ] = o;
    }
    if ( o->prev->prev )
        o->prev->prev->next = o;

    o->nb_preceding_cr       = o->prev->nb_preceding_cr;
    o->prev->nb_preceding_cr = 0;

    o->children[ 0 ] = o->prev;
    o->children[ 0 ]->parent = o;
    o->prev = o->prev->prev;

    o->children[ 0 ]->next = NULL;
    o->children[ 0 ]->prev = NULL;

    // next
    if ( o->next->next )
        o->next->next->prev = o;

    o->children[ 1 ] = o->next;
    o->children[ 1 ]->parent = o;
    o->next = o->next->next;

    o->children[ 1 ]->prev = NULL;
    o->children[ 1 ]->next = NULL;

    // str
    if ( o->children[ 1 ] ) {
        o->o_beg = o->children[ 0 ]->o_beg;
        o->o_len = o->children[ 1 ]->o_beg + o->children[ 1 ]->o_len - o->children[ 0 ]->o_beg;
    } else {
        const char *mi = std::min( o->o_beg  , o->children[ 0 ]->o_beg   );
        const char *ma = std::min( o->o_end(), o->children[ 0 ]->o_end() );
        o->o_beg = mi;
        o->o_len = ma - mi;
    }

    return true;
}

bool Lexer::assemble_right_arg( Lexem *o ) {
    if ( o->next == NULL ) {
        write_error( "Operator " + std::string( o->beg, o->beg + o->len ) + " needs a right expression.", beg, o->o_beg, src, is_in_main_block( o ) );
        return false;
    }
    if ( o->next->next )
        o->next->next->prev = o;

    o->children[ 0 ] = o->next;
    o->children[ 0 ]->parent = o;
    o->next = o->next->next;

    o->children[ 0 ]->prev = NULL;
    o->children[ 0 ]->next = NULL;

    // str
    o->o_len = o->children[ 0 ]->o_beg + o->children[ 0 ]->o_len - o->o_beg;

    return true;
}

bool Lexer::assemble_left_arg(Lexem *o) {
    if ( o->prev == NULL ) {
        write_error( "Operator " + std::string( o->beg, o->beg + o->len ) + " needs a left expression.", beg, o->o_beg, src );
        return false;
    }
    // lifting of cr information
    o->preceded_by_a_cr = o->prev->preceded_by_a_cr;

    if ( o->prev->parent ) {
        o->parent = o->prev->parent;
        if (o->prev->parent->children[0]==o->prev)
            o->prev->parent->children[0] = o;
        else
            o->prev->parent->children[1] = o;
    }
    if ( o->prev->prev )
        o->prev->prev->next = o;

    o->nb_preceding_cr        = o->prev->nb_preceding_cr;
    o->prev->nb_preceding_cr        = 0;

    o->children[ 0 ] = o->prev;
    o->children[ 0 ]->parent = o;
    o->prev = o->prev->prev;

    o->children[ 0 ]->next = NULL;
    o->children[ 0 ]->prev = NULL;

    // str
    o->o_len = o->o_beg + o->o_len - o->children[ 0 ]->o_beg;
    o->o_beg = o->children[ 0 ]->o_beg;

    return true;
}

/// take a,x. Should x be a right value for a tuple ?
bool tok_elligible_maylr( const Lexem *o, const Lexem *orig ) {
    return o and not (
            o->type == Lexem::CR or
            o->type == OPERATOR___pertinence___NUM or
            o->type == OPERATOR_in_NUM or
            o->type == OPERATOR_not_in_NUM or
            o->type == OPERATOR_is_a_NUM or
            o->type == OPERATOR_is_not_a_NUM or
            o->type == OPERATOR_comma_NUM or
            o->type == OPERATOR_semicolon_NUM or

            ( orig->type == OPERATOR_sub_NUM and (
                o->type == OPERATOR_assign_NUM or
                o->type == OPERATOR___alternative___NUM or
                o->type == OPERATOR_doubledot_NUM or
                o->type == OPERATOR_reassign_NUM or
                o->type == OPERATOR_inferior_NUM or
                o->type == OPERATOR_superior_NUM or
                o->type == OPERATOR_inferior_equal_NUM or
                o->type == OPERATOR_superior_equal_NUM or
                o->type == OPERATOR_equal_NUM or
                o->type == OPERATOR_not_equal_NUM or
                o->type == OPERATOR_shift_left_NUM or
                o->type == OPERATOR_shift_left_then_endl_NUM or

                o->type == OPERATOR_self_or_boolean_NUM or
                o->type == OPERATOR_self_xor_boolean_NUM or
                o->type == OPERATOR_self_and_boolean_NUM or
                o->type == OPERATOR_self_or_bitwise_NUM or
                o->type == OPERATOR_self_xor_bitwise_NUM or
                o->type == OPERATOR_self_and_bitwise_NUM or
                o->type == OPERATOR_self_mod_NUM or
                o->type == OPERATOR_self_shift_left_NUM or
                o->type == OPERATOR_self_shift_right_NUM or
                o->type == OPERATOR_self_div_NUM or
                o->type == OPERATOR_self_mul_NUM or
                o->type == OPERATOR_self_sub_NUM or
                o->type == OPERATOR_self_add_NUM or
                o->type == OPERATOR_self_div_int_NUM or
                o->type == OPERATOR_self_concatenate_NUM
            ) )
    );
}

/// transform next and prev to children if necessary
void Lexer::assemble_op( int mi, int ma ) {
    for( int num_group = ma; num_group >= mi; --num_group ) {
        for( Lexem *o = first_of_group[ Lexem::NB_BASE_TYPE + num_group ]; o; o = o->sibling ) {
            int behavior = o->num;
            bool need_left = behavior & 1;
            bool need_right = behavior & 2;
            bool may_need_left = behavior & 4;
            bool may_need_right = behavior & 8;
            bool need_right_right = behavior & 16;


            // both
            if (
                ( need_left  or ( may_need_left  and tok_elligible_maylr( o->prev, o ) ) ) and
                ( need_right or ( may_need_right and tok_elligible_maylr( o->next, o ) ) )
               ) {
                assemble_2_args( o, need_left, need_right );
            }
            // only right
            else if ( need_left == 0 and need_right ) {
                assemble_right_arg( o );
            }
            // only left
            else if ( need_left and need_right == 0 ) {
                assemble_left_arg( o );
            }
            // need two tokens at the right
            else if ( need_right_right ) {
                if ( ! o->next ) {
                    write_error( "Operator needs a right expression.", beg, o->o_beg, src, is_in_main_block( o ) );
                    return;
                }

                if ( o->next->next == 0 || ( o->approx_line != o->next->next->approx_line &&
                                             o->spcr >= o->next->next->spcr &&
                                             ( o->type != OPERATOR___if___NUM || o->next->next->type != OPERATOR___else___NUM ) &&
                                             ( o->type != OPERATOR___try___NUM || o->next->next->type != OPERATOR___catch___NUM ) ) ) {
                    const char *pass = "pass";
                    Lexem *p = new_tok( OPERATOR_pass_NUM, pass, 4, 0 );
                    p->beg_src = pass;
                    p->src = "internal";
                    p->approx_line = o->approx_line;
                    p->spcr = o->spcr;
                    p->beg = o->next->beg;
                    p->len = 0;
                    p->o_beg = o->next->o_beg;
                    p->o_len = 0;

                    Lexem *m = o->next;
                    if ( Lexem *n = o->next->next ) {
                        n->prev = p;
                        p->next = n;
                    }
                    m->next = p;
                    p->prev = m;
                }

                if ( ! o->next->next ) {
                    write_error( "Operator needs TWO right expressions.", beg, o->o_beg, src, is_in_main_block( o ) );
                    return;
                }

                if ( o->next->next->next )
                    o->next->next->next->prev = o;

                o->children[ 0 ] = o->next;
                o->children[ 0 ]->parent = o;

                o->children[ 1 ] = o->next->next;
                o->children[ 1 ]->parent = o;
                o->next = o->next->next->next;

                o->children[ 0 ]->prev = NULL;
                o->children[ 0 ]->next = NULL;
                o->children[ 1 ]->prev = NULL;
                o->children[ 1 ]->next = NULL;

                o->o_len = o->children[ 1 ]->o_beg + o->children[ 1 ]->o_len - o->o_beg;
            }
        }
    }
}

static Lexem *rightmost_child_for_auto_app( Lexem *l ) {
    // no right to steal an attribute, ...
    if ( l->type == OPERATOR_get_attr_ptr_NUM ||
         l->type == OPERATOR_get_attr_NUM ||
         l->type == OPERATOR_get_attr_ptr_ask_NUM ||
         l->type == OPERATOR_get_attr_ask_NUM ||
         l->type == OPERATOR_doubledoubledot_NUM ||
         l->type == OPERATOR_pointed_value_NUM ||
         l->type == OPERATOR_pointer_on_NUM ||
         l->type == OPERATOR___variable_assembly___NUM ||
         l->type == Lexem::PAREN ||
         ( l->type == Lexem::APPLY && l->num == 0 ) ||
         l->type == Lexem::SELECT ||
         l->type == Lexem::CHBEBA )
        return l;
    // no right to steal a child of an if, a class, ...
    // if ( l->type < OPERATOR_comma_NUM )
    //     return l;
    // there's no need_left_left, so a l->children[ 1 ] means a right child
    if ( l->children[ 1 ] )
        return rightmost_child_for_auto_app( l->children[ 1 ] );
    int be = operators_behavior( l->type );
    // if need_right and we have only a child => this child is on the right
    if ( l->children[ 0 ] && ( be & 2 ) )
        return rightmost_child_for_auto_app( l->children[ 0 ] );
    return l;
}

void Lexer::make_calls_without_par( Lexem *n, int phase ) {
    if ( ! n )
        return;

    Lexem *o = n;
    while(( n = n->next )) {
        // it it can be used (at this stage) as an argument of a function call
        if ( n->preceded_by_a_cr == false && ( phase || ( n->type != Lexem::BLOCK && ( n->type < 0 || n->type >= OPERATOR_comma_NUM ) ) ) ) {
            // look it the rightmost item of n->prev can be a function
            Lexem *l = n->prev;
            Lexem *r = rightmost_child_for_auto_app( l );
            if ( r->type != Lexem::BLOCK && ( r->type < 0 || r->type >= OPERATOR_comma_NUM ) && ( r->prev == 0 || r->prev->type != OPERATOR___catch___NUM ) ) {
                // new APPLY Lexem
                Lexem *c = new_tok( Lexem::APPLY, r->o_beg, n->o_beg + n->o_len - r->o_beg, 1 );

                // change successor of l (to be n->next)
                Lexem *p = n->next;
                if ( p )
                    p->prev = l;
                l->next = p;

                // replace r by c in the graph (the new lexem)
                c->preceded_by_a_cr = r->preceded_by_a_cr;
                c->nb_preceding_cr = r->nb_preceding_cr;
                c->approx_line = r->approx_line;
                c->parent = r->parent;
                c->spcr = r->spcr;
                c->prev = r->prev;
                c->next = r->next;
                if ( Lexem *pf = r->parent ) pf->children[ pf->children[ 1 ] == r ] = c;
                if ( Lexem *pr = r->prev ) pr->next = c;
                if ( Lexem *ne = r->next ) ne->prev = c;

                // and set r as a first child of c
                c->children[ 0 ] = r;
                r->parent = c;
                r->prev = 0;
                r->next = 0;

                // set n as second child of c
                c->children[ 1 ] = n;
                n->parent = c;
                n->next = 0;
                n->prev = 0;

                // update span and o (if l went down)
                if ( r != l )
                    for( Lexem *i = c; i != l; i = i->parent )
                        i->parent->o_len = i->o_end() - i->parent->o_beg;
                else if ( o == l )
                    o = c;

                // restart from c it it replaced l
                n = r == l ? c : l;
            }
        }
    }

    // go one level deeper
    for( ; o; o = o->next ) {
        make_calls_without_par( o->children[ 0 ], phase );
        make_calls_without_par( o->children[ 1 ], phase );
    }
}

static bool acts_like_an_if( const Lexem *l ) {
    return ( l->type == OPERATOR___if___NUM or l->type == OPERATOR___while___NUM );
}

typedef Vec<Lexem *> SL;

static void get_if_data( Lexer *le, const char *beg, const char *src, Lexem *t, SL &ch, SL &if_lexems, SL &el_lexems ) {
    // -> OPERATOR___if___NUM, OPERATOR___while___NUM, ...
    if ( acts_like_an_if( t ) ) { // end of recursion
        if_lexems.push_back( t );
        ch.push_back( t->children[ 1 ] );
        // conditions << t->children[ 0 ];
        return;
    }

    // -> OPERATOR___else___NUM
    if ( t->type == OPERATOR___else___NUM ) {
        el_lexems.push_back( t );

        // recursion
        if ( acts_like_an_if( t->children[ 0 ] ) or t->children[ 0 ]->type == OPERATOR___else___NUM )
            get_if_data( le, beg, src, t->children[ 0 ], ch, if_lexems, el_lexems );

        //
        if ( acts_like_an_if( t->children[ 1 ] ) )
            get_if_data( le, beg, src, t->children[ 1 ], ch, if_lexems, el_lexems );
        else
            ch.push_back( t->children[ 1 ] );

        return;
    }

    // -> ??
    le->write_error( "Operator should be an 'else'", beg, t->o_beg, src );
}

void Lexer::remove_semicolons() {
    for( Lexem *l = first_of_group[ Lexem::NB_BASE_TYPE + GROUP_semicolon_NUM ]; l; l = l->sibling ) {
        if ( l->type != OPERATOR_semicolon_NUM )
            continue;
        if ( Lexem *n = l->next )
            n->preceded_by_a_cr = true;
        l->remove();
        //        if ( l->parent or not l->prev or not l->next ) {
        //            write_error( "Misplaced ;", beg, l->o_beg, src );
        //        } else if ( not from_a_bracket_block( l->parent ) ) {
        //            ++l->next->nb_preceding_semicolon;
        //            l->next->prev = l->prev;
        //            l->prev->next = l->next;
        //        }
    }
}

void Lexer::update_else_order() {
    for( Lexem *lex_else = first_of_group[ Lexem::NB_BASE_TYPE + GROUP___else___NUM ]; lex_else; lex_else = lex_else->sibling ) {
        if ( lex_else->type != OPERATOR___else___NUM )
            continue;
        if ( lex_else->parent and lex_else->parent->type == OPERATOR___else___NUM )
            continue;

        SL ch, if_lexems, el_lexems;
        get_if_data( this, beg, src, lex_else, ch, if_lexems, el_lexems );
        Lexem *ple = lex_else->parent;

        for( size_t i = 0; i < if_lexems.size(); ++i ) {
            if ( i < el_lexems.size() ) {
                Lexem *el = el_lexems[ i ], *il = if_lexems[ i ];
                il->children[ 1 ] = el;
                el->parent = il;

                el->children[ 0 ] = ch[ i ];
                ch[ i ]->parent = el;

                if ( i + 1 < if_lexems.size() ) {
                    el->children[ 1 ] = if_lexems[ i + 1 ];
                    if_lexems[ i + 1 ]->parent = el;
                } else if ( i + 1 < ch.size() ) {
                    el->children[ 1 ] = ch[ i + 1 ];
                    ch[ i + 1 ]->parent = el;
                }
            }
        }

        if ( if_lexems.size() == 0 ) {
            write_error( "lonely else", beg, lex_else->o_beg, src );
            continue;
        }

        Lexem *lex_if = if_lexems[ 0 ];
        lex_if->prev = lex_else->prev; if ( lex_else->prev ) lex_else->prev->next = lex_if; lex_else->prev = 0;
        lex_if->next = lex_else->next; if ( lex_else->next ) lex_else->next->prev = lex_if; lex_else->next = 0;
        lex_if->parent = ple; if ( ple ) ple->children[ ple->children[ 1 ] == lex_else ] = lex_if;
    }
}

