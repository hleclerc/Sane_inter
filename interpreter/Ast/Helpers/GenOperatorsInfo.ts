
const Needlarg    = 1
const Needrarg    = 2
const Need2args   = 3
const MayNeedlarg = 4;
const MayNeedrarg = 8;
const Needrrargs  = 16
const Needplrargs = MayNeedlarg+Needrarg;
//
class Operator {
    // constructor( a, b, c = 0, d = -1 ):
    //     self.met_name = a
    //     self.cpp_name = b
    //     self.behavior = c
    //     self.group    = d
    //     self.num      = -1
    // def of_type( self, ty ):
    //     if len( self.met_name ) == 0:
    //         return False
    //     op = self.met_name[ 0 ] in string.letters
    //     return op ^ ( ty == "op" )
    // def nb_args( self ):
    //     if ( self.behavior == Need2args ) or ( self.behavior == Needrrargs ):
    //         return 2
    //     return 1

    of_type( ty: string ): boolean {
        if ( this.met_name.length == 0 )
            return false;
        const op = Boolean( this.met_name[ 0 ].match( /[a-zA-Z]/ ) );
        return Boolean( Number( op ) ^ Number( ty == "op" ) );
    }

    cpp_tr(): string {
        return this.met_name.length == 0 || this.met_name[ 0 ].match( /[a-zA-Z]/ ) ? this.cpp_name : `operator ${ this.met_name }`;
    }

    right_to_left_associativity: boolean;
    met_name                   : string;
    cpp_name                   : string;
    behavior                   : number;
    group                      : number;
}

function O( met_name: string, cpp_name: string, behavior = 0, group = -1, right_to_left_associativity = false ) {
    let res = new Operator;
    res.right_to_left_associativity = right_to_left_associativity;
    res.met_name                    = met_name;
    res.cpp_name                    = cpp_name;
    res.behavior                    = behavior;
    res.group                       = group;
    return res;
}

// TODO: expects a block (for instance =>, ...)
const operators = [
    O( ";"          , "semicolon"                                     ),

    O( "try"        , "__try__"               , Needrrargs ,  0, true ),
    O( "catch"      , "__catch__"             , Needrrargs ,  0, true ),
    O( "static"     , "__static__"            , Needrarg   ,  0, true ),
    O( "global"     , "__global__"            , Needrarg   ,  0, true ),
    O( "export"     , "__export__"            , Needrarg   ,  0, true ),
    O( "mixin"      , "__mixin__"             , Needrarg   ,  0, true ),
    O( "const"      , "__const__"             , Needrarg   ,  0, true ),
    O( "virtual"    , "__virtual__"           , Needrarg   ,  0, true ),
    O( "override"   , "__override__"          , Needrarg   ,  0, true ),
    O( "inline"     , "__inline__"            , Needrarg   ,  0, true ),
    O( "abstract"   , "__abstract__"          , Needrarg   ,  0, true ),
    O( "import"     , "__import__"            , Needrarg   ,  0, true ),
    O( "from"       , "__from__"              , Needrarg   ,  0, true ),

    //
    O( "class"      , "__class__"             , Needrrargs ,  1 ),
    O( "def"        , "__def__"               , Needrrargs ,  1 ),
    O( "enum"       , "__enum__"              , Needrrargs ,  1 ),

    O( "extends"    , "__extends__"           , Need2args  ,  2 ),
    O( "pertinence" , "__pertinence__"        , Need2args  ,  2 ),
    O( "when"       , "__when__"              , Need2args  ,  2 ),
    O( "with"       , "__with__"              , Need2args  ,  2 ),
    O( "~>"         , "ret_type"              , Need2args  ,  2 ),
    O( "wpc"        , "__wpc__"               , Needrarg   ,  2 ),
    O( "init_of"    , "__init_of__"           , Needrarg   ,  2 ),
    
    //
    O( "while"      , "__while__"             , Needrrargs ,  5, true ),
    O( "for"        , "__for__"               , Needrrargs ,  5, true ),
    O( "if"         , "__if__"                , Needrrargs ,  5, true ),
    O( "else"       , "__else__"              , Need2args  ,  5, true ),
    O( "="          , "reassign"              , Need2args  ,  5, true ),
    O( ":="         , "assign"                , Need2args  ,  5, true ),
    O( "@="         , "assign_ref"            , Need2args  ,  5, true ),
    O( "~="         , "assign_type"           , Need2args  ,  5, true ),
    O( "|="         , "self_or_bitwise"       , Need2args  ,  5, true ),
    O( "^="         , "self_xor_bitwise"      , Need2args  ,  5, true ),
    O( "&="         , "self_and_bitwise"      , Need2args  ,  5, true ),
    O( "||="        , "self_or_boolean"       , Need2args  ,  5, true ),
    O( "^^="        , "self_xor_boolean"      , Need2args  ,  5, true ),
    O( "&&="        , "self_and_boolean"      , Need2args  ,  5, true ),
    O( "%="         , "self_mod"              , Need2args  ,  5, true ),
    O( "<<="        , "self_shift_left"       , Need2args  ,  5, true ),
    O( ">>="        , "self_shift_right"      , Need2args  ,  5, true ),
    O( "/="         , "self_div"              , Need2args  ,  5, true ),
    O( "*="         , "self_mul"              , Need2args  ,  5, true ),
    O( "-="         , "self_sub"              , Need2args  ,  5, true ),
    O( "+="         , "self_add"              , Need2args  ,  5, true ),
    O( "//="        , "self_div_int"          , Need2args  ,  5, true ),
    O( "+++="       , "self_concatenate"      , Need2args  ,  5, true ),
    O( "ref"        , "__ref__"               , Needrarg   ,  5, true ),
    
    // ------------------------------------------------------------
    // limit auto function call (without parenthesis)
    // ------------------------------------------------------------

    //
    O( ","          , "comma"                 , Need2args  ,  8 ),

    //
    O( "=>"         , "double_arrow"          , Needplrargs, 11, true ),
    O( "?"          , "__alternative__"       , Need2args  , 11, true ),
    O( ":"          , "doubledot"             , Need2args  , 11, true ),
    O( ":."         , "doubledotdot"          , Need2args  , 11, true ),

    //
    O( "<<<<"       , "shift_left_long_str"   , Need2args  , 13 ),
    O( "<<<"        , "shift_left_then_endl"  , Need2args  , 13 ),
    O( "<<"         , "shift_left"            , Need2args  , 13 ),
    O( ">>"         , "shift_right"           , Need2args  , 13 ),

    //
    O( "or"         , "or_boolean__"          , Need2args  , 14 ),
    O( "||"         , "or_boolean_sgn__"      , Need2args  , 14 ),

    O( "and"        , "and_boolean__"         , Need2args  , 15 ),
    O( "&&"         , "and_boolean_sgn__"     , Need2args  , 15 ),

    O( "xor"        , "xor_boolean___"        , Need2args  , 16 ),
    O( "^^"         , "xor_boolean_sgn___"    , Need2args  , 16 ),

    //
    O( "not"        , "not_boolean"           , Needrarg   , 17, true ),
    O( "!"          , "not_boolean_sgn"       , Needrarg   , 17, true ),

    // bitwise
    O( "|"          , "or_bitwise"            , Need2args  , 18 ),

    O( "^"          , "xor_bitwise"           , Need2args  , 19 ),

    O( "&"          , "and_bitwise"           , Need2args  , 20 ),
    
    //
    O( "is_a"       , "is_a"                  , Need2args  , 21 ),
    O( "is_not_a"   , "is_not_a"              , Need2args  , 21 ),
    O( "in"         , "in"                    , Need2args  , 21 ),
    O( "not_in"     , "not_in"                , Need2args  , 21 ),

    //
    O( "!="         , "not_equal"             , Need2args  , 22 ),
    O( "=="         , "equal"                 , Need2args  , 22 ),
    O( ">="         , "superior_equal"        , Need2args  , 22 ),
    O( "<="         , "inferior_equal"        , Need2args  , 22 ),
    O( ">"          , "superior"              , Need2args  , 22 ),
    O( "<"          , "inferior"              , Need2args  , 22 ),

    O( "+++"        , "concatenate"           , Need2args  , 23 ),

    O( ".."         , "range"                 , Need2args  , 24 ),
    O( "..."        , "triple_dots"           , Needrarg   , 24 ),
    O( "...."       , "quadruple_dots"        , Needlarg   , 24 ),

    O( "+"          , "add"                   , Need2args  , 25 ),
    O( "-"          , "sub"                   , Needplrargs, 25 ),
    O( ""           , "neg"                   , Needrarg   , 25 ),

    O( "*"          , "mul"                   , Need2args  , 26 ),
    O( "/"          , "div"                   , Need2args  , 26 ),
    O( "//"         , "div_int"               , Need2args  , 26 ),
    O( "%"          , "mod"                   , Need2args  , 26 ),
    // O( "\\"         , "ml_div"                , Need2args  , 26 ),
    
    O( "./"         , "dot_slash"             , Needrarg   , 27 ),
    O( "../"        , "dot_dot_slash"         , Needrarg   , 27 ),

    O( "**"         , "pow"                   , Need2args  , 28 ),

    O( "~"          , "not_bitwise"           , Needrarg   , 29, true ),

    O( "'"          , "trans"                 , Needlarg   , 30 ),

    O( "--"         , "pre_dec"               , Needrarg   , 31 ),
    O( "++"         , "pre_inc"               , Needrarg   , 31 ),
    O( ""           , "post_dec"              , Needlarg   , 31 ),
    O( ""           , "post_inc"              , Needlarg   , 31 ),

    O( "@"          , "pointed_value"         , Needrarg   , 32, true ),
    O( "\\"         , "pointer_on"            , Needrarg   , 32, true ),

    // ------------------------------------------------------------
    // priority handled by special functions
    // ------------------------------------------------------------

    // O( ""          , "calc_name"             , Needrarg   , 32 ),
    O( "->"         , "get_attr_ptr"          , Need2args  , 33 ),
    O( "::"         , "doubledoubledot"       , Needplrargs, 33 ),
    O( "."          , "get_attr"              , Need2args  , 33 ),
    O( ".?"         , "get_attr_ask"          , Need2args  , 33 ),
    O( "->?"        , "get_attr_ptr_ask"      , Need2args  , 33 ),

    O( "operator"   , "__operator__"          , 0          , 34 ), // Needed rarg handled internally

    O( ""           , "__string_assembly__"   , Need2args  , 35 ),
    O( ""           , "__variable_assembly__" , Need2args  , 36 ),
    O( ""           , "tensorial_product"     , Need2args  , 37 ),

    // ------------------------------------------------------------
    // isolated variable
    // ------------------------------------------------------------

    O( "pass"       , "pass"                  , 0          , 38 ),
] as Array<Operator>;

const hash_table = [ 83,263,151,199,619,311,347,19,601,43,239,523,463,599,223,29 ];

function hash_string( s: string ): number {
    let res = 0;
    let cpt = 0;
    for( const i of s ) {
        res += i.charCodeAt( 0 ) * hash_table[ cpt % hash_table.length ];
        cpt += 1;
    }
    return res
}

// return True if hash(operators) % n appears several times
function hash_collision( n: number, ty ) {
    let l = new Set<number>();
    for( const o of operators ) {
        if ( o.of_type( ty ) ) {
            const h = hash_string( o.met_name ) % n;
            if ( l.has( h ) )
                return true;
            l.add( h );
        }
    }
    return false;
}

function make_num_operator( ty ) {
    let n = 1;
    while ( hash_collision( n, ty ) )
        n += 1;

    // 
    let lst = new Array<number>();
    for( let i = 0; i < n; ++i )
        lst.push( -1 );
    operators.forEach( ( op, cpt ) => {
        if ( op.of_type( ty ) )
            lst[ hash_string( op.met_name ) % n ] = cpt;
    } );
    //
    console.log( `inline int num_operator_${ ty }( const char *beg, int len ) {` );
    console.log( `    static const int tab[] = {${ lst.join( ',' ) }};` );
    console.log( `    int n = tab[ nstring_hash( beg, len ) % ${ n } ];` );
    console.log( "    if ( n >= 0 && operators_met_size( n ) == len && strncmp( beg, operators_met_name( n ), len ) == 0 )" );
    console.log( "        return n;" );
    console.log( "    return -1;" );
    console.log( "}" );
}

function make_nstring_hash() {
    console.log( "inline unsigned nstring_hash( const char *beg, int len ) {" );
    console.log( `    static const unsigned m[] = {${ hash_table.join( ',' ) }};` );
    console.log( "    " );
    console.log( "    unsigned res = 0;" );
    console.log( "    for( int i = 0; i < len; ++i )" );
    console.log( `        res += beg[ i ] * m[ i % ${ hash_table.length } ];` );
    console.log( "    return res;" );
    console.log( "}" );
}

function make_operator_decl() {
    let nb_groups = 0;
    operators.forEach( ( op, ind ) => {
        console.log( `#define OPERATOR_${ op.cpp_name }_NUM ${ ind }` );
        console.log( `#define GROUP_${ op.cpp_name }_NUM ${ op.group }` );
        if ( nb_groups < op.group + 1 )
            nb_groups = op.group + 1;
    } )
    console.log( `#define NB_OPERATORS ${ operators.length }` );
    console.log( `#define NB_GROUPS ${ nb_groups }` );
    console.log( `inline int group_of_operator( int type ) { static const int m[] = {${ operators.map( o => o.group ).join( ',' ) }}; return m[ type ]; }` );
}

function make_operator_name() {
    console.log( `inline const char *operators_met_name( int n ) { static const char *tab[] = {${ operators.map( op => JSON.stringify( op.met_name ) ) }}; return tab[ n ]; }` );
    console.log( `inline int         operators_met_size( int n ) { static int         tab[] = {${ operators.map( op => JSON.stringify( op.met_name.length ) ) }}; return tab[ n ]; }` );
    console.log( `inline const char *operators_cpp_name( int n ) { static const char *tab[] = {${ operators.map( op => JSON.stringify( op.cpp_tr() ) ) }}; return tab[ n ]; }` );
    console.log( `inline int         operators_behavior( int n ) { static int         tab[] = {${ operators.map( op => JSON.stringify( op.behavior ) ) }}; return tab[ n ]; }` );
    console.log( `inline bool        right_to_left_associativity( int n ) { static bool tab[] = {${ operators.map( op => JSON.stringify( op.right_to_left_associativity ) ) }}; return tab[ n ]; }` );

}
console.log( "#include <string.h>" );
make_operator_name()
make_nstring_hash ();
make_num_operator ( "le" );
make_num_operator ( "op" );
make_operator_decl();

// print >> h_file, "", len( operators.lst )

// def make_usual_string_info( h_file, c_file, usual_strings ):
//     print >> c_file, "// generated by generation.py"
//     print >> h_file, "// generated by generation.py"
//     print >> h_file, "#define NB_USUAL_STRINGS ", len( usual_strings )
    
//     cpt = 0
//     for o in usual_strings:
//         print >> h_file, "#define STRING_" + o + "_NUM " + str( cpt )
//         cpt += 1
        
//     # str usual
//     print >> h_file, "extern const char *usual_strings_str[];"
//     print >> c_file, "const char *usual_strings_str[] = {"
//     for o in usual_strings: print >> c_file, '    "' + o + '",'
//     print >> c_file, "};"
    
//     print >> h_file, "extern int usual_strings_len[];"
//     print >> c_file, "int usual_strings_len[] = {"
//     for o in usual_strings: print >> c_file, '    ' + str( len( o ) ) + ','
//     print >> c_file, "};"
    
//     print >> h_file, "extern int usual_strings_val[];"
//     print >> c_file, "int usual_strings_val[] = {"
//     for o in usual_strings: print >> c_file, '    ' + str( hash_string( o ) ) + ','
//     print >> c_file, "};"


// def make_usual_strings():
//     h_file = file( "src/Stela/System/UsualStrings.h"  , "w" )
//     c_file = file( "src/Stela/System/UsualStrings.cpp", "w" )
//     print >> c_file, "// generated by generation.py"
//     print >> c_file, "#include \"UsualStrings.h\""
//     print >> c_file, "#include \"NstringHash.h\""
//     print >> c_file, "#include <string.h>"

//     # usual_strings preparation
//     usual_strings = []
//     for o in operators.lst:
//         usual_strings.append( o.cpp_name )

//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#ifndef USUAL_STRINGS"
//     print >> h_file, "#define USUAL_STRINGS"
//     print >> h_file, "#define NB_PRIMITIVES", len( operations ) + len( base_primitives )
//     print >> h_file, "#define FIRST_PRIM_ID", len( usual_strings )
//     for s in base_primitives + operations:
//         if not ( s in usual_strings ):
//             usual_strings.append( "___" + s )
//     # print >> h_file, "inline int get_primitive_id( int num_nstring ) { return num_nstring * ( num_nstring >= " + str( len( operators.lst ) ) + " and num_nstring < " + str( len( usual_strings ) ) + " ); }"

//     for s in std_strings:
//         if not ( s in usual_strings ):
//             usual_strings.append( s )

//     # operators_met_name and cpp_name
//     def cm( met_name ):
//         if met_name == "\\":
//             return "\\\\"
//         return met_name
//     print >> h_file, "#define NB_OPERATORS", len( operators.lst )
//     print >> h_file, "const char *get_operators_met_name( int n );"
//     print >> h_file, "const char *get_operators_cpp_name( int n );"
//     print >> h_file, "int         get_operators_behavior( int n );"

//     print >> c_file, "static const char *operators_met_name[] = { " + string.join( [ '"' + cm( o.met_name ) + '"' for o in operators.lst ], "," ) + " };"
//     print >> c_file, "static const char *operators_cpp_name[] = { " + string.join( [ '"' + cm( o.cpp_name ) + '"' for o in operators.lst ], "," ) + " };"
//     print >> c_file, "static int         operators_behavior[] = { " + string.join( [      str( o.behavior )       for o in operators.lst ], "," ) + " };"
//     print >> c_file, "const char *get_operators_met_name( int n ) { return operators_met_name[ n ]; }"
//     print >> c_file, "const char *get_operators_cpp_name( int n ) { return operators_cpp_name[ n ]; }"
//     print >> c_file, "int         get_operators_behavior( int n ) { return operators_behavior[ n ]; }"

//     # 
    
//     #
//     make_usual_string_info( h_file, c_file, usual_strings )

//     # nb args needed
//     print >> h_file, "extern int usual_strings_nb_args[];"
//     print >> c_file, "int usual_strings_nb_args[] = {"
//     for o in operators.lst: print >> c_file, '    ' + str( o.nb_args() ) + ','
//     print >> c_file, "};"
    
//     print >> h_file, "#endif // USUAL_STRINGS"
    
// def make_hash_string():
//     h_file = file( "src/Stela/System/NstringHash.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#ifndef NSTRINGHASH_H"
//     print >> h_file, "#define NSTRINGHASH_H"
//     print >> h_file, ""
//     print >> h_file, "inline unsigned nstring_hash( const char *beg, int len ) {"
//     print >> h_file, "    static const unsigned m[] = { " + string.join( [ str( x ) for x in hash_table ], "," ) + " };"
//     print >> h_file, "    "
//     print >> h_file, "    unsigned res = 0;"
//     print >> h_file, "    for( int i = 0; i < len; ++i )"
//     print >> h_file, "        res += beg[ i ] * m[ i % " + str( len( hash_table ) ) + " ];"
//     print >> h_file, "    return res;"
//     print >> h_file, "}"
//     print >> h_file, ""
//     print >> h_file, "#endif // NSTRINGHASH_H"

// def make_ir_data():
//     h_file = file( "src/Stela/Ir/Decl_Base.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     for x in base_token_types:
//         print >> h_file, "DECL_IR_TOK( " + x + " )"
        
//     # unary
//     h_file = file( "src/Stela/Ir/Decl_UnaryOperations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#include \"Decl_UnaryBoolOperations.h\""
//     print >> h_file, "#include \"Decl_UnaryHomoOperations.h\""
    
//     h_file = file( "src/Stela/Ir/Decl_UnaryBoolOperations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     for x in unary_bool_operations:
//         print >> h_file, "DECL_IR_TOK( " + x + " )"
        
//     h_file = file( "src/Stela/Ir/Decl_UnaryHomoOperations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     for x in unary_homo_operations:
//         print >> h_file, "DECL_IR_TOK( " + x + " )"

//     # binary
//     h_file = file( "src/Stela/Ir/Decl_BinaryOperations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#include \"Decl_BinaryBoolOperations.h\""
//     print >> h_file, "#include \"Decl_BinaryHomoOperations.h\""
    
//     h_file = file( "src/Stela/Ir/Decl_BinaryBoolOperations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     for x in binary_bool_operations:
//         print >> h_file, "DECL_IR_TOK( " + x + " )"
        
//     h_file = file( "src/Stela/Ir/Decl_BinaryHomoOperations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     for x in binary_homo_operations:
//         print >> h_file, "DECL_IR_TOK( " + x + " )"

//     # all op        
//     h_file = file( "src/Stela/Ir/Decl_Operations.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#include \"Decl_UnaryOperations.h\""
//     print >> h_file, "#include \"Decl_BinaryOperations.h\""

//     h_file = file( "src/Stela/Ir/Decl_Primitives.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     for x in base_primitives:
//         print >> h_file, "DECL_IR_TOK( " + x + " )"

//     h_file = file( "src/Stela/Ir/Decl.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#include \"Decl_Base.h\""
//     print >> h_file, "#include \"Decl_Primitives.h\""
//     print >> h_file, "#include \"Decl_UnaryOperations.h\""
//     print >> h_file, "#include \"Decl_BinaryOperations.h\""

//     # numberq
//     h_file = file( "src/Stela/Ir/Numbers.h", "w" )
//     print >> h_file, "// generated by bin/generation.py"
//     print >> h_file, "#include \"../System/TypeConfig.h\""
//     cpt = 0
//     for x in base_token_types + base_primitives + operations:
//         print >> h_file, "#define IR_TOK_" + x + "  PI8( " + str( cpt ) + " )"
//         cpt += 1

// def make_primitive_inc():
//     pass
//     #h_file = file( "src/Stela/VirtualMachines/VirtualMachine/Defines/Primitives.h", "w" )
//     #print >> h_file, "// generated by bin/generation.py"
//     #for x in base_primitives:
//         #print >> h_file, "DECL_PRIM(", x, ")"
//     #print >> h_file, '#define DECL_OP( X ) DECL_PRIM( X )'
//     #print >> h_file, '#include "../../../Operations/Defines/All.h"'
//     #print >> h_file, '#undef DECL_OP'

//     #u_file = file( "src/Stela/Operations/Defines/Unary.h", "w" )
//     #print >> u_file, "// generated by bin/generation.py"
//     #for x in unary_operations:
//         #print >> u_file, "DECL_OP(", x, ")"

//     ##print >> u_file, "#define DECL_BT( TT ) DECL_OP( Conv_##TT )"
//     ##print >> u_file, "#include \"../../PrimTypes/Defines/Arithmetic.h\""
//     ##print >> u_file, "#undef DECL_BT"

//     #u_file = file( "src/Stela/Operations/Defines/Binary.h", "w" )
//     #print >> u_file, "// generated by bin/generation.py"
//     #for x in binary_operations:
//         #print >> u_file, "DECL_OP(", x, ")"

// # ---------------------------------------------------------------------------
// operators = Operators()

// unary_bool_operations = string.split( """
//     not_boolean
// """ )

// unary_homo_operations = string.split( """
//     log
//     ceil
// """ )

// unary_operations = unary_bool_operations + unary_homo_operations

// # conv

// binary_bool_operations = string.split( """
//     or_boolean
//     and_boolean

//     equ
//     neq
//     sup
//     inf
//     sup_eq
//     inf_eq
// """ )
// binary_homo_operations = string.split( """
//     add
//     sub
//     mul
//     div
//     pow
//     mod
//     shift_left
// """ )
// binary_operations = binary_bool_operations + binary_homo_operations


// operations = unary_operations + binary_operations

// base_primitives = string.split( """
//     info
//     disp
//     rand
//     syscall
//     set_base_size_and_alig
//     set_RawRef_dependancy
//     reassign_rec
//     assign_rec
//     init_va
//     set_ptr_val
//     select_SurdefList
//     select_Varargs
//     ptr_size
//     ptr_alig
//     sizeof
//     aligof
//     typeof
//     address
//     get_slice
//     pointed_value
//     pointer_on
//     block_exec
//     get_argc
//     get_argv
//     apply_LambdaFunc
//     inst_of
//     repeat
//     code
//     repeated_type
//     slice
//     call
//     call_block
//     get_size
//     make_code_for
// """ ) #

// std_strings = string.split( """
//     break continue abstract
//     SourceFile Callable Class Def ClassInst DefInst SurdefList
//     GetSetSopDef GetSetSopInst Ptr Block getr_ptr self_ptr
//     Bool Void Error FreeSlot self this
//     SI8 SI16 SI32 SI64 PI8 PI16 PI32 PI64 FP32 FP64 FP80
//     PermanentString true false select chabeha
//     varargs init delete Vec Nstring UntypedPtr Expr
//     BegItemDataRefArray
//     EndItemDataRefArray
//     BegItemSizeRefArray
//     EndItemSizeRefArray
//     RefArray

//     VarargsItemBeg
//     VarargsItemEnd

//     BegSurdefsItem
//     EndSurdefsItem
//     Type
//     apply
//     super
//     super_0 super_1 super_2 super_3
//     super_4 super_5 super_6 super_7
//     destroy
//     LambdaFunc
//     get_size
//     name
//     ext_refs
//     RawPtr
//     RawRef
//     BaseInt
//     null_ref
// """ )

// base_token_types = string.split( """
//     BLOCK
//     DEF
//     CLASS
//     RETURN
//     APPLY
//     SELECT
//     CHBEBA
//     SI32
//     PI32
//     SI64
//     PI64
//     PTR
//     STRING
//     VAR
//     ASSIGN
//     GET_ATTR
//     GET_ATTR_PTR
//     GET_ATTR_ASK
//     GET_ATTR_PTR_ASK
//     GET_ATTR_PA
//     IF
//     WHILE
//     BREAK
//     CONTINUE
//     FALSE
//     TRUE
//     VOID
//     SELF
//     THIS
//     FOR
//     IMPORT
//     NEW
//     LIST
//     LAMBDA
//     AND
//     OR
// """ )

// make_hash_string()
// make_usual_strings()
// make_ir_data()
// make_primitive_inc()




 
