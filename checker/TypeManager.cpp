//
// Created by os on 4/30/22.
//
#include "TypeManager.h"
#include "sstream"

void IdentifierMap::RecordIdentifier(const string &s) {
    for (const auto &id: identifierMap) {
        if (id == s)return;
    }
    identifierMap.push_back(s);
}

string IdentifierMap::nti(int id) {
    assert(inited);
    assert(id && id <= identifierMap.size());
    return identifierMap[id - 1];
}

int IdentifierMap::itn(const string &s) {
    assert(inited);
    assert(id2num.find(s) != id2num.end());

    return id2num[s];
}

void IdentifierMap::Init() {
    assert(!inited);
    int cnt = 0;
    for (auto &it: identifierMap) {
        id2num[it] = ++cnt;
    }
    inited = true;
}


TypeInfo::TypeInfo(VALUE_TYPE t, int n) {
    type_name = n;
    type = t;
    assert(type_name > 0);
    assert(t == LINK_V || t == REF_V);
}

TypeInfo::TypeInfo(VALUE_TYPE t) {
    type_name = 0;
    type = t;
    assert(t == VOID_V || t == INT_V);
}

template<VALUE_TYPE T>
bool TypeInfo::Is() const {
    return type == T;
}

string TypeInfo::Format(IdentifierMap *idMap) {
    std::stringstream stringstream;
    if (type == VOID_V) {
        return "[void]";
    } else if (type == INT_V) {
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


ClassInfo::ClassInfo(const int name) {
    className = name;
}

STATUS ClassInfo::DeclareMember(const int name, const TypeInfo &type) {
    if (type.Is<VOID_V>()) {
        return VOID_FORBID;
    }
    if (membersType.find(name) == membersType.end()) {

        membersOrder.push_back(name);
        membersType[name] = type;
        return OK;
    } else {
        return SYMBOL_REPEAT;
    }
}

string ClassInfo::Format(IdentifierMap *idMap) {
    std::stringstream stringstream;
    stringstream << idMap->nti(className) << "<" << className << ">:\n";
    for (auto &i: membersOrder) {
        auto x = membersType.find(i);
        stringstream << "\t" << x->second.Format(idMap) << " " << idMap->nti(x->first) << "<" << x->first << ">\n";
    }
    return stringstream.str();
}


TypeManager::TypeManager() {

}

void TypeManager::Init() {
    idMap.Init();
}

void TypeManager::RecordIdentifier(const string &s) {
    idMap.RecordIdentifier(s);
}

STATUS TypeManager::TryDecodeType(const string &ts, TypeInfo &type) {
    auto pos = ts.find_first_of("|");
    assert(pos < ts.length());
    auto tt = ts.substr(0, pos);
    if (tt == "void") {
        assert(pos == ts.length() - 1);
        type = TypeInfo(VOID_V);
        return OK;
    } else if (tt == "int") { ;
        assert(pos == ts.length() - 1);
        type = TypeInfo(INT_V);
        return OK;
    } else {
        assert(pos < ts.length() - 1);
        if (classes.find(idMap.itn(ts.substr(pos + 1))) == classes.end()) {
            return SYMBOL_UNDEFINED;
        }
        if (tt == "ref") {
            type = TypeInfo(REF_V, idMap.itn(ts.substr(pos + 1)));
        } else if (tt == "link") {
            type = TypeInfo(LINK_V, idMap.itn(ts.substr(pos + 1)));
        } else {
            error("wrong value type");
        }
        return OK;
    }
}

STATUS TypeManager::DeclareClass(const string &name) {

    if (classes.find(idMap.itn(name)) == classes.end()) {
        classes[idMap.itn(name)] = new ClassInfo(idMap.itn(name));
        return OK;
    } else {
        return SYMBOL_REPEAT;
    }
}

STATUS TypeManager::DeclareMember(const string &name, const string &mem, const string &type) {
    assert(classes.find(idMap.itn(name)) != classes.end());

    TypeInfo t;
    auto sts = TryDecodeType(type, t);
    if (sts != OK) {
        return sts;
    }
    return classes[idMap.itn(name)]->DeclareMember(idMap.itn(mem), t);

}

void TypeManager::Print() {
    for (auto &c: classes) {
        printf("%s\n", c.second->Format(&idMap).c_str());
    }
}

