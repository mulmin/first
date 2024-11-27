#include <stdio.h>
#include "kvs.h"

// KVS 초기화 및 복구 포함
kvs_t* createKVS(size_t capacity) {
    kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
    if (!kvs) {
        perror("Failed to allocate memory for KVS");
        return NULL;
    }

    kvs->entries = (entry_t*)malloc(capacity * sizeof(entry_t));
    if (!kvs->entries) {
        perror("Failed to allocate memory for KVS entries");
        free(kvs);
        return NULL;
    }

    for (size_t i = 0; i < capacity; i++) {
        kvs->entries[i].key = NULL;    // 초기화
        kvs->entries[i].value = NULL; // 초기화
    }

    kvs->size = 0;
    kvs->capacity = capacity;
    return kvs;
}

