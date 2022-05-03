//
// Created by os on 5/3/22.
//

#ifndef LIGHTC_OBJECT_H
#define LIGHTC_OBJECT_H

#include "vector"
#include "cassert"
#include "../asm_include/arm64/AsmDef.hpp"
#include "string"

using std::vector;

vector<AsmCode> object_generate();

void object_dump(const vector<AsmCode> codes);

#endif //LIGHTC_OBJECT_H
