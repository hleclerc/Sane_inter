#pragma once

#include "Helpers/Ast_writer.h"
#include "../System/Deque.h"
#include "Ast_visitor.h"
#include "Ast_crepr.h"
#include <functional>
#include <map>

class Error_list;

/**
  Make a compact representation of an Ast tree from
 */
class Ast_maker {
public:
    enum { PRIVATE = 1, PROTECTED = 2 };

    Ast_maker( Error_list *error_list );

    void                 parse            ( const char *content, const char *sourcefile, bool display_lexems = false ); ///< add nb_tokens + offsets to $nb_tokens tokens
    Ast_crepr            to_Ast_crepr     () const; ///< compact representation
    void                 write_to_stream  ( std::ostream &os ) const;

protected:
    using InhNameMap = std::map<const Lexem *,Vec<Hpipe::CmString>>;

    void                 make_ast_variable ( const Lexem *lex );
    void                 make_ast_operator ( const Lexem *lex );
    void                 make_ast_number   ( const Lexem *lex );
    void                 make_ast_string   ( const Lexem *lex );
    void                 make_ast_paren    ( const Lexem *lex );
    void                 make_ast_block    ( const Lexem *lex );
    void                 make_ast_assign   (const Lexem *lex, int type );
    void                 make_ast_callable ( const Lexem *lex, bool def );
    void                 make_ast_enum     ( const Lexem *lex );
    void                 make_ast_call_op  ( const Lexem *lex );
    void                 make_ast_self     ( const Lexem *lex );
    void                 make_ast_this     ( const Lexem *lex );
    void                 make_ast_pass     ( const Lexem *lex );
    void                 make_ast_if       ( const Lexem *lex );
    void                 make_ast_try      ( const Lexem *lex );
    void                 make_ast_while    ( const Lexem *lex );
    void                 make_ast_global   ( const Lexem *lex );
    void                 make_ast_virtual  ( const Lexem *lex );
    void                 make_ast_override ( const Lexem *lex );
    void                 make_ast_inline   ( const Lexem *lex );
    void                 make_ast_abstract ( const Lexem *lex );
    void                 make_ast_export   ( const Lexem *lex );
    void                 make_ast_static   ( const Lexem *lex );
    void                 make_ast_mixin    ( const Lexem *lex );
    void                 make_ast_const    ( const Lexem *lex );
    void                 make_ast_for      ( const Lexem *lex );
    void                 make_ast_import   ( const Lexem *lex );
    void                 make_ast_lambda   ( const Lexem *lex );
    void                 make_ast_get_attr ( const Lexem *lex, bool ptr, bool ask, bool ddo );
    void                 make_ast_init_of  ( const Lexem *lex );
    void                 make_ast_and      ( const Lexem *lex );
    void                 make_ast_or       ( const Lexem *lex );
    void                 make_ast_alternat ( const Lexem *lex );
    void                 make_var_assembly ( const Lexem *lex );

    void                 make_ast_apply    ( const Lexem *lex, const std::function<void (const Lexem *, const Vec<const Lexem *> &, const Vec<Hpipe::CmString> &, const Vec<size_t> &)> &cb );

    void                 make_ast_block    ( const Vec<const Lexem *> &vl );
    void                 make_single       ( const Lexem *l );
    Vec<const Lexem *>   make_lex_vec_from ( const Lexem *l, const Lexem *end = 0 );
    void                 add_error         ( const std::string &msg, const Lexem *l );
    Hpipe::CmString      inheritance_name  ( size_t i );
    const Lexem         *parent_class      ( const Lexem *lex );

    Error_list          *error_list;
    Ast_writer           aw;
    bool                 global_inst;
    bool                 export_inst;
    bool                 static_inst;
    bool                 private_inst;
    bool                 protected_inst;
    bool                 virtual_inst;
    bool                 override_inst;
    bool                 inline_inst;
    bool                 abstract_inst;
    bool                 mixin_inst;
    bool                 const_inst;
    Deque<Lexem>         lexem_pool;
    Deque<std::string>   inheritance_name_string_pool;
    InhNameMap           inheritance_names;
};
