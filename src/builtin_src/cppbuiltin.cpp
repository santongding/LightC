//
// Created by os on 5/5/22.
//
//#define PRINT_DEBUG

#include "cppbuiltin.h"
#include "map"
#include "cassert"


using std::pair;
using std::string;
using std::vector;

enum ERROR {
    OK_R = 0,
    REF_TO_NULL_R = -1,
    REF_TO_WASTE_R = -2,
    GARBAGE_EXISTS = -3

};


extern std::map<int, ClassInfo> classes;
vector<vtype> object_stk;

void push_obj(vtype obj) {
#ifdef PRINT_DEBUG

    printf("stk push:%ld\n",obj);
#endif
    object_stk.push_back(obj);
}

vtype pop_obj() {
    assert(object_stk.empty() == false);
#ifdef PRINT_DEBUG
    printf("stk pop:%ld\n",object_stk.back());
#endif
    auto ans = object_stk.back();
    object_stk.pop_back();
    return ans;
}

class obj {
public:
    obj() = default;

    obj(int t, int oid) : classInfo(&classes[t]), ref(1), id(oid), tid(t) {
        values = vector<vtype>(classInfo->GetMemNum(), 0);
#ifdef PRINT_DEBUG
        printf("obj create, type:%d, id:%d\n", tid, id);
#endif
    }

    bool reduceRef() {
        assert(ref);
        ref--;
#ifdef PRINT_DEBUG
        printf("obj reduce ref, type:%d, id:%d, cur ref:%d \n", tid, id, ref);
#endif
        return ref == 0;
    }

    void addRef() {
        assert(ref);
        ref++;
#ifdef PRINT_DEBUG
        printf("obj add ref, type:%d, id:%d, cur ref:%d \n", tid, id, ref);
#endif
    }

    void setValue(int mid, vtype v) {
        values[classInfo->nid2order(mid)] = v;
    }

    vtype getValue(int mid) {
        return values[classInfo->nid2order(mid)];
    }

    TypeInfo getValueType(int mid) {
        auto ans = classInfo->GetMemType(mid);
        assert(ans.Is<INVALID_V>() == false);
        return ans;
    }

    TypeInfo getValueTypeByOrder(int order) {
        return classInfo->GetMemType(classInfo->order2nid(order));
    }

    vtype getValueByOrder(int order) {
        return values[order];
    }

    int getValueSize() {
        return values.size();
    }

private:
    vector<vtype> values;
    ClassInfo *classInfo = 0;
    int ref = 0, id = 0, tid = 0;
};

std::unordered_map<vtype, obj> obj_pool;
int cur_obj_num = 0;


obj *getObj(int oid) {
    if (!oid) {
        exit(REF_TO_NULL_R);
    } else if (obj_pool.find(oid) == obj_pool.end()) {
        exit(REF_TO_WASTE_R);
    }
    return &obj_pool[oid];
}

vtype getObjValue(vtype oid, vtype mid) {
    return getObj(oid)->getValue(mid);
}

void reduceRef(int oid) {
    auto o = getObj(oid);
    if (!o->reduceRef())return;
    //dispose the object

    for (int i = 0; i < o->getValueSize(); i++) {
        auto t = o->getValueTypeByOrder(i);
        if (t.Is<REF_V>()) {
            if (o->getValueByOrder(i)) {
                reduceRef(o->getValueByOrder(i));
            }
        }
    }

    obj_pool.erase(oid);
}

ClassInfo &getClass(vtype tid) {
    assert(tid);
    assert(classes.find(tid) != classes.end());
    return classes[tid];
}

extern "C" vtype __LIGHTC_FUNC_main_main(vtype t);

extern "C" vtype __LIGHTC_LOCATE(vtype oid, vtype mid) {
    auto o = getObj(oid);
    auto t = o->getValueType(mid);
    auto v = o->getValue(mid);
    if (t.Is<REF_V>()) {
        assert((!v) || getObj(v));
    } else if (t.Is<LINK_V>() && v) {
        getObj(v)->addRef();
        push_obj(v);
    }
    return v;
}

extern "C" vtype __LIGHTC_NEW(vtype tid) {
    ++cur_obj_num;
    obj_pool[cur_obj_num] = obj(tid, cur_obj_num);
    push_obj(cur_obj_num);
    return cur_obj_num;
}


extern "C" void __LIGHTC_BIND(vtype v, vtype oid, vtype mid) {
    auto o = getObj(oid);
    auto old_v = o->getValue(mid);
    auto t = o->getValueType(mid);
    if (v == old_v)return;
    if (t.Is<REF_V>()) {
        if (old_v) {
            reduceRef(old_v);
        }
    }
    o->setValue(mid, v);
}

extern "C" void __LIGHTC_BEFORE_CALL() {
    push_obj(0);
}

extern "C" void __LIGHTC_RET(vtype v, bool isRefRet) {

    if (isRefRet) {
        getObj(v)->addRef();
    }
    assert(object_stk.empty() == false);
    while (object_stk.back()) {
        reduceRef(pop_obj());
        assert(object_stk.empty() == false);
    }
    pop_obj();
    if (isRefRet) {
        push_obj(v);
    }
}

extern vtype main_obj_tid;

int main() {

    push_obj(0);
    auto m = __LIGHTC_NEW(main_obj_tid);
    __LIGHTC_BEFORE_CALL();
    auto code = __LIGHTC_FUNC_main_main(m);

    __LIGHTC_RET(0, false);

    if (obj_pool.size() != 0) {
        exit(GARBAGE_EXISTS);
    } else {
        exit(code);
    }
}
