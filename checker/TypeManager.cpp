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
    assert(t == INT_V);
}

template<VALUE_TYPE T>
bool TypeInfo::Is() const {
    return type == T;
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
        stringstream << argsType[i].first.Format(idMap) << " " << idMap->nti(argsType[i].second);
        if (i != argsType.size() - 1)
            stringstream << ", ";
    }
    stringstream << ") -> " << retType.Format(idMap);

    return stringstream.str();
}


ClassInfo::ClassInfo(const int name) {
    className = name;
}

STATUS ClassInfo::DeclareMember(const int name, const TypeInfo &type) {
    if (membersType.find(name) == membersType.end() && funcs.find(name) == funcs.end()) {
        membersOrder.push_back(name);
        membersType[name] = type;
        return OK;
    } else {
        return SYMBOL_REPEAT;
    }
}


STATUS ClassInfo::DeclareFunc(const int name, TypeInfo ret, vector<pair<TypeInfo, int>> ts) {
    assert(ts.size() >= 1);
    if (membersType.find(name) == membersType.end() && funcs.find(name) == funcs.end()) {
        funcs[name] = FuncInfo(ret, vector<pair<TypeInfo, int>>(ts.begin() + 1, ts.end()));
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
    for (auto &x: funcs) {

        stringstream << "\t" << idMap->nti(x.first) << "<" << x.first << ">: " << x.second.Format(idMap) << "\n";
    }
    return stringstream.str();
}


TypeManager::TypeManager() {

}

void TypeManager::Init() {
    idMap.RecordIdentifier("this");
    idMap.Init();
}

void TypeManager::RecordIdentifier(const string &s) {
    idMap.RecordIdentifier(s);
}

STATUS TypeManager::TryDecodeType(const string &ts, TypeInfo &type) {
    auto pos = ts.find_first_of("|");
    assert(pos < ts.length());
    auto tt = ts.substr(0, pos);
    if (tt == "int") { ;
        assert(pos == ts.length() - 1);
        type = TypeInfo(INT_V);
        return OK;
    } else if (tt == "any") {
        assert(pos == ts.length() - 1);
        type = TypeInfo(ANY_V);
        return OK;
    } else if (tt == "pointer") {
        auto sts = TryDecodeType(ts.substr(pos + 1), type);
        type.IsPointer = true;
        return sts;
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


STATUS TypeManager::DeclareFunc(const string &name, const string &funcname, const TAC *func) {
    assert(classes.find(idMap.itn(name)) != classes.end());

    TypeInfo t;
    TypeInfo ret;
    CheckStatus(TryDecodeType(func->a->ToStr(), ret));
    vector<pair<TypeInfo, int>> ts;
    TAC *first = func->next;
    assert(first && first->op == TAC_FORMAL);
    auto thistype = first->a->ToStr();
    assert(first->b->ToStr() == "this");
    assert(thistype.find_first_of('|') == thistype.length() - 1);
    thistype += name;

    CheckStatus(TryDecodeType(thistype, t));

    ts.push_back({t, idMap.itn("this")});

    for (TAC *form = first->next; form && form->op == TAC_FORMAL; form = form->next) {
        CheckStatus(TryDecodeType(form->a->ToStr(), t));
        ts.push_back({t, idMap.itn(form->b->ToStr())});
    }
    return classes[idMap.itn(name)]->DeclareFunc(idMap.itn(funcname), ret, ts);
}

void TypeManager::Print() {
    for (auto &c: classes) {
        printf("%s\n", c.second->Format(&idMap).c_str());
    }
}

void TypeManager::CastType(string &from, const string &to) {
    TypeInfo to_t;
    CheckStatus(TryDecodeType(to, to_t));
    TypeInfo from_t;
    CheckStatus(TryDecodeType(from, from_t));
    CheckStatus(from_t.Cast(to_t));
    from = from_t.ToStr(&idMap);
}
