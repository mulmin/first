#ifndef KVS_H
#define KVS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// KVS 구조체 정의
typedef struct {
    char* key;    // 배열 대신 포인터
    char* value;  // 배열 대신 포인터
} entry_t;

typedef struct {
    entry_t* entries;  // 엔트리 배열
    size_t size;       // 현재 크기
    size_t capacity;   // 최대 용량
} kvs_t;


// 함수 선언
kvs_t* createKVS(size_t initial_capacity);
void freeKVS(kvs_t* kvs);
int put(kvs_t* kvs, const char* key, const char* value);
char* get(kvs_t* kvs, const char* key);
int do_snapshot(kvs_t* kvs);
int do_recovery(kvs_t* kvs);
void load_workload(kvs_t* kvs, const char* filename);  // 워크로드 로드 함수

#endif // KVS_H
