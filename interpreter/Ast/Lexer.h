#pragma once

#include "Helpers/(GenOperatorsInfo.ts).h"
#include "../System/Error_list.h"
#include "Lexem.h"

/**
  Make lexem trees from source files
*/
class Lexer {
public:
    using StrPool = std::deque<std::string>;
    using LexPool = std::deque<Lexem>;

    Lexer( Error_list *error_list );

    void        parse                        ( const char *beg, const char *src ); ///< add content to the Lexem tree
    void        display                      (); ///< graphviz display
    Lexem      *root                         ();

    void        write_error                  ( const std::string &msg, const char *beg, const char *pos, const std::string &src, bool due_to_not_ended = false );
protected:

    template<bool use_stop_condition>
    void        read_s                       ( const char *limit = 0 );
    Lexem      *new_tok                      ( int type, const char *beg, unsigned len, int num );
    Lexem      *app_tok                      ( int type, const char *beg, unsigned len, int num );
    void        read_dollar                  ( const char *old_str, int num_operator = OPERATOR___string_assembly___NUM );
    bool        assemble_2_args              ( Lexem *o, int need_left, int need_right );
    bool        assemble_right_arg           ( Lexem *o );
    bool        assemble_left_arg            ( Lexem *o );
    void        rem_tok                      ( Lexem *l );

    void        make_flat_token_list         ();
    void        keyword_operator_assembly    ();
    void        not_in_replacement           ();
    void        make_sibling_lists           ();
    void        set_next_prev_fields         ();
    void        parenthesis_hierarchisation  ( const Lexem *ft ); ///< mark data into (), [] and {} as children
    void        make_unit_hierarchy          ();
    void        remove_cr                    ();
    void        assemble___str_and_var_assembly__();
    void        assemble_par_and_getattr     ( const Lexem *ft );
    void        assemble_op                  ( int mi, int ma );
    void        make_calls_without_par       ( Lexem *n, int phase);
    void        make_calls_without_par_fin   ( Lexem *n );
    void        remove_semicolons            ();
    void        update_else_order            ();

    void        set_num_scope                ( Lexem *b, int np, int &nsd, int &nss, int scope_type );

    const char *s;   ///< current char
    const char *beg; ///<
    const char *src; ///< provenance

    //    Lexem      *first_of_type [ NB_OPERATORS + Lexem::NB_BASE_TYPE ]; ///
    //    Lexem      *last_of_type  [ NB_OPERATORS + Lexem::NB_BASE_TYPE ]; ///
    Lexem      *first_of_group[ Lexem::NB_BASE_TYPE + NB_GROUPS ]; ///
    Lexem      *last_of_group [ Lexem::NB_BASE_TYPE + NB_GROUPS ]; ///
    Lexem       first_tok, *end;
    int         approx_line;
    int         spcr;

    LexPool     lexems;
    StrPool     str_pool;
    Error_list  *error_list;
};
