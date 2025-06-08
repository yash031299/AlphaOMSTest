FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    meson \
    ninja-build \
    libhiredis-dev \
    libspdlog-dev \
    libnlohmann-json-dev \
    libcrow-all-dev

WORKDIR /app
COPY . .

RUN meson setup build && meson compile -C build

CMD ["./build/wallet-sync-service"]
