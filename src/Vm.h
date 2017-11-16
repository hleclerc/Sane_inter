#pragma once

#include "System/Error_list.h"
#include "System/va_string.h"
#include "System/Deque.h"
#include "System/Vec.h"
#include "Variable.h"
#include <map>

/**
*/
class Vm {
public:
    struct CallStackItem {};
    using MSV   = std::map<String,Variable>;
    using CS    = Deque<CallStackItem>;
    using Error = Error_list::Error;

    Vm();

    Variable      import      ( const String &filename, const String &import_dir = {}, bool display_lexems = false );
    String        path        ( const String &filename, const String &import_dir = {} );

    template<class ...Args>
    void          add_error   ( int nb_calls_to_skip, const String &msg, const Args &...args ) { disp_Error( prep_Error( nb_calls_to_skip, msg, args... ) ); }
    template<class ...Args>
    void          add_error   ( const String &msg, const Args &...args ) { add_error( 0, msg, args... ); }

    template<class ...Args>
    Error        &prep_Error  ( int nb_calls_to_skip, const String &msg, const Args &...args ) { return prep_Error( 0, va_string( msg, args... ) ); }
    Error        &prep_Error  ( int nb_calls_to_skip, const String &msg );
    void          disp_Error  ( const Error &error ) const;

    Vec<String>   includes;
    MSV           imported;
    bool          init_mode;
    Variable      ref_error;
    Error_list    error_list;
    CS            call_stack;
    std::ostream *error_stream;
};

