//
// Created by os on 5/5/22.
//
#include "linker.hpp"
#include "cstdio"
#include "cstdlib"
const string temp_builtin_path = "./src/builtin_src/cppbuiltin.cpp";
void dumpBuiltin(TypeManager *mgr,const string & builtinOutput){
    auto cmd = ("cp "+temp_builtin_path+" "+builtinOutput);
    system(cmd.c_str());
    auto bo = fopen(builtinOutput.c_str(),"a");
    auto f = mgr->Dump();
    fprintf(bo,"\n%s",f.c_str());
    fclose(bo);
}