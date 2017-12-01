#pragma once

#include "Rc_string.h"
#include "Stream.h"
#include <vector>
#include <string>
#include <deque>

/**
*/
class Error_list {
public:
    struct Provenance {
        Provenance( const char *beg, const char *pos, const Rc_string &provenance, const Rc_string &msg = {} );
        Provenance( const Rc_string &src, int off, const Rc_string &msg = {} );
        Provenance( int line, const Rc_string &provenance );

        void      _init( const char *beg, const char *pos );

        Rc_string complete_line;
        Rc_string provenance; /// name of file
        int       line;
        int       col;
        Rc_string msg;
    };
    using VP = Vec<Provenance>;

    struct Error {
        Error();
        void      write_to_stream( std::ostream &os ) const;

        Error    &ac             ( const char *beg, const char *pos, const Rc_string &provenance ); ///< add a caller
        Error    &ac             ( const Rc_string &src, int off );

        Error    &ap             ( const char *beg, const char *pos, const Rc_string &provenance, const Rc_string &msg = "" ); ///< add a possibility
        Error    &ap             ( const Rc_string &src, int off, const Rc_string &msg = "" );

        bool      due_to_not_ended_expr, display_col, display_escape_sequences, warn;
        VP        caller_stack;  /// "copy" of caller stack
        VP        possibilities; /// if ambiguous overload, list of possible functions
        Rc_string msg;
    };

    Error_list();

    Error            &add            ( const Rc_string &msg, bool warn = false, bool due_to_not_ended_expr = false );
    operator          bool           () const { return error_list.size(); }
    void              write_to_stream( std::ostream &os ) const;
    size_t            size           () const;

protected:
    std::deque<Error> error_list;
    bool              display_escape_sequences;
};

