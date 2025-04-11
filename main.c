#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE (1024 * 1024 * 16) // 64MB（キャッシュより十分大きい領域）

int main() {
    // メモリの割り当て
    char *array = malloc(SIZE);
    int *random_indices = malloc(SIZE * sizeof(int)); // ランダムインデックス用の配列
    clock_t start, end;

    // ランダムインデックスの事前生成
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        random_indices[i] = rand() % SIZE;
    }
    end = clock();
    // printf("Random indices generation: %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 連続アクセス（キャッシュが有効）
    start = clock();
    for (int i = 0; i < SIZE; i++) array[i] = i % 256; // 各要素に値を代入
    end = clock();
    printf("Sequential access: %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // ランダムアクセス（キャッシュミス多発）
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        array[random_indices[i]] = i % 256; // 事前生成したランダムインデックスを使用
    }
    end = clock();
    printf("Random access: %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    // メモリの解放
    free(array);
    free(random_indices);
    return 0;
}