//
// Created by os on 5/5/22.
//
#define IS_BUILTIN

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

class obj {
public:
    obj()=default;
    obj(int t) : classInfo(&classes[t]), ref(1) {
        values = vector<vtype>(classInfo->GetMemNum(), 0);
    }

    bool reduceRef() {
        assert(ref);
        ref--;
        return ref == 0;
    }

    void addRef() {
        assert(ref);
        ref++;
    }

    void setValue(int mid, vtype v) {
        values[classInfo->nid2order(mid)] = v;
    }

    vtype getValue(int mid) {
        return values[classInfo->nid2order(mid)];
    }

    TypeInfo getValueType(int mid) {
        auto ans =  classInfo->GetMemType(mid);
        assert(ans.Is<INVALID_V>()==false);
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
    int ref = 0;
};

std::unordered_map<vtype, obj> obj_pool;
int cur_obj_num = 0;


obj *getObj(int oid) {
    if(!oid){
        exit(REF_TO_NULL_R);
    }else if(obj_pool.find(oid)!=obj_pool.end()){
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
        object_stk.push_back(v);
    }
    return v;
}

extern "C" vtype __LIGHTC_NEW(vtype tid) {
    obj_pool[++cur_obj_num] = obj(tid);
    object_stk.push_back(cur_obj_num);
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
    object_stk.push_back(0);
}

extern "C" void __LIGHTC_RET(vtype v, bool isRefRet) {

    if (isRefRet) {
        getObj(v)->addRef();
    }
    while (object_stk.back()) {
        reduceRef(object_stk.back());
        object_stk.pop_back();
    }
    object_stk.pop_back();
    if (isRefRet) {
        object_stk.push_back(v);
    }
}

extern vtype main_obj_tid;

int main() {
    object_stk.push_back(0);
    auto m = __LIGHTC_NEW(main_obj_tid);
    auto code = __LIGHTC_FUNC_main_main(m);

    __LIGHTC_RET(0, false);

    if (obj_pool.size() != 0) {
        exit(GARBAGE_EXISTS);
    } else {
        exit(code);
    }
}
