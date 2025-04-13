#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#ifdef __linux__
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

#define PERF_EVENT_OPEN 336

// キャッシュレベルの定義
#define PERF_COUNT_HW_CACHE_L1D 0
#define PERF_COUNT_HW_CACHE_L1I 1
#define PERF_COUNT_HW_CACHE_LL  2

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                          int cpu, int group_fd, unsigned long flags) {
    return syscall(PERF_EVENT_OPEN, hw_event, pid, cpu, group_fd, flags);
}
#endif

int main() {
#ifdef __linux__
    struct perf_event_attr pe;
    int fd1, fd2, fd3;
    long long value1, value2, value3;
    int ret;

    // L1データキャッシュミスの設定
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HW_CACHE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    fd1 = perf_event_open(&pe, -1, 0, -1, 0);
    if (fd1 == -1) {
        fprintf(stderr, "Error opening L1 cache event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // L1命令キャッシュミスの設定
    pe.config = (PERF_COUNT_HW_CACHE_L1I) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
    fd2 = perf_event_open(&pe, -1, 0, -1, 0);
    if (fd2 == -1) {
        fprintf(stderr, "Error opening L1 instruction cache event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // 最後レベルキャッシュ（LLC）ミスの設定
    pe.config = (PERF_COUNT_HW_CACHE_LL) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
    fd3 = perf_event_open(&pe, -1, 0, -1, 0);
    if (fd3 == -1) {
        fprintf(stderr, "Error opening LLC cache event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // イベントの開始
    ioctl(fd1, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd2, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd3, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd1, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd2, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd3, PERF_EVENT_IOC_ENABLE, 0);

    // テスト用のメモリアクセス
    const int size = 1000000;
    int *array = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        array[i] = i;
    }

    // イベントの停止
    ioctl(fd1, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd2, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd3, PERF_EVENT_IOC_DISABLE, 0);

    // 結果の読み取り
    ret = read(fd1, &value1, sizeof(long long));
    if (ret == -1) {
        fprintf(stderr, "Error reading L1 data cache event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    ret = read(fd2, &value2, sizeof(long long));
    if (ret == -1) {
        fprintf(stderr, "Error reading L1 instruction cache event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    ret = read(fd3, &value3, sizeof(long long));
    if (ret == -1) {
        fprintf(stderr, "Error reading LLC cache event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("L1 Data Cache Misses: %lld\n", value1);
    printf("L1 Instruction Cache Misses: %lld\n", value2);
    printf("Last Level Cache (LLC) Misses: %lld\n", value3);

    free(array);
    close(fd1);
    close(fd2);
    close(fd3);
#else
    printf("このプログラムはLinux環境でのみ動作します。\n");
    printf("コンテナ上で実行するには、Linuxコンテナを使用してください。\n");
#endif

    return 0;
} 