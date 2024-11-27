#include "kvs.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

// 시간 측정 함수
double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// 워크로드 로드 함수
void load_workload(kvs_t* kvs, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open workload file");
        return;
    }

    char key[512];
    char value[8192];
    while (fscanf(file, "%511s %8191s", key, value) == 2) {
        if (strlen(key) >= sizeof(key) || strlen(value) >= sizeof(value)) {
            printf("Warning: Skipping oversized entry - key: %s, value: %s\n", key, value);
            continue;
        }
        put(kvs, key, value);  // KVS에 데이터 추가
    }

    fclose(file);
    printf("Workload loaded successfully from %s\n", filename);
}

// 지정된 키에 대해 get 요청 수행 및 출력
void perform_get_tests(kvs_t* kvs) {
    const char* test_keys[] = {"tweet55", "tweet13843", "tweet3482"};
    const int num_keys = sizeof(test_keys) / sizeof(test_keys[0]);

    printf("\nPerforming get requests for test keys:\n");
    for (int i = 0; i < num_keys; i++) {
        const char* key = test_keys[i];
        const char* value = get(kvs, key);  // get 호출
        if (value) {
            printf("Key: %s, Value: %s\n", key, value);
            free((void*)value);  // get 함수가 할당한 메모리 해제
        } else {
            printf("Key: %s, Value: NOT FOUND\n", key);
        }
    }
}

int main() {
    printf("DEBUG: Program started.\n");

    // KVS 생성
    kvs_t* kvs = createKVS(1000);  // 초기 크기 1000
    if (!kvs) {
        fprintf(stderr, "Failed to create KVS. Exiting...\n");
        return -1;
    }
    printf("DEBUG: KVS created.\n");

    // 스냅샷 복구
    if (do_recovery(kvs) != 0) {
        printf("No valid snapshot found. Starting with an empty KVS.\n");
    }
    printf("DEBUG: Recovery process completed.\n");

    // 워크로드 로드
    load_workload(kvs, "cluster004.trc");
    printf("DEBUG: Workload loaded.\n");

    // 지정된 키에 대해 get 테스트 수행
    perform_get_tests(kvs);

    // 스냅샷 생성 시간 측정
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    do_snapshot(kvs);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("\nSnapshot time: %.6f seconds\n", get_time_diff(start, end));

    // KVS 종료 및 메모리 해제
    printf("DEBUG: Freeing KVS...\n");
    freeKVS(kvs);
    printf("DEBUG: KVS freed.\n");

    return 0;
}

