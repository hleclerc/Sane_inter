#include "FileReader.h"
#include "Error_list.h"

#include <stdlib.h>
#include <string.h>

ErrorList::Provenance::Provenance( const char *beg, const char *pos, const RcString &provenance, const RcString &msg ) : provenance( provenance ), msg( msg ) {
    _init( beg, pos );
}

ErrorList::Provenance::Provenance( const String &src, int off, const RcString &msg ) : provenance( src ), msg( msg ) {
    if ( src.size() and off >= 0 ) {
        FileReader rf( src );
        if ( rf ) {
            _init( rf.data, rf.data + off );
            return;
        }
    }

    line = -1;
    col  = -1;
}

ErrorList::Provenance::Provenance( int line, const RcString &provenance ) : provenance( provenance ), line( line ) {
    col = -1;
}

void ErrorList::Provenance::_init( const char *beg, const char *pos ) {
    if ( not pos ) {
        col  = 0;
        line = 0;
        return;
    }

    const char *b = pos, *e = pos;
    while ( b >= beg and *b != '\n' and *b != '\r' )
        --b;
    ++b;
    while ( *e and *e != '\n' and *e != '\r' )
        ++e;
    if ( b > e )
        b = e;

    complete_line = RcString( b, e );

    col = pos - b + 1;
    line = 1;
    for ( b = pos; b >= beg; --b )
        line += ( *b == '\n' );
}

void display_line( std::ostream &os, const RcString &complete_line, int col, bool display_col ) {
    if ( display_col )
        os << "  ";
    if ( complete_line.size() < 64 ) {
        os << complete_line;
        if ( display_col ) {
            os << "\n";
            for ( int i = 1;i < 2 + col;++i )
                os << " ";
        }
    } else {
        if ( complete_line.size() - col < 64 ) { // only the ending
            os << "...";
            os.write( complete_line.data() + complete_line.size() - 64, 64 );
            if ( display_col ) {
                os << "\n";
                for ( unsigned i = 1;i < 2 + 64 + 3 - complete_line.size() + col;++i )
                    os << " ";
            }
        } else if ( col < 64 ) { // only the beginning
            os.write( complete_line.data(), 64 );
            os << "...";
            if ( display_col ) {
                os << "\n";
                for ( int i = 1;i < 2 + col;++i )
                    os << " ";
            }
        } else { // middle
            os << "...";
            os.write( complete_line.data() + col - 32, 61 );
            os << "...";
            if ( display_col ) {
                os << "\n";
                for ( int i = 1;i < 2 + 32 + 3;++i )
                    os << " ";
            }
        }
    }
    if ( display_col )
        os << "^";
}

ErrorList::Error::Error() {
    display_escape_sequences = true;
    due_to_not_ended_expr = false;
    display_col = true;
    warn = false;
}

ErrorList::Error &ErrorList::Error::ac( const char *beg, const char *pos, const RcString &provenance ) {
    caller_stack.push_back( beg, pos, provenance );
    return *this;
}

ErrorList::Error &ErrorList::Error::ac( const RcString &src, int off ) {
    if ( src && off >= 0 )
        caller_stack.push_back( src, off );
    return *this;
}

ErrorList::Error &ErrorList::Error::ap( const char *beg, const char *pos, const RcString &provenance, const RcString &msg ) {
    possibilities.push_back( beg, pos, provenance, msg );
    return *this;
}

ErrorList::Error &ErrorList::Error::ap( const RcString &src, int off, const RcString &msg ) {
    possibilities.push_back( src, off, msg );
    return *this;
}

void ErrorList::Error::write_to_stream( std::ostream &os ) const {
    // last item in caller stack
    if ( caller_stack.size() ) {
        if ( display_escape_sequences )
            os << "\033[1m";
        const ErrorList::Provenance &po = caller_stack[ 0 ];
        if ( po.provenance.size() )
            os << po.provenance << ":";
        if ( po.line ) {
            os << po.line;
            if ( po.col > 0 )
                os << ":" << po.col;
            os << ": ";
        }
        os << "error: " << msg << ( display_col ? "\n" : " in '" );
        if ( po.complete_line.size() )
            display_line( os, po.complete_line, po.col, display_col );
        os << ( display_col ? "\n" : "'\n" );
        if ( display_escape_sequences )
            os << "\033[0m";
    }
    else {
        if ( display_escape_sequences )
            os << "\033[1m";
        os << "error: " << msg << "\n";
        if ( display_escape_sequences )
            os << "\033[0m";
    }

    // caller_stack
    for( size_t num_prov = 1; num_prov < caller_stack.size(); ++num_prov ) {
        const ErrorList::Provenance &po = caller_stack[ num_prov ];
        if ( po.provenance.size() )
            os << po.provenance << ":";
        if ( po.line ) {
            os << po.line << ":" << po.col;
            //while ( num_prov>0 and error.caller_stack[ num_prov-1 ].line==po.line ) os << "," << error.caller_stack[ --num_prov ].col;
            os << ": ";
        }
        os << "instantiated from: ";
        display_line( os, po.complete_line, po.col, false );
        os << "\n";
    }

    // possibilities
    if ( possibilities.size() ) {
        os << "Possibilities are:" << std::endl;
        for ( size_t i = 0; i < possibilities.size();++i ) {
            for( size_t j = 0; ; ++j ) {
                if ( j == i ) {
                    const ErrorList::Provenance & po = possibilities[ i ];
                    if ( po.provenance.size() and po.line )
                        os << "" << po.provenance << ":" << po.line << ":" << po.col << ": ";
                    else
                        os << "(in primitive functions or classes)";
                    if ( po.msg.size() )
                        os << po.msg << ": ";
                    display_line( os, po.complete_line, po.col, false );
                    os << "\n";
                    break;
                }
                if ( possibilities[ i ].provenance == possibilities[ j ].provenance and
                     possibilities[ i ].line == possibilities[ j ].line and
                     possibilities[ i ].col  == possibilities[ j ].col )
                    break;
            }
        }
    }
}

static bool term_supports_color() {
    const char *c_term = getenv( "TERM" );
    return c_term && strcmp( c_term, "dumb" );
}


ErrorList::ErrorList() {
    display_escape_sequences = term_supports_color();
}

ErrorList::Error &ErrorList::add( const RcString &msg, bool warn, bool due_to_not_ended_expr ) {
    error_list.emplace_back();

    Error *e = &error_list.back();
    e->display_escape_sequences = display_escape_sequences;
    e->due_to_not_ended_expr = due_to_not_ended_expr;
    e->display_col = true;
    e->warn = warn;
    e->msg = msg;

    return *e;
}

void ErrorList::write_to_stream( std::ostream &os ) const {
    for ( size_t i = 0; i < error_list.size(); ++i )
        os << error_list[ i ];
}

size_t ErrorList::size() const {
    size_t res = 0;
    for( size_t i = 0; i < error_list.size(); ++i )
        res += error_list[ i ].warn == false;
    return res;
}
