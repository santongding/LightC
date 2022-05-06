//
// Created by os on 5/5/22.
//

#ifndef LIGHTC_CPPBUILTIN_H
#define LIGHTC_CPPBUILTIN_H

#include "def.h"
#include "TypeInfo.hpp"

extern "C" {
vtype __LIGHTC_LOCATE(vtype oid, vtype mid);
vtype __LIGHTC_NEW(vtype tid);
void __LIGHTC_BIND(vtype v, vtype oid, vtype mid);
void __LIGHTC_BEFORE_CALL();
void __LIGHTC_RET(vtype v, bool isRefRet);
};


#endif //LIGHTC_CPPBUILTIN_H

