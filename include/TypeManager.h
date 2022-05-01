//
// Created by os on 4/30/22.
//

#ifndef LIGHTC_TYPEMANAGER_H
#define LIGHTC_TYPEMANAGER_H

#include "string"
#include "set"
#include "unordered_map"
#include "tac.hpp"
#include "vector"
#include "def.h"

inline void CheckStatus(STATUS s) {
    if (s != OK) {
        switch (s) {
            case STATUS::SYMBOL_REPEAT: {
                error("Symbol is repeatedly used in this scope");
                break;
            }
            case STATUS::SYMBOL_UNDEFINED: {
                error("Symbol undefined!");
                break;
            }
            default: {

                error("Unknown error!");
                break;
            }
        }
    }
}

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

    STATUS Cast(TypeInfo &t) {
        if (Is<ANY_V>()) {
            assert(!type_name);
            assert(t.Is<ANY_V>() == false);
            type = t.type;
            type_name = t.type_name;
        } else {
            if (type != t.type || type_name != t.type_name) {
                return TYPE_CONFLICT;
            }

        }
        return OK;
    }

    bool IsPointer;

    string Format(IdentifierMap *idMap);

    string ToStr(IdentifierMap *idMap) const {
        string ans;
        if (IsPointer) {
            ans = "pointer|";
        }
        if (type == INT_V) {
            ans += "int|";
        } else if (type == LINK_V) {
            ans += "link|";
        } else if (type == REF_V) {
            ans += "ref|";
        } else {
            error("unknown type");
        }
        if (type_name)ans += idMap->nti(type_name);
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

    void CastType(string &from, const string &to);

    void Print();


private:

    IdentifierMap idMap;
    std::unordered_map<int, ClassInfo *> classes;

};


#endif //LIGHTC_TYPEMANAGER_H
