#include "File_reader.h"
#include "Error_list.h"

#include <stdlib.h>
#include <string.h>

Error_list::Provenance::Provenance( const char *beg, const char *pos, const Rc_string &provenance, const Rc_string &msg ) : provenance( provenance ), msg( msg ) {
    _init( beg, pos );
}

Error_list::Provenance::Provenance( const Rc_string &src, int off, const Rc_string &msg ) : provenance( src ), msg( msg ) {
    if ( src.size() and off >= 0 ) {
        File_reader rf( src );
        if ( rf ) {
            _init( rf.data, rf.data + off );
            return;
        }
    }

    line = -1;
    col  = -1;
}

Error_list::Provenance::Provenance( int line, const Rc_string &provenance ) : provenance( provenance ), line( line ) {
    col = -1;
}

void Error_list::Provenance::_init( const char *beg, const char *pos ) {
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

    complete_line = Rc_string( b, e );

    col = pos - b + 1;
    line = 1;
    for ( b = pos; b >= beg; --b )
        line += ( *b == '\n' );
}

void display_line( std::ostream &os, const Rc_string &complete_line, int col, bool display_col ) {
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

Error_list::Error::Error() {
    display_escape_sequences = true;
    due_to_not_ended_expr = false;
    display_col = true;
    warn = false;
}

Error_list::Error &Error_list::Error::ac( const char *beg, const char *pos, const Rc_string &provenance ) {
    caller_stack.push_back( beg, pos, provenance );
    return *this;
}

Error_list::Error &Error_list::Error::ac( const Rc_string &src, int off ) {
    if ( src && off >= 0 )
        caller_stack.push_back( src, off );
    return *this;
}

Error_list::Error &Error_list::Error::ap( const char *beg, const char *pos, const Rc_string &provenance, const Rc_string &msg ) {
    possibilities.push_back( beg, pos, provenance, msg );
    return *this;
}

Error_list::Error &Error_list::Error::ap( const Rc_string &src, int off, const Rc_string &msg ) {
    possibilities.push_back( src, off, msg );
    return *this;
}

void Error_list::Error::write_to_stream( std::ostream &os ) const {
    // last item in caller stack
    if ( caller_stack.size() ) {
        if ( display_escape_sequences )
            os << "\033[1m";
        const Error_list::Provenance &po = caller_stack[ 0 ];
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
        const Error_list::Provenance &po = caller_stack[ num_prov ];
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
                    const Error_list::Provenance & po = possibilities[ i ];
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


Error_list::Error_list() {
    display_escape_sequences = term_supports_color();
}

Error_list::Error &Error_list::add( const Rc_string &msg, bool warn, bool due_to_not_ended_expr ) {
    error_list.emplace_back();

    Error *e = &error_list.back();
    e->display_escape_sequences = display_escape_sequences;
    e->due_to_not_ended_expr = due_to_not_ended_expr;
    e->display_col = true;
    e->warn = warn;
    e->msg = msg;

    return *e;
}

void Error_list::write_to_stream( std::ostream &os ) const {
    for ( size_t i = 0; i < error_list.size(); ++i )
        os << error_list[ i ];
}

size_t Error_list::size() const {
    size_t res = 0;
    for( size_t i = 0; i < error_list.size(); ++i )
        res += error_list[ i ].warn == false;
    return res;
}
