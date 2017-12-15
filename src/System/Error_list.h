#pragma once

#include "RcString.h"
#include "Stream.h"
#include <vector>
#include <string>
#include <deque>

/**
*/
class ErrorList {
public:
    struct Provenance {
        Provenance( const char *beg, const char *pos, const RcString &provenance, const RcString &msg = {} );
        Provenance( const String &src, int off, const RcString &msg = {} );
        Provenance( int line, const RcString &provenance );

        void      _init( const char *beg, const char *pos );

        RcString complete_line;
        RcString provenance; /// name of file
        int       line;
        int       col;
        RcString msg;
    };
    using VP = Vec<Provenance>;

    struct Error {
        Error();
        void      write_to_stream( std::ostream &os ) const;

        Error    &ac             ( const char *beg, const char *pos, const RcString &provenance ); ///< add a caller
        Error    &ac             ( const RcString &src, int off );

        Error    &ap             ( const char *beg, const char *pos, const RcString &provenance, const RcString &msg = "" ); ///< add a possibility
        Error    &ap             ( const RcString &src, int off, const RcString &msg = "" );

        bool      due_to_not_ended_expr, display_col, display_escape_sequences, warn;
        VP        caller_stack;  /// "copy" of caller stack
        VP        possibilities; /// if ambiguous overload, list of possible functions
        RcString msg;
    };

    ErrorList();

    Error            &add            ( const RcString &msg, bool warn = false, bool due_to_not_ended_expr = false );
    operator          bool           () const { return error_list.size(); }
    void              write_to_stream( std::ostream &os ) const;
    size_t            size           () const;

protected:
    std::deque<Error> error_list;
    bool              display_escape_sequences;
};

