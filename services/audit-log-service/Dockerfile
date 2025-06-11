# ───────────────────────────────────────────────
# Stage 1: Build Stage
# ───────────────────────────────────────────────
FROM debian:bullseye AS builder

RUN apt-get update && apt-get install -y \
    git cmake build-essential ninja-build \
    pkg-config libboost-all-dev libspdlog-dev \
    libpq-dev libpqxx-dev libgrpc++-dev libprotobuf-dev protobuf-compiler-grpc \
    libhiredis-dev libssl-dev curl unzip wget

WORKDIR /app
COPY . .

RUN mkdir -p build && \
    meson setup build --buildtype=release && \
    meson compile -C build

# ───────────────────────────────────────────────
# Stage 2: Runtime Image
# ───────────────────────────────────────────────
FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y libboost-system-dev libboost-thread-dev libspdlog-dev libgrpc++1 libprotobuf-dev libhiredis-dev

WORKDIR /app
COPY --from=builder /app/build/audit-log-service /usr/local/bin/audit-log-service
COPY data ./data

EXPOSE 9090

ENTRYPOINT ["/usr/local/bin/audit-log-service"]
