#include "kvs.h"
#include <stdlib.h>
#include <stdio.h>

void freeKVS(kvs_t* kvs) {
    if (!kvs) return;

    printf("DEBUG: Freeing KVS entries...\n");
    for (size_t i = 0; i < kvs->size; i++) {
        if (kvs->entries[i].key) {
            free(kvs->entries[i].key);  // key 메모리 해제
            kvs->entries[i].key = NULL;
        }
        if (kvs->entries[i].value) {
            free(kvs->entries[i].value);  // value 메모리 해제
            kvs->entries[i].value = NULL;
        }
    }

    free(kvs->entries);  // 전체 엔트리 배열 해제
    kvs->entries = NULL;

    free(kvs);  // KVS 자체 해제
    printf("DEBUG: KVS freed successfully.\n");
}

