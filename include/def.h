//
// Created by os on 4/29/22.
//

#ifndef LIGHTC_DEF_H
#define LIGHTC_DEF_H

enum STATUS {
    OK,
    SYMBOL_REPEAT,
    SYMBOL_UNDEFINED,
    TYPE_CONFLICT
};
enum VALUE_TYPE {

    LINK_V = 1,
    INT_V = 2,
    REF_V = 3,
    ANY_V = 4,
};


#endif //LIGHTC_DEF_H
