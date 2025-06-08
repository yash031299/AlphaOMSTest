# ---------- STAGE 1 ----------
FROM ubuntu:22.04 as builder

RUN apt-get update && apt-get install -y \
    build-essential cmake meson ninja-build \
    libspdlog-dev libboost-all-dev libnlohmann-json-dev pkg-config

WORKDIR /app
COPY . .

RUN meson setup build --buildtype=release
RUN meson compile -C build

# ---------- STAGE 2 ----------
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libspdlog-dev libboost-system-dev libnlohmann-json-dev

COPY --from=builder /app/build/alert-service /usr/local/bin/alert-service
COPY config /config

WORKDIR /usr/local/bin
CMD ["/usr/local/bin/alert-service"]
