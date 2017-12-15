#pragma once

#include "../System/TypeConfig.h"
#include "../System/EnumFlags.h"
#include "../System/Stream.h"
#include <Hpipe/CmString.h>
#include <string>

/** */
class Lexem {
public:
    enum {
        VARIABLE        = -1, // must be -1 ( @see num_operator_le )
        NUMBER          = -2,
        STRING          = -3,
        CR              = -4,
        PAREN           = -5,
        CCODE           = -6,
        BLOCK           = -9,
        NONE            = -10,
        APPLY           = -12,
        SELECT          = -13,
        CHBEBA = -14,

        NB_BASE_TYPE    = 16
    };
    enum class Flags: int {
        NONE            = 0,

        // flags for apply
        NEW             = 1,
    };

    Lexem();

    template<class Os>
    void         write_to_stream( Os &os ) const { os.write( beg, len ); }

    void         write_rec      ( std::ostream &os, const std::string &sp = "" ) const; ///< display this, children and next

    void         to_rat         ( int &num, int &den ) const; ///< assuming this is a number
    void         to_num_exp     ( int &num, int &exp ) const; ///< assuming this is a number
    int          to_int         () const; ///< assuming this is a number
    PI64         to_PI64        () const; ///< assuming this is a number
    bool         is_an_int      () const;
    bool         eq             ( const Hpipe::CmString &p ) const;
    bool         eq             ( const std::string &p ) const;
    bool         eq             ( const char *p ) const;
    bool         eq             ( char p ) const;
    bool         begin_with     ( const char *p ) const;
    bool         same_str       ( const char *p, int len ) const;
    const char  *end            () const { return beg + len; }
    const char  *o_end          () const { return o_beg + o_len; }
    std::string  str            () const { return { beg, beg + len }; }
    int          off            () const { return o_beg - beg_src; }
    const Lexem *has_child_type ( int type ) const;
    void         remove         ();

    int          type;    /// >0 => operator. <=0 => @see enum
    int          num;     /// Used by type==CR for nb_spaces. If type==CCODE, num in size_cvar

    const char  *beg_src; /// provenance data (beginning of the .met data)
    const char  *src;     /// provenance

    const char  *o_beg;   /// orig beginning in .met
    int          o_len;   /// orig size in .met

    const char  *beg;     /// beginning in .met
    int          len;     /// size in .met

    Lexem       *children[ 2 ], *parent, *next, *prev, *sibling, *parent_paren, *prev_chrono;

    bool         preceded_by_a_cr; ///<
    // int       nb_preceding_semicolon;
    int          nb_preceding_cr;
    int          approx_line;
    int          spcr;
    Flags        flags;
};
ENUM_FLAGS( Lexem::Flags )

inline int is_in_main_block( const Lexem *t ) { while( t->prev ) t = t->prev; return not t->parent; }
inline int need_larg( const Lexem *t ) { return t && t->type >= 0 && ( t->num & 1 ); }
inline int need_rarg( const Lexem *t ) { return t && t->type >= 0 && ( t->num & ( 2 | 16 ) ); }

void display_graph( const Lexem *t, const char *file_name = "tmp/res.dot", const Lexem *avoid = 0 );

/// a,b,c -> [a b c]
template<class CL,class TL>
void get_children_of_type( CL t, int type, TL &res ) {
    if ( not t )
        return;
    if ( t->type == type ) {
        get_children_of_type( t->children[ 0 ], type, res );
        res.push_back( t->children[ 1 ] );
        return;
    }
    res.push_back( t );
}

///
template<class TL>
void get_leaves( TL &ch, const Lexem *l ) {
    if ( not l )
        return;
    if ( l->children[ 0 ] or l->children[ 1 ] ) {
        get_leaves( ch, l->children[ 0 ] );
        get_leaves( ch, l->children[ 1 ] );
    } else
        ch << l;
}

// a,b,c -> 3
inline unsigned nb_children_of_type( const Lexem *t, int type ) {
    if ( not t )
        return 0 ;
    return t->type == type ? nb_children_of_type( t->children[ 0 ], type ) + 1 : 1;
}

//int offset_to_doc_of( const Lexem *l, const char *sar_txt );

inline const Lexem *child_if_block( const Lexem *t ) { if ( t->type == Lexem::BLOCK ) return t->children[ 0 ]; return t; } ///
inline       Lexem *child_if_block(       Lexem *t ) { if ( t->type == Lexem::BLOCK ) return t->children[ 0 ]; return t; } ///

inline const Lexem *child_if_paren( const Lexem *t ) { if ( t->type == Lexem::PAREN ) return t->children[ 0 ]; return t; } ///

inline const Lexem *next_if_CR( const Lexem *cc ) {
    if ( cc and cc->type == Lexem::CR )
        return cc->next;
    return cc;
}
