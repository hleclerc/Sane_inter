#pragma once

#include "System/Error_list.h"
#include "System/Deque.h"
#include "Scope.h"
#include "Type.h"

/**
*/
class Vm {
public:
    using SFM = std::map<Rc_string,std::function<Variable( Scope *scope )>>;
    using SVM = std::map<Rc_string,Variable>;
    using SVT = std::map<Rc_string,Type *>;
    using DV  = Deque<Variable>;

    Vm();
    ~Vm();

    Variable       import      ( const Rc_string &filename, const Rc_string &import_dir );
    Variable       import      ( const Rc_string &absolute_filename, bool display_lexems = false );
    Type          *type_ptr_for( const Rc_string &name, const Vec<Variable> &args );

    Variable       new_String  ( const Rc_string &str );
    Variable       new_Type    ( Type *type );
    Variable       new_Bool    ( Bool value );
    Variable       new_SI32    ( SI32 value );
    Variable       new_PT      ( PT   value );

    Error_list     el;
    Deque<Type *>  types;
    bool           init_mode;
    Scope          main_scope;

    #define BT( T ) Type *type_##T;
    #include "BaseTypes.h"
    #undef BT
    SVT            base_types;
    Variable       ref_error;
    SVM            imported;
    Vec<Rc_string> includes;
    Variable       ref_void;
    SFM            predeffs;
    int            nb_calls;
    SVM            predefs;
    int            nb_enum;
    int            argc;
    char         **argv;
    std::ostream  *os;
};
