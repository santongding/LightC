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
    RecordIdentifier(MAIN_CLASS_NAME);
    assert(!inited);
    int cnt = 0;
    for (auto &it: identifierMap) {
        id2num[it] = ++cnt;
    }
    inited = true;
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
    } else {
        assert(pos < ts.length() - 1);
        if (classes.find(idMap.itn(ts.substr(pos + 1))) == classes.end()) {
            return TYPE_UNKNOWN;
        }
        if (tt == "ref") {
            type = TypeInfo(REF_V, idMap.itn(ts.substr(pos + 1)));
        } else if (tt == "link") {
            type = TypeInfo(LINK_V, idMap.itn(ts.substr(pos + 1)));
        } else {
            CheckStatus(TYPE_CONFLICT);
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

void TypeManager::DeclareMember(const string &name, const string &mem, const string &type) {
    assert(classes.find(idMap.itn(name)) != classes.end());

    TypeInfo t;
    CheckStatus(TryDecodeType(type, t));

    classes[idMap.itn(name)]->DeclareMember(idMap.itn(mem), t);

}


void TypeManager::DeclareFunc(const string &name, const string &funcname, const TAC *func) {
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
    first->a->SetStr(thistype);

    CheckStatus(TryDecodeType(thistype, t));

    ts.push_back({t, idMap.itn("this")});

    for (TAC *form = first->next; form && form->op == TAC_FORMAL; form = form->next) {
        CheckStatus(TryDecodeType(form->a->ToStr(), t));
        ts.push_back({t, idMap.itn(form->b->ToStr())});
    }
    classes[idMap.itn(name)]->DeclareFunc(idMap.itn(funcname), ret, ts);
}

void TypeManager::Print() {
    for (auto &c: classes) {
        printf("%s\n", c.second->Format(&idMap).c_str());
    }
}

void
TypeManager::CheckTac(SYM *caller, SYM *callee, TAC *tac, std::function<SYM *(SYM *)> &get_sym_type) {
    TypeInfo ee;
    CheckStatus(TryDecodeType(callee->ToStr(), ee));

    if (!ee.Is<REF_V>()) {
        CheckStatus(TYPE_NEED_CLASS);
    }
    auto type = ee.GetTypeName();

    assert(classes.find(type) != classes.end());

    auto cls = classes[type];
    if (tac->op == TAC_LOCATE || tac->op == TAC_BIND) {
        auto tp = cls->GetMemType(idMap.itn(tac->c->ToStr()));
        if (tp.Is<INVALID_V>()) {
            CheckStatus(CLASS_MEMBER_NOT_EXIST);
        }
        CastType(caller, tp);
    } else if (tac->op == TAC_CALL) {
        auto tp = cls->GetFunc(idMap.itn(tac->c->ToStr()));
        auto ti = tp.GetRetType();
        CastType(caller, ti);

        vector<TAC *> tacs;
        while (tac->prev->op == TAC_ACTUAL) {
            tacs.push_back(tac = tac->prev);
        }
        if (tacs.size() != tp.GetArgsType().size()) {
            CheckStatus(FUNC_PARAM_NOT_MATCH);
        }
        for (auto &arg: tp.GetArgsType()) {
            tac = tacs.back();
            tacs.pop_back();

            SYM *sym_type = get_sym_type(tac->a);
            CastType(sym_type, arg.first);
        }


    } else {
        CheckStatus(UNEXPECTED_ERROR);
    }
}

void TypeManager::CastType(SYM *from, SYM *to, bool isInt) {
    TypeInfo to_t;
    CheckStatus(TryDecodeType(to->ToStr(), to_t));
    CastType(from, to_t);

    if (isInt) {
        if (from->ToStr().find("int")) {
            CheckStatus(TYPE_NEED_INT);
        }
    }
}

void TypeManager::CastType(SYM *from, const TypeInfo &t) {
    TypeInfo from_t;
    CheckStatus(TryDecodeType(from->ToStr(), from_t));
    from_t.Cast(t);
    from->SetStr(from_t.ToStr(&idMap));
}

string TypeManager::Dump() {
    std::stringstream stringstream;
    stringstream<<"std::map<int, ClassInfo> classes = {\n";
    for (auto &c: classes) {
        stringstream << "\t{" << c.first << "," << c.second->Dump() << "},\n";
    }
    stringstream << "};\n";
    if(classes.find( idMap.itn(MAIN_CLASS_NAME))==classes.end()){
        CheckStatus(MAIN_CLASS_NOT_DEFINE);
    }
    stringstream << "vtype main_obj_tid = " << idMap.itn(MAIN_CLASS_NAME) << ";\n";
    return stringstream.str();
}