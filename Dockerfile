# C開発環境
FROM gcc:14 as c
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
CMD ["tail", "-f", "/dev/null"]

# Go開発環境
FROM golang:1.24.2-bullseye as go
ENV GOPATH="/root/go"
ENV PATH="${GOPATH}/bin:${PATH}"
WORKDIR /app
CMD ["tail", "-f", "/dev/null"]

# Rust開発環境
FROM rust:1.86-bullseye as rust
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
CMD ["tail", "-f", "/dev/null"]

# Taskをインストール
# RUN go install github.com/go-task/task/v3/cmd/task@latest
