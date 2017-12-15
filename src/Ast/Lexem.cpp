#include "Helpers/(GenOperatorsInfo.ts).h"
#include "Helpers/Char_type.h"
#include "Lexem.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>

Lexem::Lexem() {
    children[ 0 ]          = 0;
    children[ 1 ]          = 0;
    parent                 = 0;
    next                   = 0;
    prev                   = 0;
    sibling                = 0;
    preceded_by_a_cr       = false;
    flags                  = Flags::NONE;

    beg_src                = 0;
    o_beg                  = 0;
    o_len                  = 0;
    src                    = 0;

    nb_preceding_cr        = 0;
}

void Lexem::write_rec( std::ostream &os, const std::string &sp ) const {
    os << sp << *this;
    if ( children[ 0 ] ) children[ 0 ]->write_rec( os << '\n', sp + " " );
    if ( children[ 1 ] ) children[ 1 ]->write_rec( os << '\n', sp + " " );
    if ( next ) next->write_rec( os << '\n', sp );
}

void Lexem::to_rat( int &num, int &den ) const {
    num = 0;
    den = 1;
    for( int i = 0; i < len; ++i ) {
        if ( beg[ i ] == '.' ) {
            for( int j = i + 1; j < len; ++j ) {
                num = num * 10 + ( beg[ j ] - '0' );
                den *= 10;
            }
            break;
        }
        num = num * 10 + ( beg[ i ] - '0' );
    }
}

void Lexem::to_num_exp( int &num, int &exp ) const {
    num = 0;
    exp = 0;
    for( int i = 0; i < len; ++i ) {
        if ( beg[ i ] == '.' ) {
            for( int j = i + 1; j < len; ++j ) {
                num = num * 10 + ( beg[ j ] - '0' );
                exp += 1;
            }
            break;
        }
        num = num * 10 + ( beg[ i ] - '0' );
    }
}

int Lexem::to_int() const {
    int res = 0;
    for( int i = 0; i < len and beg[ i ] >= '0' and beg[ i ] <= '9'; ++i )
        res = res * 10 + ( beg[ i ] - '0' );
    return res;
}

PI64 Lexem::to_PI64() const {
    PI64 res = 0;
    for( int i = 0; i < len and beg[ i ] >= '0' and beg[ i ] <= '9'; ++i )
        res = res * 10 + ( beg[ i ] - '0' );
    return res;
}

bool Lexem::is_an_int() const {
    if ( type != NUMBER )
        return false;
    for( int i = 0; i < len; ++i )
        if ( char_type( beg[ i ] ) != CHAR_TYPE_number )
            return false;
    return true;
}

bool Lexem::eq( const Hpipe::CmString &p ) const {
    return len == int( p.size() ) && strncmp( (const char *)p.get_beg(), beg, len ) == 0;
}

bool Lexem::eq( const std::string &p ) const {
    return len == int( p.size() ) && strncmp( p.data(), beg, len ) == 0;
}

bool Lexem::eq( const char *p ) const {
    return len == int( strlen( p ) ) && strncmp( p, beg, len ) == 0;
}

bool Lexem::eq( char p ) const {
    return len == 1 && *beg == p;
}

bool Lexem::begin_with( const char *p ) const {
    return int( strlen( p ) ) <= len and strncmp( p, beg, strlen( p ) ) == 0;
}

bool Lexem::same_str( const char *p, int l ) const {
    return len == l and strncmp( beg, p, l ) == 0;
}

const Lexem *Lexem::has_child_type( int wanted_type ) const {
    if ( type == wanted_type )
        return this;
    if ( const Lexem *ch = children[ 0 ] )
        if ( const Lexem *res = ch->has_child_type( wanted_type ) )
            return res;
    if ( const Lexem *ch = children[ 1 ] )
        if ( const Lexem *res = ch->has_child_type( wanted_type ) )
            return res;
    return 0;
}

void Lexem::remove() {
    if ( parent )
        parent->children[ parent->children[ 1 ] == this ] = next;
    if ( prev )
        prev->next = next;
    if ( next ) {
        next->parent = parent;
        next->prev = prev;
    }
    parent = 0;
    prev = 0;
    next = 0;
}

static void disp_for_record( std::ostream &os, const char *beg, size_t len ) {
    // hum
    if ( len > 10000 )
        return;
    for( size_t i = 0; i < len; ++i ) {
        if ( beg[ i ]=='\n' )
            os << "\\n";
        else if ( beg[ i ]=='"' || beg[ i ]=='{' || beg[ i ]=='}' || beg[ i ]=='|' || beg[ i ]=='<' || beg[ i ]=='>' )
            os << "\\" << beg[ i ];
        else
            os << beg[ i ];
    }
}

void display_graph_rec( std::ostream &os, const Lexem *t, unsigned level, unsigned &max_level, const Lexem *avoid ) {
    if ( t == avoid )
        return;

    unsigned j;
    max_level = std::max( max_level, level );
    const Lexem *old_t = t;
    // nodes
    for( ; t ; t = t->next ) {
        os << "{ rank = same; " << level << " node" << t << " [shape=record,label=\"";
        //if ( t->preceded_by_a_cr ) os << "[CR]";
        //if ( t->nb_preceding_semicolon )
        //    os << "[;=>" << t->nb_preceding_semicolon << ']';
        //if ( t->nb_preceding_cr )
        //    os << "[n=>" << t->nb_preceding_cr << ']';

        bool disp_str = true;
        if ( t->type >= 0 ) {
            std::ostringstream op;
            if ( size_t len = operators_met_size( t->type ) )
                op.write( operators_met_name( t->type ), len );
            else
                op << operators_cpp_name( t->type );

            std::string str = op.str();
            disp_for_record( os, str.data(), str.size() );
        } else {
            switch ( t->type ) {
            case Lexem::CR             : os << "CR" << t->num; disp_str = false; break;
            case Lexem::BLOCK          : os << "BL" << t->num; break;
            case Lexem::NONE           : os << "NO"; disp_str = false; break;
            case Lexem::APPLY          : os << ( t->flags & Lexem::Flags::NEW ? "new" : "()" ); break;
            case Lexem::SELECT         : os << "[]"; break;
            case Lexem::CHBEBA: os << "{}"; break;
            case Lexem::CCODE          : os << "``"; break;
            case Lexem::NUMBER         : os << "NU"; break;
            case Lexem::VARIABLE       : os << "VA"; break;
            case Lexem::STRING         : os << "ST" << t->num; break;
            case Lexem::PAREN          : os << "PA"; break;
            default                    : os << t->type; break;
                // case STRING___if___NUM     : os << "If"; break;
                //case OPERATOR_not_boolean_NUM: os << "!"; break;
            }
        }



        if ( disp_str ) {
            os << "|";
            disp_for_record( os, t->o_beg, t->o_len );
        }

        os << "\"] }\n";

        // children
        for( j = 0; j < 2; ++j ) {
            if ( t->children[ j ] and t->children[ j ] != avoid ) {
                display_graph_rec( os, t->children[ j ], level + 1, max_level, avoid );
                os << "  node" << t << " -> node" << t->children[ j ] << ";\n";
            }
        }
        // ascending edge
        if ( t->parent and t->parent != avoid )
            os << "  node" << t << " -> node" << t->parent << " [color=red];\n";
        //        if ( t->parent_paren )
        //            os << "  node" << t << " -> node" << t->parent_paren << " [color=blue];\n";
    }
    // edges
    t = old_t;
    for( ; t ; t = t->next ) {
        if ( t->next and t->next != avoid )
            os << "  node" << t << " -> node" << t->next << " [color=green];\n";
        if ( t->prev and t->prev != avoid ) //
            os << "  node" << t << " -> node" << t->prev << " [color=yellow];\n";
    }
}

void display_graph( const Lexem *t, const char *file_name, const Lexem *avoid ) {
    std::ofstream f(file_name);
    f << "digraph popoterie {";
    unsigned max_level = 1;
    display_graph_rec( f, t, 1, max_level, avoid );

    for( unsigned i = 1; i <= max_level; ++i )
        f << "  " << i << " [ shape=plaintext ];\n  ";
    for( unsigned i = 1; i <= max_level; ++i )
        f << i << ( i<max_level ? " -> " : ";" );

    f << "}";
    f.close();

    int res = system( ("dot -Tps "+std::string(file_name)+" > "+std::string(file_name)+".eps && gv "+std::string(file_name)+".eps").c_str() );
    if ( res )
        res = 0;
}


