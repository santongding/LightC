//
// Created by os on 5/5/22.
//

#ifndef LIGHTC_CPPBUILTIN_H
#define LIGHTC_CPPBUILTIN_H

int __LIGHTC_LOCATE(int oid,int mid);

int __LIGHTC_NEW(int tid);

int __LIGHTC_BIND(int v,int oid,int mid);

void __LIGHTC_BEFORE_CALL();

void __LIGHTC_RET(bool isRefRet);


#endif //LIGHTC_CPPBUILTIN_H

