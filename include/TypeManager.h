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

    string Format(IdentifierMap *idMap);

private:
    VALUE_TYPE type;
    int type_name;

};

class FuncInfo{
public:
    FuncInfo()=default;
private:
    TypeInfo retType;
    vector<TypeInfo> argsType;
};

class ClassInfo {
public:
    ClassInfo(const int name);

    STATUS DeclareMember(const int name, const TypeInfo &type);

    STATUS DeclareFunc(const int name,TAC * formals,int formalNum,TAC * ret);


    string Format(IdentifierMap *idMap);

private:
    vector<int>membersOrder;
    std::unordered_map<int,TypeInfo> membersType;
    std::unordered_map<int,FuncInfo> funcs;
    int className;
};

class TypeManager {
public:
    TypeManager();

    void Init();

    void RecordIdentifier(const string &s);

    STATUS TryDecodeType(const string &ts, TypeInfo & type);

    STATUS DeclareClass(const string &name);

    STATUS DeclareMember(const string &name, const string &mem, const string &type);

    STATUS DeclareFunc(const string &name,const string &funcname,TAC * formals,int formalNum,TAC * ret);

    void Print();


private:

    IdentifierMap idMap;
    std::unordered_map<int, ClassInfo *> classes;

};


#endif //LIGHTC_TYPEMANAGER_H
