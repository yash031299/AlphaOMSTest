# ---------- STAGE 1: Build ----------
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake meson ninja-build \
    libspdlog-dev nlohmann-json3-dev pkg-config

WORKDIR /app
COPY . .

RUN meson setup build --buildtype=release
RUN meson compile -C build

# ---------- STAGE 2: Runtime ----------
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libspdlog-dev nlohmann-json3-dev

COPY --from=builder /app/build/audit-log-service /usr/local/bin/audit-log-service
COPY --from=builder /app/data /usr/local/data

WORKDIR /usr/local/bin

CMD ["/usr/local/bin/audit-log-service"]
