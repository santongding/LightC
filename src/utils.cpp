//
// Created by os on 5/3/22.
//

#include "utils.h"

const char *Status2Str(STATUS c) {
    switch (c) {
#define DEF_STATUS(v, x) case STATUS::v: return #v;
        STATUS_TABLE()
#undef DEF_STATUS
        default:
            return "??";
    }
}
int _user_line =0;
void setlineno(int l){
    _user_line = l;
}

int getlineno(){
    return _user_line;
}

void error(const char *str,int code) {
    fprintf(stderr, "line %d:  error<%d>: %s\n", getlineno(), code,str);
    exit(code);
}

void error(const string &str,int code) {
    error(str.c_str(),code);
}

void CheckStatus(STATUS s) {
    if (s != OK) {
        error(Status2Str(s), s);
    }
}