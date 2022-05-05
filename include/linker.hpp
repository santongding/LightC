//
// Created by os on 5/5/22.
//

#ifndef LIGHTC_LINKER_HPP
#define LIGHTC_LINKER_HPP

#include "TypeManager.h"

void dumpBuiltin(TypeManager *mgr,const string & builtinOutput);

void link2Exe(const string & asmpath);

#endif //LIGHTC_LINKER_HPP
