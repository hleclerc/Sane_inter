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
    struct ModFd {
        RcPtr<Inst> mod_inst;    ///< last modifying inst
        bool        mod_cursor;  ///< true if modified position in the file
        bool        mod_content; ///< true if modified the content of the file
    };

    using MSV   = std::map<String,Variable>;
    using Error = ErrorList::Error;
    using SFM   = std::map<RcString,std::function<Variable()>>;
    using SVM   = std::map<RcString,Variable>;
    using SVT   = std::map<RcString,Type *>;
    using MMFD  = std::map<Value,ModFd>; ///< fd -> mod

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

    Variable      visit        ( const RcString &names, const Vec<RcString> &code, bool want_ret );
    Variable      visit        ( const RcString &names, const RcString &code, bool want_ret );
    Variable      visit        ( const Vec<AstCrepr> &creps, bool want_ret );
    Variable      visit        ( const AstCrepr &ac, bool want_ret );

    bool          want_exec    () const { return true; }

    Type         *type_ptr_for ( const RcString &name, const Vec<Variable> &args );
    Variable      make_inst    ( Type *type, const Vec<Variable> &ctor_args, const Vec<RcString> &ctor_names, ApplyFlags apply_flags );
    Variable      new_Type     ( Type *type );

    bool          little_endian() const;

    void          mod_fd       ( RcPtr<Inst> mod_inst, const Value &fd, bool mod_cursor, bool mod_content );

    void          display_graph();

    #define BT( T ) Type *type_##T;
    #include "BaseTypes.h"
    #undef BT

    Variable      ref_void;
    Variable      ref_error;

    int           nb_breaks; ///<

    Scope        *scope;
    Deque<Type *> types;
    SVM           predefs;
    MMFD          mod_fds;            ///< modified file descriptors
    SFM           predeffs;
    Vec<String>   includes;
    int           nb_calls;
    MSV           imported;
    bool          init_mode;
    ErrorList     error_list;
    Scope         main_scope;
    SVT           base_types;
    SI32          sizeof_ptr;
    SI32          aligof_ptr;
    std::ostream *error_stream;
    bool          reverse_endianness;
};

