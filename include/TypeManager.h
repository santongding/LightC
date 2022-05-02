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



using std::string;
using std::set;
using std::pair;
using std::vector;

class IdentifierMap {
private:
    std::unordered_map<string, int> id2num;
    vector<string> identifierMap;

    bool inited = false;
public:
    void RecordIdentifier(const string &s);

    string nti(int id);

    int itn(const string &s);

    void Init();
};


class TypeInfo {

public:
    TypeInfo() = default;

    TypeInfo(VALUE_TYPE t, int n);

    TypeInfo(VALUE_TYPE t);


    template<VALUE_TYPE T>
    bool Is() const;

    int GetTypeName() const {
        assert(type_name);
        assert(Is<LINK_V>() || Is<REF_V>());
        return type_name;
    }

    STATUS Cast(const TypeInfo &t) {
        if (t.Is<ANY_V>()) {
            return TYPE_UNKNOWN;
        }
        if (Is<ANY_V>()) {
            assert(!type_name);
            type = t.type;
            type_name = t.type_name;
        } else {
            if (type != t.type || type_name != t.type_name) {
                return TYPE_CONFLICT;
            }
        }
        return OK;
    }


    string Format(IdentifierMap *idMap);

    string ToStr(IdentifierMap *idMap) const {
        string ans;
        if (type == INT_V) {
            ans += "int|";
        } else if (type == LINK_V) {
            ans += "link|";
        } else if (type == REF_V) {
            ans += "ref|";
        } else {
            CheckStatus(TYPE_UNKNOWN);
        }
        if (type_name)ans += idMap->nti(type_name), assert(type != INT_V);
        return ans;
    }

private:
    VALUE_TYPE type;
    int type_name;

};

class FuncInfo {
public:
    FuncInfo() = default;

    string Format(IdentifierMap *idMap);

    FuncInfo(TypeInfo ret, vector<pair<TypeInfo, int>> formals) : retType(ret), argsType(formals) {};

    TypeInfo GetRetType() const {
        return retType;
    }

    const vector<pair<TypeInfo, int>> &GetArgsType() const {
        return argsType;
    }

private:
    TypeInfo retType;
    vector<pair<TypeInfo, int>> argsType;
};

class ClassInfo {
public:
    ClassInfo(const int name);

    STATUS DeclareMember(const int name, const TypeInfo &type);

    STATUS DeclareFunc(const int name, TypeInfo ret, vector<pair<TypeInfo, int>> ts);


    string Format(IdentifierMap *idMap);

    TypeInfo GetMemType(int id) {
        if (membersType.find(id) == membersType.end()) {
            CheckStatus(CLASS_MEMBER_NOT_EXIST);
        } else {
            return membersType[id];
        }
    }

    FuncInfo GetFunc(int id) {
        if (funcs.find(id) == funcs.end()) {
            CheckStatus(CLASS_MEMBER_NOT_EXIST);
        } else {
            return funcs[id];
        }
    }


private:
    vector<int> membersOrder;
    std::unordered_map<int, TypeInfo> membersType;
    std::unordered_map<int, FuncInfo> funcs;
    int className;
};

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
