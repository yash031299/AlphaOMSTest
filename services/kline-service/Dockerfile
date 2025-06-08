# ---------- STAGE 1: Build ----------
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake meson ninja-build \
    libspdlog-dev libboost-all-dev pkg-config

WORKDIR /app
COPY . .

RUN meson setup build --buildtype=release
RUN meson compile -C build

# ---------- STAGE 2: Runtime ----------
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libspdlog-dev libboost-system-dev

COPY --from=builder /app/build/kline-service /usr/local/bin/kline-service

WORKDIR /usr/local/bin

CMD ["/usr/local/bin/kline-service"]
