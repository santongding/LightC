//
// Created by os on 5/3/22.
//

#ifndef LIGHTC_TYPEINFO_HPP
#define LIGHTC_TYPEINFO_HPP

#include "def.h"
#include "string"
#include "set"
#include "unordered_map"
#include "vector"
#include "cassert"

using std::pair;
using std::string;
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

    TypeInfo(VALUE_TYPE t, int n) : type_name(n), type(t) {

        assert(type_name > 0);
        assert(t == LINK_V || t == REF_V);
    }

    TypeInfo(VALUE_TYPE t) : type_name(0), type(t) {
        assert(t == INT_V || t == ANY_V || t == INVALID_V);
    }

    string Dump() const;

    template<VALUE_TYPE T>
    bool Is() const {
        return type == T;
    }

    int GetTypeName() const;

    void Cast(const TypeInfo &t);


    string Format(IdentifierMap *idMap);

    string ToStr(IdentifierMap *idMap) const;

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
    ClassInfo()=default;
    ClassInfo(const int name);

    ClassInfo(vector<pair<int, TypeInfo>> types) {
        for (auto &t: types) {
            membersType[t.first] = t.second;
            id2order[t.first] = order2id.size();
            order2id.push_back(t.first);
        }
    }

    void DeclareMember(const int name, const TypeInfo &type);

    void DeclareFunc(const int name, TypeInfo ret, vector<pair<TypeInfo, int>> ts);

    string Dump() const;

    string Format(IdentifierMap *idMap);

    FuncInfo GetFunc(int id);

    TypeInfo GetMemType(int id) {
        if (membersType.find(id) == membersType.end()) {
            return {};
        } else {
            return membersType[id];
        }
    }


    int GetMemNum() {
        return membersType.size();
    }

    int order2nid(int order) {
        return order2id[order];
    }

    int nid2order(int id) {
        return id2order[id];
    }


private:
    std::unordered_map<int, int> id2order;
    vector<int> order2id;
    std::unordered_map<int, TypeInfo> membersType;
    std::unordered_map<int, FuncInfo> funcs;
    int className;
};

#endif //LIGHTC_TYPEINFO_HPP
