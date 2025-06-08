# ---------- STAGE 1: Build ----------
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake meson ninja-build \
    libprotobuf-dev protobuf-compiler \
    libgrpc++-dev protobuf-compiler-grpc \
    libspdlog-dev libnlohmann-json-dev \
    git pkg-config

WORKDIR /app
COPY . .

RUN meson setup build --buildtype=release
RUN meson compile -C build

# ---------- STAGE 2: Runtime ----------
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libprotobuf-dev libgrpc++1 libspdlog-dev libnlohmann-json-dev

COPY --from=builder /app/build/symbol-registry-service /usr/local/bin/symbol-registry-service
COPY config/symbols.json /etc/symbol-registry/symbols.json

WORKDIR /usr/local/bin
ENV REDIS_HOST=localhost
ENV REDIS_PORT=6379

CMD ["/usr/local/bin/symbol-registry-service"]
