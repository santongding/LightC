//
// Created by os on 5/3/22.
//

#include "AsmDump.hpp"

int main() {
    vector<AsmCode> codes = {
            AsmCode(JR, AsmOpValue("!@!#@"))
    };
    for (auto &c: codes) {
        c.Dump();
    }
}