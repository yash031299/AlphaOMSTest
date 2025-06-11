# ─────────────────────────────────────────────
# Stage 1: Build Stage
# ─────────────────────────────────────────────
FROM debian:bullseye AS builder

RUN apt-get update && apt-get install -y \
    git build-essential cmake ninja-build meson \
    pkg-config curl unzip wget \
    libprotobuf-dev protobuf-compiler \
    libgrpc++-dev grpc-proto \
    libspdlog-dev libboost-all-dev \
    libnlohmann-json-dev

WORKDIR /app/config-service

COPY . .
COPY ../proto ./proto/

# Install dependencies (Conan optional, disabled if local)
# RUN conan install . --output-folder=build --build=missing

RUN meson setup build --buildtype=release && \
    meson compile -C build

# ─────────────────────────────────────────────
# Stage 2: Runtime Stage
# ─────────────────────────────────────────────
FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    libprotobuf-dev libgrpc++1 libboost-system-dev libboost-thread-dev \
    libspdlog-dev libnlohmann-json-dev && \
    apt-get clean

WORKDIR /service

COPY --from=builder /app/config-service/build/config-service .
COPY .env .
COPY data/ data/
COPY health_checker.cpp health_checker.cpp

# Build health checker
RUN g++ -o health_checker health_checker.cpp

# Health check mechanism
HEALTHCHECK --interval=10s --timeout=2s --start-period=5s CMD ./health_checker

EXPOSE 50051

ENTRYPOINT ["./config-service"]
