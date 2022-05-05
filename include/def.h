//
// Created by os on 4/29/22.
//

#ifndef LIGHTC_DEF_H
#define LIGHTC_DEF_H


const std::string TEMP_VALUE_PREFIX="#t";
const std::string MEM_FUNC_PREFIX="__LIGHTC_FUNC_";
const std::string LOCATE_FUNC = "__LIGHTC_LOCATE";

const std::string NEW_FUNC = "__LIGHTC_NEW";
const std::string BIND_FUNC = "__LIGHTC_BIND";

const std::string ASM_SOURCE_NAME =".LIGHTC_ASM.s";

enum VALUE_TYPE {
    INVALID_V = 0,
    LINK_V = 1,
    INT_V = 2,
    REF_V = 3,
    ANY_V = 4,
};


#endif //LIGHTC_DEF_H
