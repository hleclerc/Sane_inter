#pragma once

#include "System/Error_list.h"
#include "System/va_string.h"
#include "System/Deque.h"
#include "System/Vec.h"
#include "Variable.h"
#include "Scope.h"
#include <map>
class Type;

/**
*/
class Vm {
public:
    struct Position { RcString cur_names; size_t cur_src; size_t cur_off; };
    struct CallStackItem {};
    using MSV   = std::map<String,Variable>;
    using CS    = Deque<CallStackItem>;
    using Error = ErrorList::Error;

    Vm();

    Variable      import      ( const String &filename, const String &import_dir = {}, bool display_lexems = false );
    String        path        ( const String &filename, const String &import_dir = {} );

    template<class ...Args>
    Variable      add_error   ( int nb_calls_to_skip, const String &msg, const Args &...args ) { disp_Error( prep_Error( nb_calls_to_skip, msg, args... ) ); return ref_error; }
    template<class ...Args>
    Variable      add_error   ( const String &msg, const Args &...args ) { return add_error( 0, msg, args... ); }

    template<class ...Args>
    Error        &prep_Error  ( int nb_calls_to_skip, const String &msg, const Args &...args ) { return prep_Error( 0, va_string( msg, args... ) ); }
    Error        &prep_Error  ( int nb_calls_to_skip, const String &msg );
    void          disp_Error  ( const Error &error ) const;

    Variable      visit       ( const RcString &names, const Vec<RcString> &code, bool want_ret );
    Variable      visit       ( const RcString &names, const RcString &code, bool want_ret );
    Variable      visit       ( const Vec<AstCrepr> &creps, bool want_ret );
    Variable      visit       ( const AstCrepr &ac, bool want_ret );

    bool          want_exec   () const { return true; }

    #define BT( T ) Type *type_##T;
    #include "BaseTypes.h"
    #undef BT

    Position      pos;
    Scope        *scope;
    Vec<String>   includes;
    MSV           imported;
    bool          init_mode;
    Variable      ref_void;
    Variable      ref_error;
    ErrorList    error_list;
    Scope         main_scope;
    CS            call_stack;
    std::ostream *error_stream;
};

