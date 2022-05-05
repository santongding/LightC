//
// Created by os on 5/5/22.
//

#include "TypeInfo.hpp"
#include "cppbuiltin.h"
#include "map"

extern std::map<int, ClassInfo> classes;
vector<vtype> object_stk;

struct obj {
    obj(int t) : classInfo(&classes[t]), ref(1) {
        values = vector<vtype>(classInfo->GetMemNum(), 0);
    }


    vector<vtype> values;
    ClassInfo *classInfo = 0;
    int ref = 0;
};

std::unordered_map<vtype, obj> obj_pool;
int cur_obj_num = 0;


obj &getObj(int oid) {
    assert(obj_pool.find(oid) != obj_pool.end());
    return obj_pool[oid];
}


void reduceRef(int oid) {
    auto &o = getObj(oid);
    assert(o.ref);
    o.ref--;
    if (o.ref)return;

    //dispose the object

    for (int i = 0; i < o.values.size(); i++) {
        auto t = o.classInfo->GetMemType(o.classInfo->order2nid(i));
        if (t.Is<REF_V>()) {
            if (o.values[i]) {
                reduceRef(o.values[i]);
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

extern vtype __LIGHTC_FUNC_main_main(vtype t);

vtype __LIGHTC_LOCATE(vtype oid, vtype vtype) {

}

vtype __LIGHTC_NEW(vtype tid) {
    obj_pool[++cur_obj_num] = obj(tid);
    return cur_obj_num;
}

vtype __LIGHTC_BIND(vtype v, vtype oid, vtype mid);

void __LIGHTC_BEFORE_CALL() {
    object_stk.push_back(0);
}

void __LIGHTC_RET(int v, bool isRefRet) {

    while (object_stk.back()){
        reduceRef(object_stk.back());
        object_stk.pop_back();
    }
    object_stk.pop_back();
    assert(object_stk.empty()== false);
}

extern vtype main_obj_tid;

int main() {
    object_stk.push_back(0);
    auto m = __LIGHTC_NEW(main_obj_tid);
    exit(__LIGHTC_FUNC_main_main(m));
}


std::map<int, ClassInfo> classes={};
vtype main_obj_tid=43;