# 컴파일러 및 플래그
CC = gcc
CFLAGS = -Wall -g -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lrt

# 소스 파일 목록
COMMON_SRCS = put.c get.c open.c close.c
BASELINE_SRCS = main.c kvs_baseline.c
CUSTOM_SRCS = main.c kvs_custom.c

# 타겟 이름
BASELINE = kvs_baseline
CUSTOM = kvs_custom

# 기본 규칙 (Baseline과 Custom 빌드)
all: $(BASELINE) $(CUSTOM)

# Baseline 빌드
$(BASELINE): $(BASELINE_SRCS) $(COMMON_SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Custom 빌드
$(CUSTOM): $(CUSTOM_SRCS) $(COMMON_SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# 실행 규칙
run_baseline: kvs_baseline
	@if [ ! -f kvs.img ]; then \
		echo "No snapshot file found. Creating an empty snapshot file..."; \
		touch kvs.img; \
	fi
	./kvs_baseline


run_custom: $(CUSTOM)
	./$(CUSTOM)

# Clean 규칙
clean:
	rm -f $(BASELINE) $(CUSTOM) *.o

