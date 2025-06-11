# ───────────────────────────────
# Stage 1: Build
# ───────────────────────────────
FROM debian:bullseye AS builder

RUN apt-get update && apt-get install -y \
    git build-essential cmake ninja-build \
    pkg-config python3-pip curl wget \
    libboost-all-dev libgrpc++-dev libprotobuf-dev protobuf-compiler-grpc \
    libhiredis-dev libspdlog-dev

# Install Conan + Meson
RUN pip3 install conan==2.2.2 meson==1.3.2

WORKDIR /app

COPY . .

# Build
RUN conan profile detect && \
    conan install . --output-folder=deps --build=missing && \
    meson setup build --buildtype=release && \
    meson compile -C build

# ───────────────────────────────
# Stage 2: Runtime
# ───────────────────────────────
FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y libprotobuf32 libgrpc++1 libspdlog1 libhiredis0.14 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /execution

COPY --from=builder /app/build/execution_service .
COPY .env .

EXPOSE 50051
EXPOSE 8080

CMD ["./execution_service"]
