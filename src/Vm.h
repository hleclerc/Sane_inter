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
    using SFM   = std::map<RcString,std::function<Variable()>>;
    using SVM   = std::map<RcString,Variable>;
    using SVT   = std::map<RcString,Type *>;

    Vm( SI32 sizeof_ptr = 8 * sizeof( void * ), bool reverse_endianness = false );

    Variable      import       ( const String &filename, const String &import_dir = {}, bool display_lexems = false );
    String        path         ( const String &filename, const String &import_dir = {} );

    template<class ...Args>
    Variable      add_error    ( int nb_calls_to_skip, const String &msg, const Args &...args ) { disp_Error( prep_Error( nb_calls_to_skip, msg, args... ) ); return ref_error; }
    template<class ...Args>
    Variable      add_error    ( const String &msg, const Args &...args ) { return add_error( 0, msg, args... ); }

    template<class ...Args>
    Error        &prep_Error   ( int nb_calls_to_skip, const String &msg, const Args &...args ) { return prep_Error( 0, va_string( msg, args... ) ); }
    Error        &prep_Error   ( int nb_calls_to_skip, const String &msg );
    void          disp_Error   ( const Error &error ) const;
    RcString      src_name     ( size_t index ) const;

    Variable      visit        ( const RcString &names, const Vec<RcString> &code, bool want_ret );
    Variable      visit        ( const RcString &names, const RcString &code, bool want_ret );
    Variable      visit        ( const Vec<AstCrepr> &creps, bool want_ret );
    Variable      visit        ( const AstCrepr &ac, bool want_ret );

    bool          want_exec    () const { return true; }

    Variable      new_Type     ( Type *type );
    Variable      make_inst    ( Type *type, const Vec<Variable> &ctor_args, const Vec<RcString> &ctor_names, ApplyFlags apply_flags );
    Type         *type_ptr_for ( const RcString &name, const Vec<Variable> &args );

    #define BT( T ) Type *type_##T;
    #include "BaseTypes.h"
    #undef BT

    Variable      ref_void;
    Variable      ref_error;

    int           nb_breaks; ///<

    Position      pos;
    Scope        *scope;
    Deque<Type *> types;
    SVM           predefs;
    SFM           predeffs;
    Vec<String>   includes;
    int           nb_calls;
    MSV           imported;
    bool          init_mode;
    ErrorList     error_list;
    Scope         main_scope;
    SVT           base_types;
    CS            call_stack;
    SI32          sizeof_ptr;
    SI32          aligof_ptr;
    std::ostream *error_stream;
    bool          reverse_endianness;
};

