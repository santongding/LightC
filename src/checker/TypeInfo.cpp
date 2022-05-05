//
// Created by os on 5/5/22.
//
#include "TypeInfo.hpp"
#include "string"
#include "sstream"
#include "utils.hpp"


int TypeInfo::GetTypeName() const {
    assert(type_name);
    assert(Is<LINK_V>() || Is<REF_V>());
    return type_name;
}

void TypeInfo::Cast(const TypeInfo &t) {
    if (t.Is<ANY_V>()) {
        CheckStatus(TYPE_UNKNOWN);
    }
    if (Is<ANY_V>()) {
        assert(!type_name);
        type = t.type;
        type_name = t.type_name;
    } else {
        if (type != t.type || type_name != t.type_name) {
            CheckStatus(TYPE_CONFLICT);
        }
    }
}

string TypeInfo::ToStr(IdentifierMap *idMap) const {
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

string TypeInfo::Format(IdentifierMap *idMap) {
    std::stringstream stringstream;
    assert(type != ANY_V);
    if (type == INT_V) {
        return "[int]";
    } else {
        stringstream << "[";
        if (type == LINK_V) {
            stringstream << "link ";
        }
        stringstream << idMap->nti(type_name) << "<" << type_name << ">]";
    }
    return stringstream.str();
}

string FuncInfo::Format(IdentifierMap *idMap) {
    std::stringstream stringstream;
    stringstream << "(";
    for (int i = 0; i < argsType.size(); i++) {
        stringstream << idMap->nti(argsType[i].second) << "<"
                     << argsType[i].second << ">: " << argsType[i].first.Format(idMap);
        if (i != argsType.size() - 1)
            stringstream << ", ";
    }
    stringstream << ") -> " << retType.Format(idMap);

    return stringstream.str();
}


ClassInfo::ClassInfo(const int name) {
    className = name;
}

void ClassInfo::DeclareMember(const int name, const TypeInfo &type) {
    if (membersType.find(name) == membersType.end() && funcs.find(name) == funcs.end()) {

        order2id.push_back(name);
        id2order[name] = order2id.size() - 1;
        membersType[name] = type;
    } else {
        CheckStatus(SYMBOL_REPEAT);
    }
}


void ClassInfo::DeclareFunc(const int name, TypeInfo ret, vector<pair<TypeInfo, int>> ts) {
    assert(ts.size() >= 1);
    if (membersType.find(name) == membersType.end() && funcs.find(name) == funcs.end()) {
        funcs[name] = FuncInfo(ret, vector<pair<TypeInfo, int>>(ts.begin(), ts.end()));

    } else {
        CheckStatus(SYMBOL_REPEAT);
    }
}

string ClassInfo::Format(IdentifierMap *idMap) {
    std::stringstream stringstream;
    stringstream << idMap->nti(className) << "<" << className << ">:\n";
    for (auto &x: membersType) {
        stringstream << "\t" << idMap->nti(x.first) << "<" << x.first << ">: " << x.second.Format(idMap) << "\n";
    }
    for (auto &x: funcs) {

        stringstream << "\t" << idMap->nti(x.first) << "<" << x.first << ">: " << x.second.Format(idMap) << "\n";
    }
    return stringstream.str();
}


FuncInfo ClassInfo::GetFunc(int id) {
    if (funcs.find(id) == funcs.end()) {
        CheckStatus(CLASS_MEMBER_NOT_EXIST);
    } else {
        return funcs[id];
    }
}

string ClassInfo::Dump() const {
    std::stringstream stringstream("{");
    for (auto &t: membersType) {
        stringstream << "{" << t.first << "," << t.second.Dump() << "},";
    }
    stringstream << "}";
    return stringstream.str();

}

string TypeInfo::Dump() const {
    std::stringstream stringstream("{");
    if (type == INT_V) {
        stringstream << INT_V;
    } else {
        assert(type == REF_V || type == LINK_V);
        stringstream << type << "," << type_name;
    }
    stringstream << "}";
    return stringstream.str();

}