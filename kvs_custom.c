#include "kvs.h"
#include <fcntl.h>   // open 함수
#include <unistd.h>  // read, write, close, fsync 함수
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>    // isspace

// Custom 스냅샷 생성
int do_snapshot(kvs_t* kvs) {
    int fd = open("kvs.img", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open kvs.img for writing");
        return -1;
    }

    for (size_t i = 0; i < kvs->size; i++) {
        size_t key_len = strlen(kvs->entries[i].key);
        size_t value_len = strlen(kvs->entries[i].value);
        write(fd, &key_len, sizeof(size_t));
        write(fd, kvs->entries[i].key, key_len);
        write(fd, &value_len, sizeof(size_t));
        write(fd, kvs->entries[i].value, value_len);
    }

    fsync(fd); // 디스크 동기화
    close(fd);
    printf("Snapshot completed successfully.\n");
    return 0;
}

int do_recovery(kvs_t* kvs) {
    int fd = open("kvs.img", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open kvs.img for reading");
        printf("No valid snapshot found. Starting with an empty KVS.\n");
        return 0;  // 빈 상태로 시작
    }

    char buffer[9000];      // 파일 읽기 버퍼
    char leftover[9000] = ""; // 이전 버퍼에서 남은 데이터
    size_t count = 0;
    ssize_t bytes_read;

    static int warning_count = 0;  // 경고 메시지 출력 제한용

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  // NULL-terminate
        printf("DEBUG: Read %zd bytes from snapshot file.\n", bytes_read);

        // 이전 남은 데이터와 현재 버퍼를 결합
        char combined[18000];
        snprintf(combined, sizeof(combined), "%s%s", leftover, buffer);

        // 데이터 파싱
        char* line = strtok(combined, "\n");
        while (line != NULL) {
            char key[512], value[8192];

            // 유효성 검사 및 파싱
            if (sscanf(line, "%511s %8191[^\n]", key, value) == 2) {
                if (strlen(key) < 512 && strlen(value) < 8192) {
                    if (put(kvs, key, value) == 0) {
                        count++;
                    } else if (warning_count < 10) {
                        printf("Warning: Failed to insert key: %s\n", key);
                        warning_count++;
                    }
                } else if (warning_count < 10) {
                    printf("Warning: Skipping oversized key or value: key=%s\n", key);
                    warning_count++;
                }
            } else if (warning_count < 10) {
                printf("Warning: Skipping invalid line: %s\n", line);
                warning_count++;
            }

            line = strtok(NULL, "\n");
        }

        // 마지막 줄(완성되지 않은 데이터)을 leftover에 저장
        if (line != NULL) {
            size_t leftover_length = strlen(line);
            if (leftover_length < sizeof(leftover)) {
                strncpy(leftover, line, leftover_length);
                leftover[leftover_length] = '\0';
                printf("DEBUG: Leftover saved for next read: %s\n", leftover);
            } else {
                printf("Warning: Leftover data too large to save. Discarding.\n");
                leftover[0] = '\0';
            }
        } else {
            leftover[0] = '\0';  // 남은 데이터 없음
        }
    }

    if (bytes_read == -1) {
        perror("Failed to read from kvs.img");
    }

    if (warning_count >= 10) {
        printf("Warning: Too many invalid lines. Further warnings suppressed.\n");
    }

    close(fd);

    printf("Recovery completed successfully. %zu entries restored.\n", count);
    return 0;
}




