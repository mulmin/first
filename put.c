#include <stdlib.h>
#include <string.h>  
#include "kvs.h"

int put(kvs_t* kvs, const char* key, const char* value) {
    // KVS 용량 초과 시 확장
    if (kvs->size >= kvs->capacity) {
        printf("Expanding KVS capacity from %lu to %lu...\n", kvs->capacity, kvs->capacity * 2);

        // 새로운 용량 할당
        size_t new_capacity = kvs->capacity * 2;
        entry_t* new_entries = (entry_t*)malloc(new_capacity * sizeof(entry_t));
        if (!new_entries) {
            perror("Failed to expand KVS capacity");
            return -1;
        }

        // 기존 데이터 복사
        for (size_t i = 0; i < kvs->size; i++) {
            new_entries[i].key = kvs->entries[i].key;
            new_entries[i].value = kvs->entries[i].value;
        }

        // 기존 배열 해제
        free(kvs->entries);

        // 새로운 배열로 교체
        kvs->entries = new_entries;
        kvs->capacity = new_capacity;
    }

    // 새 데이터 추가
    size_t idx = kvs->size++;
    kvs->entries[idx].key = strdup(key);
    kvs->entries[idx].value = strdup(value);

    //printf("Inserted key: %s, value: %s\n", key, value);
    return 0;
}




