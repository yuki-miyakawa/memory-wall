use std::time::Instant;
use rand::Rng;

const SIZE: usize = 16 * 1024 * 1024; // 64MB（キャッシュより十分大きい領域）

fn main() {
    // メモリの割り当て
    let mut array = vec![0u8; SIZE];
    let mut rng = rand::thread_rng();
    let mut random_indices: Vec<usize> = (0..SIZE).collect();
    
    // ランダムインデックスの事前生成
    let start = Instant::now();
    for i in 0..SIZE {
        random_indices[i] = rng.gen_range(0..SIZE);
    }
    // println!("Random indices generation: {:?}", start.elapsed());

    // 連続アクセス（キャッシュが有効）
    let start = Instant::now();
    for i in 0..SIZE {
        array[i] = (i % 256) as u8;
    }
    println!("Sequential access: {:?}", start.elapsed());

    // ランダムアクセス（キャッシュミス多発）
    let start = Instant::now();
    for i in 0..SIZE {
        array[random_indices[i]] = (i % 256) as u8;
    }
    println!("Random access: {:?}", start.elapsed());
} 