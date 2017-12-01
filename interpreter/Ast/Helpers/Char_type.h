#ifndef CHAR_TYPE_H
#define CHAR_TYPE_H

inline unsigned char char_type(char c) {
    static const unsigned char type_char_[] = {
        11, 0, 0, 0, 0, 0, 0, 0,
        0, 3, 1, 0, 0, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 9, 4, 4, 4,
        7, 7, 4, 4, 4, 4, 4, 4,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 4, 4, 4, 4, 4, 4,
        4, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 7, 4, 7, 4, 9,
        10, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 7, 4, 7, 4, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    return type_char_[ unsigned(c) ];
};

#define CHAR_TYPE_comment 6
#define CHAR_TYPE_dos_makes_me_sick 2
#define CHAR_TYPE_end 11
#define CHAR_TYPE_string 5
#define CHAR_TYPE_space 3
#define CHAR_TYPE_unknown 0
#define CHAR_TYPE_number 8
#define CHAR_TYPE_letter 9
#define CHAR_TYPE_parenthesis 7
#define CHAR_TYPE_operator 4
#define CHAR_TYPE_cr 1
#define CHAR_TYPE_ccode 10

#endif // CHAR_TYPE_H
