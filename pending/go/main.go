package main

import (
	"fmt"
	"math/rand"
	"time"
)

const SIZE = 1024 * 1024 * 16 // 64MB（キャッシュより十分大きい領域）

func main() {
	// メモリの割り当て
	array := make([]byte, SIZE)
	randomIndices := make([]int, SIZE)

	// ランダムインデックスの事前生成
	start := time.Now()
	for i := 0; i < SIZE; i++ {
		randomIndices[i] = rand.Intn(SIZE)
	}
	// fmt.Printf("Random indices generation: %v\n", time.Since(start))

	// 連続アクセス（キャッシュが有効）
	start = time.Now()
	for i := 0; i < SIZE; i++ {
		array[i] = byte(i % 256)
	}
	fmt.Printf("Sequential access: %v\n", time.Since(start))

	// ランダムアクセス（キャッシュミス多発）
	start = time.Now()
	for i := 0; i < SIZE; i++ {
		array[randomIndices[i]] = byte(i % 256)
	}
	fmt.Printf("Random access: %v\n", time.Since(start))
} 