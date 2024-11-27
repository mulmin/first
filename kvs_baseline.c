#include "kvs.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h> // fsync 함수 정의

// Baseline 스냅샷 생성
int do_snapshot(kvs_t* kvs) {
    FILE* file = fopen("kvs.img", "w");
    if (!file) {
        perror("Failed to open kvs.img for writing");
        return -1;
    }

    for (size_t i = 0; i < kvs->size; i++) {
        fprintf(file, "%s %s\n", kvs->entries[i].key, kvs->entries[i].value);
    }

    fflush(file);
    fsync(fileno(file));  // 디스크에 데이터 저장
    fclose(file);
    printf("Snapshot completed successfully.\n");
    return 0;
}


// Baseline 복구
int do_recovery(kvs_t* kvs) {
    FILE* file = fopen("kvs.img", "r");
    if (!file) {
        perror("Failed to open kvs.img for reading");
        printf("No valid snapshot found. Starting with an empty KVS.\n");
        return 0;  // 빈 상태로 시작
    }

    char line[9000];
    char key[512], value[8192];
    size_t count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%511s %8191[^\n]", key, value) == 2) {
            if (put(kvs, key, value) == 0) {
                count++;
            } else {
                printf("Warning: Failed to insert key: %s\n", key);
            }
        }
    }

    fclose(file);
    printf("Recovery completed successfully. %zu entries restored.\n", count);
    return 0;
}


