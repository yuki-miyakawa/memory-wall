FROM ubuntu:22.04

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Rustをインストール
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

# 作業ディレクトリを作成
WORKDIR /app

CMD ["tail", "-f", "/dev/null"]