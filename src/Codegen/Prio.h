#pragma once

enum {
    //Group 1 precedence, no associativity
    PRIO_Scope_resolution                =  1, // ::
    //Group 2 precedence, left to right associativity
    PRIO_Member_selection                =  2, // . or –>
    PRIO_Array_subscript                 =  2, // [ ]
    PRIO_Function_call                   =  2, // ( )
    PRIO_Postfix_increment               =  2, // ++
    PRIO_Postfix_decrement               =  2, // --
    PRIO_Type_name                       =  2, // typeid( )
    PRIO_Constant_type_conversion        =  2, // const_cast
    PRIO_Dynamic_type_conversion         =  2, // dynamic_cast
    PRIO_Reinterpreted_type_conversion   =  2, // reinterpret_cast
    PRIO_Static_type_conversion          =  2, // static_cast
    // Group 3 precedence, right to left associativity
    PRIO_Size_of_object_or_type          =  3, // sizeof
    PRIO_Prefix_increment                =  3, // ++
    PRIO_Prefix_decrement                =  3, // --
    PRIO_One_s_complement                =  3, // ~
    PRIO_Logical_not                     =  3, // !
    PRIO_Unary_negation                  =  3, // -
    PRIO_Unary_plus                      =  3, // +
    PRIO_Address_of                      =  3, // &
    PRIO_Indirection                     =  3, // *
    PRIO_Create_object                   =  3, // new
    PRIO_Destroy_object                  =  3, // delete
    PRIO_Cast                            =  3, // ()
    // Group 4 precedence, left to right associativity
    PRIO_Pointer_to_member               =  4, // .* or –>*
    // Group 5 precedence, left to right associativity
    PRIO_Multiplication                  =  5, // *
    PRIO_Division                        =  5, // /
    PRIO_Modulus                         =  5, // %
    // Group 6 precedence, left to right associativity
    PRIO_Addition                        =  6, // +
    PRIO_Subtraction                     =  6, // –
    // Group 7 precedence, left to right associativity
    PRIO_Left_shift                      =  7, // <<
    PRIO_Right_shift                     =  7, // >>
    // Group 8 precedence, left to right associativity
    PRIO_Less_than                       =  8, // <
    PRIO_Greater_than                    =  8, // >
    PRIO_Less_than_or_equal_to           =  8, // <=
    PRIO_Greater_than_or_equal_to        =  8, // >=
    // Group 9 precedence, left to right associativity
    PRIO_Equality                        =  9, // ==
    PRIO_Inequality                      =  9, // !=
    // Group 10 precedence left to right associativity
    PRIO_Bitwise_AND                     = 10, // &
    // Group 11 precedence, left to right associativity
    PRIO_Bitwise_exclusive_OR            = 11, // ^
    // Group 12 precedence, left to right associativity
    PRIO_Bitwise_inclusive_OR            = 12, // |
    // Group 13 precedence, left to right associativity
    PRIO_Logical_AND                     = 13, // &&
    // Group 14 precedence, left to right associativity
    PRIO_Logical_OR                      = 14, // ||
    // Group 15 precedence, right to left associativity
    PRIO_Conditional                     = 15, // ? :
    // Group 16 precedence, right to left associativity
    PRIO_Assignment                      = 16, // =
    PRIO_Multiplication_assignment       = 16, // *=
    PRIO_Division_assignment             = 16, // /=
    PRIO_Modulus_assignment              = 16, // %=
    PRIO_Addition_assignment             = 16, // +=
    PRIO_Subtraction_assignment          = 16, // –=
    PRIO_Left_shift_assignment           = 16, // <<=
    PRIO_Right_shift_assignment          = 16, // >>=
    PRIO_Bitwise_AND_assignment          = 16, // &=
    PRIO_Bitwise_inclusive_OR_assignment = 16, // &#124;=
    PRIO_Bitwise_exclusive_OR_assignment = 16, // ^=
    // Group 17 precedence, right to left associativity
    PRIO_throw_expression                = 17, // throw
    // Group 18 precedence, left to right associativity
    PRIO_Comma                           = 18, // ,

    //
    PRIO_paren                           = 19, //
};

