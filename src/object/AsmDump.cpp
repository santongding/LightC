//
// Created by os on 5/3/22.
//

#include "AsmDump.hpp"
#include "AsmDef.hpp"

auto arg_regs_num = Args_Reg_Num;
auto callee_regs = Get_Callee_REG_NAME();
auto caller_regs = Get_Caller_REG_NAME();
auto ra_name =Get_RA_REG_NAME();
auto sp_name = Get_SP_REG_NAME();
auto z_name = Get_ZERO_REG_NAME();
auto asmMap = GetAsmTypeMap();


string AsmOpValue::Dump(AsmValueType target) {
    switch (type) {
        case None:{

            assert(target == NONE);
            return "";
        }
            break;
        case Label:{

            assert(target == STRING);
            assert(label != "");
            return label;
        }
            break;
        case Imm:{

            assert(target == IMM||target==REG_OR_IMM);
            return std::to_string(value);
        }
            break;
        case RA:{

            assert(target == REG||target==REG_OR_IMM);
            assert(value == 0);
            return ra_name;
        }
            break;
        case SP:{

            assert(target == REG||target==REG_OR_IMM);
            assert(value == 0);
            return sp_name;
        }
            break;
        case Zero:{

            assert(target == REG||target==REG_OR_IMM);
            assert(value == 0);
            return z_name;
        }
            break;
        case CallerSaved:{

            assert(target == REG||target==REG_OR_IMM);
            assert(value < caller_regs.size());
            return caller_regs[value];
        }
            break;
        case CalleeSaved:{

            assert(target == REG||target==REG_OR_IMM);
            assert(value < callee_regs.size());
            return callee_regs[value];
        }
            break;

        default:
            assert(false);
            break;
    }
}

string AsmCode::Dump() {
    assert(asmMap.find(type) != asmMap.end());
    auto asminfo = asmMap[type];

    const static string tps[] = {"[0]", "[1]", "[2]"};
    for (int i = 0; i < 3; i++) {
        auto str = v[i].Dump(asminfo.t[i]);
        int bg = 0;
        while (true) {
            auto pos = asminfo.asmcode.find(tps[i], bg);
            if (pos >= asminfo.asmcode.length()) {
                break;
            }
            assert(str != "");
            bg = pos + str.length();
            asminfo.asmcode.replace(pos, tps[i].length(), str);

        }
        if(str!=""){
            assert(bg);
        }else{
            assert(!bg);
        }
    }
    return asminfo.asmcode;
}