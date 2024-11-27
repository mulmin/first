#include <string.h>
#include "kvs.h"

// KVS에서 key 검색
char* get(kvs_t* kvs, const char* key) {
    for (size_t i = 0; i < kvs->size; i++) {
        if (strcmp(kvs->entries[i].key, key) == 0) {
            return kvs->entries[i].value;
        }
    }
    printf("Key not found: %s\n", key);
    return NULL;
}

