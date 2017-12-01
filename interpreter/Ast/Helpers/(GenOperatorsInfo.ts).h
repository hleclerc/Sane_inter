#include <string.h>
inline const char *operators_met_name( int n ) { static const char *tab[] = {";","try","catch","static","global","export","mixin","const","virtual","override","inline","abstract","import","from","class","def","enum","extends","pertinence","when","with","~>","wpc","init_of","while","for","if","else","=",":=","@=","~=","|=","^=","&=","||=","^^=","&&=","%=","<<=",">>=","/=","*=","-=","+=","//=","+++=","ref",",","=>","?",":",":.","<<<<","<<<","<<",">>","or","||","and","&&","xor","^^","not","!","|","^","&","is_a","is_not_a","in","not_in","!=","==",">=","<=",">","<","+++","..","...","....","+","-","","*","/","//","%","./","../","**","~","'","--","++","","","@","\\","->","::",".",".?","->?","operator","","","","pass"}; return tab[ n ]; }
inline int         operators_met_size( int n ) { static int         tab[] = {1,3,5,6,6,6,5,5,7,8,6,8,6,4,5,3,4,7,10,4,4,2,3,7,5,3,2,4,1,2,2,2,2,2,2,3,3,3,2,3,3,2,2,2,2,3,4,3,1,2,1,1,2,4,3,2,2,2,2,3,2,3,2,3,1,1,1,1,4,8,2,6,2,2,2,2,1,1,3,2,3,4,1,1,0,1,1,2,1,2,3,2,1,1,2,2,0,0,1,1,2,2,1,2,3,8,0,0,0,4}; return tab[ n ]; }
inline const char *operators_cpp_name( int n ) { static const char *tab[] = {"operator ;","__try__","__catch__","__static__","__global__","__export__","__mixin__","__const__","__virtual__","__override__","__inline__","__abstract__","__import__","__from__","__class__","__def__","__enum__","__extends__","__pertinence__","__when__","__with__","operator ~>","__wpc__","__init_of__","__while__","__for__","__if__","__else__","operator =","operator :=","operator @=","operator ~=","operator |=","operator ^=","operator &=","operator ||=","operator ^^=","operator &&=","operator %=","operator <<=","operator >>=","operator /=","operator *=","operator -=","operator +=","operator //=","operator +++=","__ref__","operator ,","operator =>","operator ?","operator :","operator :.","operator <<<<","operator <<<","operator <<","operator >>","or_boolean__","operator ||","and_boolean__","operator &&","xor_boolean___","operator ^^","not_boolean","operator !","operator |","operator ^","operator &","is_a","is_not_a","in","not_in","operator !=","operator ==","operator >=","operator <=","operator >","operator <","operator +++","operator ..","operator ...","operator ....","operator +","operator -","neg","operator *","operator /","operator //","operator %","operator ./","operator ../","operator **","operator ~","operator '","operator --","operator ++","post_dec","post_inc","operator @","operator \\","operator ->","operator ::","operator .","operator .?","operator ->?","__operator__","__string_assembly__","__variable_assembly__","tensorial_product","pass"}; return tab[ n ]; }
inline int         operators_behavior( int n ) { static int         tab[] = {0,16,16,2,2,2,2,2,2,2,2,2,2,2,16,16,16,3,3,3,3,3,2,2,16,16,16,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,6,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,3,6,2,3,3,3,3,2,2,3,2,1,2,2,1,1,2,2,3,6,3,3,3,0,3,3,3,0}; return tab[ n ]; }
inline bool        right_to_left_associativity( int n ) { static bool tab[] = {false,true,true,true,true,true,true,true,true,true,true,true,true,true,false,false,false,false,false,false,false,false,false,false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false}; return tab[ n ]; }
inline unsigned nstring_hash( const char *beg, int len ) {
    static const unsigned m[] = {83,263,151,199,619,311,347,19,601,43,239,523,463,599,223,29};
    
    unsigned res = 0;
    for( int i = 0; i < len; ++i )
        res += beg[ i ] * m[ i % 16 ];
    return res;
}
inline int num_operator_le( const char *beg, int len ) {
    static const int tab[] = {-1,-1,-1,-1,-1,17,-1,24,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,22,47,20,-1,-1,-1,105,-1,-1,57,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,-1,-1,-1,-1,-1,-1,26,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,59,-1,61,-1,-1,-1,-1,-1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,23,15,-1,-1,-1,-1,-1,-1,-1,-1,14,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,-1,-1,-1,70,-1,-1,-1,-1,19,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,-1,-1,-1,-1,-1,-1,-1,10,-1,-1,-1,-1,-1,-1,-1,71,-1,-1,-1,-1,-1,63,-1,1,5,6,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,-1,16,-1,-1,-1,-1,-1,-1,-1,4,69,-1,-1,-1,-1,-1,-1,-1,68,109,-1,-1,-1,-1,-1,-1,13,7,-1,27,-1,25,-1,-1};
    int n = tab[ nstring_hash( beg, len ) % 204 ];
    if ( n >= 0 && operators_met_size( n ) == len && strncmp( beg, operators_met_name( n ), len ) == 0 )
        return n;
    return -1;
}
inline int num_operator_op( const char *beg, int len ) {
    static const int tab[] = {-1,-1,77,33,-1,-1,-1,-1,-1,40,67,-1,-1,-1,-1,-1,-1,-1,87,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,41,-1,-1,-1,62,-1,99,31,21,-1,-1,-1,-1,-1,-1,-1,60,-1,-1,-1,81,-1,-1,-1,-1,-1,-1,45,-1,-1,55,75,-1,-1,-1,83,80,-1,-1,34,98,-1,65,-1,-1,36,-1,-1,85,-1,-1,-1,-1,28,-1,-1,-1,37,-1,-1,-1,93,-1,-1,-1,-1,51,-1,-1,-1,53,39,-1,-1,-1,-1,-1,-1,-1,-1,94,-1,-1,-1,-1,-1,-1,64,-1,-1,91,-1,-1,-1,-1,-1,43,100,-1,-1,-1,30,-1,32,-1,-1,-1,-1,-1,42,-1,-1,52,-1,73,49,-1,78,102,-1,-1,-1,-1,-1,101,-1,-1,29,-1,-1,-1,82,-1,-1,-1,-1,76,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,72,0,-1,-1,-1,-1,-1,-1,-1,88,-1,-1,-1,-1,-1,-1,79,89,58,-1,-1,-1,-1,-1,66,-1,95,-1,-1,-1,104,-1,-1,103,54,-1,-1,-1,-1,90,-1,-1,-1,-1,44,-1,-1,-1,-1,74,56,-1,-1,86,-1,-1,46,-1,-1,35,92,-1,-1,-1,-1,-1,48,-1,-1,-1,38,50,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int n = tab[ nstring_hash( beg, len ) % 262 ];
    if ( n >= 0 && operators_met_size( n ) == len && strncmp( beg, operators_met_name( n ), len ) == 0 )
        return n;
    return -1;
}
#define OPERATOR_semicolon_NUM 0
#define GROUP_semicolon_NUM -1
#define OPERATOR___try___NUM 1
#define GROUP___try___NUM 0
#define OPERATOR___catch___NUM 2
#define GROUP___catch___NUM 0
#define OPERATOR___static___NUM 3
#define GROUP___static___NUM 0
#define OPERATOR___global___NUM 4
#define GROUP___global___NUM 0
#define OPERATOR___export___NUM 5
#define GROUP___export___NUM 0
#define OPERATOR___mixin___NUM 6
#define GROUP___mixin___NUM 0
#define OPERATOR___const___NUM 7
#define GROUP___const___NUM 0
#define OPERATOR___virtual___NUM 8
#define GROUP___virtual___NUM 0
#define OPERATOR___override___NUM 9
#define GROUP___override___NUM 0
#define OPERATOR___inline___NUM 10
#define GROUP___inline___NUM 0
#define OPERATOR___abstract___NUM 11
#define GROUP___abstract___NUM 0
#define OPERATOR___import___NUM 12
#define GROUP___import___NUM 0
#define OPERATOR___from___NUM 13
#define GROUP___from___NUM 0
#define OPERATOR___class___NUM 14
#define GROUP___class___NUM 1
#define OPERATOR___def___NUM 15
#define GROUP___def___NUM 1
#define OPERATOR___enum___NUM 16
#define GROUP___enum___NUM 1
#define OPERATOR___extends___NUM 17
#define GROUP___extends___NUM 2
#define OPERATOR___pertinence___NUM 18
#define GROUP___pertinence___NUM 2
#define OPERATOR___when___NUM 19
#define GROUP___when___NUM 2
#define OPERATOR___with___NUM 20
#define GROUP___with___NUM 2
#define OPERATOR_ret_type_NUM 21
#define GROUP_ret_type_NUM 2
#define OPERATOR___wpc___NUM 22
#define GROUP___wpc___NUM 2
#define OPERATOR___init_of___NUM 23
#define GROUP___init_of___NUM 2
#define OPERATOR___while___NUM 24
#define GROUP___while___NUM 5
#define OPERATOR___for___NUM 25
#define GROUP___for___NUM 5
#define OPERATOR___if___NUM 26
#define GROUP___if___NUM 5
#define OPERATOR___else___NUM 27
#define GROUP___else___NUM 5
#define OPERATOR_reassign_NUM 28
#define GROUP_reassign_NUM 5
#define OPERATOR_assign_NUM 29
#define GROUP_assign_NUM 5
#define OPERATOR_assign_ref_NUM 30
#define GROUP_assign_ref_NUM 5
#define OPERATOR_assign_type_NUM 31
#define GROUP_assign_type_NUM 5
#define OPERATOR_self_or_bitwise_NUM 32
#define GROUP_self_or_bitwise_NUM 5
#define OPERATOR_self_xor_bitwise_NUM 33
#define GROUP_self_xor_bitwise_NUM 5
#define OPERATOR_self_and_bitwise_NUM 34
#define GROUP_self_and_bitwise_NUM 5
#define OPERATOR_self_or_boolean_NUM 35
#define GROUP_self_or_boolean_NUM 5
#define OPERATOR_self_xor_boolean_NUM 36
#define GROUP_self_xor_boolean_NUM 5
#define OPERATOR_self_and_boolean_NUM 37
#define GROUP_self_and_boolean_NUM 5
#define OPERATOR_self_mod_NUM 38
#define GROUP_self_mod_NUM 5
#define OPERATOR_self_shift_left_NUM 39
#define GROUP_self_shift_left_NUM 5
#define OPERATOR_self_shift_right_NUM 40
#define GROUP_self_shift_right_NUM 5
#define OPERATOR_self_div_NUM 41
#define GROUP_self_div_NUM 5
#define OPERATOR_self_mul_NUM 42
#define GROUP_self_mul_NUM 5
#define OPERATOR_self_sub_NUM 43
#define GROUP_self_sub_NUM 5
#define OPERATOR_self_add_NUM 44
#define GROUP_self_add_NUM 5
#define OPERATOR_self_div_int_NUM 45
#define GROUP_self_div_int_NUM 5
#define OPERATOR_self_concatenate_NUM 46
#define GROUP_self_concatenate_NUM 5
#define OPERATOR___ref___NUM 47
#define GROUP___ref___NUM 5
#define OPERATOR_comma_NUM 48
#define GROUP_comma_NUM 8
#define OPERATOR_double_arrow_NUM 49
#define GROUP_double_arrow_NUM 11
#define OPERATOR___alternative___NUM 50
#define GROUP___alternative___NUM 11
#define OPERATOR_doubledot_NUM 51
#define GROUP_doubledot_NUM 11
#define OPERATOR_doubledotdot_NUM 52
#define GROUP_doubledotdot_NUM 11
#define OPERATOR_shift_left_long_str_NUM 53
#define GROUP_shift_left_long_str_NUM 13
#define OPERATOR_shift_left_then_endl_NUM 54
#define GROUP_shift_left_then_endl_NUM 13
#define OPERATOR_shift_left_NUM 55
#define GROUP_shift_left_NUM 13
#define OPERATOR_shift_right_NUM 56
#define GROUP_shift_right_NUM 13
#define OPERATOR_or_boolean___NUM 57
#define GROUP_or_boolean___NUM 14
#define OPERATOR_or_boolean_sgn___NUM 58
#define GROUP_or_boolean_sgn___NUM 14
#define OPERATOR_and_boolean___NUM 59
#define GROUP_and_boolean___NUM 15
#define OPERATOR_and_boolean_sgn___NUM 60
#define GROUP_and_boolean_sgn___NUM 15
#define OPERATOR_xor_boolean____NUM 61
#define GROUP_xor_boolean____NUM 16
#define OPERATOR_xor_boolean_sgn____NUM 62
#define GROUP_xor_boolean_sgn____NUM 16
#define OPERATOR_not_boolean_NUM 63
#define GROUP_not_boolean_NUM 17
#define OPERATOR_not_boolean_sgn_NUM 64
#define GROUP_not_boolean_sgn_NUM 17
#define OPERATOR_or_bitwise_NUM 65
#define GROUP_or_bitwise_NUM 18
#define OPERATOR_xor_bitwise_NUM 66
#define GROUP_xor_bitwise_NUM 19
#define OPERATOR_and_bitwise_NUM 67
#define GROUP_and_bitwise_NUM 20
#define OPERATOR_is_a_NUM 68
#define GROUP_is_a_NUM 21
#define OPERATOR_is_not_a_NUM 69
#define GROUP_is_not_a_NUM 21
#define OPERATOR_in_NUM 70
#define GROUP_in_NUM 21
#define OPERATOR_not_in_NUM 71
#define GROUP_not_in_NUM 21
#define OPERATOR_not_equal_NUM 72
#define GROUP_not_equal_NUM 22
#define OPERATOR_equal_NUM 73
#define GROUP_equal_NUM 22
#define OPERATOR_superior_equal_NUM 74
#define GROUP_superior_equal_NUM 22
#define OPERATOR_inferior_equal_NUM 75
#define GROUP_inferior_equal_NUM 22
#define OPERATOR_superior_NUM 76
#define GROUP_superior_NUM 22
#define OPERATOR_inferior_NUM 77
#define GROUP_inferior_NUM 22
#define OPERATOR_concatenate_NUM 78
#define GROUP_concatenate_NUM 23
#define OPERATOR_range_NUM 79
#define GROUP_range_NUM 24
#define OPERATOR_triple_dots_NUM 80
#define GROUP_triple_dots_NUM 24
#define OPERATOR_quadruple_dots_NUM 81
#define GROUP_quadruple_dots_NUM 24
#define OPERATOR_add_NUM 82
#define GROUP_add_NUM 25
#define OPERATOR_sub_NUM 83
#define GROUP_sub_NUM 25
#define OPERATOR_neg_NUM 84
#define GROUP_neg_NUM 25
#define OPERATOR_mul_NUM 85
#define GROUP_mul_NUM 26
#define OPERATOR_div_NUM 86
#define GROUP_div_NUM 26
#define OPERATOR_div_int_NUM 87
#define GROUP_div_int_NUM 26
#define OPERATOR_mod_NUM 88
#define GROUP_mod_NUM 26
#define OPERATOR_dot_slash_NUM 89
#define GROUP_dot_slash_NUM 27
#define OPERATOR_dot_dot_slash_NUM 90
#define GROUP_dot_dot_slash_NUM 27
#define OPERATOR_pow_NUM 91
#define GROUP_pow_NUM 28
#define OPERATOR_not_bitwise_NUM 92
#define GROUP_not_bitwise_NUM 29
#define OPERATOR_trans_NUM 93
#define GROUP_trans_NUM 30
#define OPERATOR_pre_dec_NUM 94
#define GROUP_pre_dec_NUM 31
#define OPERATOR_pre_inc_NUM 95
#define GROUP_pre_inc_NUM 31
#define OPERATOR_post_dec_NUM 96
#define GROUP_post_dec_NUM 31
#define OPERATOR_post_inc_NUM 97
#define GROUP_post_inc_NUM 31
#define OPERATOR_pointed_value_NUM 98
#define GROUP_pointed_value_NUM 32
#define OPERATOR_pointer_on_NUM 99
#define GROUP_pointer_on_NUM 32
#define OPERATOR_get_attr_ptr_NUM 100
#define GROUP_get_attr_ptr_NUM 33
#define OPERATOR_doubledoubledot_NUM 101
#define GROUP_doubledoubledot_NUM 33
#define OPERATOR_get_attr_NUM 102
#define GROUP_get_attr_NUM 33
#define OPERATOR_get_attr_ask_NUM 103
#define GROUP_get_attr_ask_NUM 33
#define OPERATOR_get_attr_ptr_ask_NUM 104
#define GROUP_get_attr_ptr_ask_NUM 33
#define OPERATOR___operator___NUM 105
#define GROUP___operator___NUM 34
#define OPERATOR___string_assembly___NUM 106
#define GROUP___string_assembly___NUM 35
#define OPERATOR___variable_assembly___NUM 107
#define GROUP___variable_assembly___NUM 36
#define OPERATOR_tensorial_product_NUM 108
#define GROUP_tensorial_product_NUM 37
#define OPERATOR_pass_NUM 109
#define GROUP_pass_NUM 38
#define NB_OPERATORS 110
#define NB_GROUPS 39
inline int group_of_operator( int type ) { static const int m[] = {-1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,8,11,11,11,11,13,13,13,13,14,14,15,15,16,16,17,17,18,19,20,21,21,21,21,22,22,22,22,22,22,23,24,24,24,25,25,25,26,26,26,26,27,27,28,29,30,31,31,31,31,32,32,33,33,33,33,33,34,35,36,37,38}; return m[ type ]; }
