//
// Created by os on 4/30/22.
//

#ifndef LIGHTC_TYPEMANAGER_H
#define LIGHTC_TYPEMANAGER_H

#include <functional>
#include "string"
#include "set"
#include "unordered_map"
#include "tac.hpp"
#include "vector"
#include "def.h"
#include "utils.hpp"
#include "TypeInfo.hpp"

using std::string;
using std::set;
using std::pair;
using std::vector;

class TypeManager {
public:
    TypeManager();

    void Init();

    void RecordIdentifier(const string &s);

    STATUS TryDecodeType(const string &ts, TypeInfo &type);

    STATUS DeclareClass(const string &name);

    STATUS DeclareMember(const string &name, const string &mem, const string &type);

    STATUS DeclareFunc(const string &name, const string &funcname, const TAC *func);

    void CastType(SYM *from, SYM *to, bool isInt);

    void CastType(SYM *from, const TypeInfo &t);

    void CheckTac(SYM *caller, SYM *callee, TAC *tac, std::function<SYM *(SYM *)> &get_sym_type);

    void Print();


private:

    IdentifierMap idMap;
    std::unordered_map<int, ClassInfo *> classes;

};


#endif //LIGHTC_TYPEMANAGER_H
