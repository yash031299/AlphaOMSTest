# ─────────────────────────────
# Stage 1: Build Stage
# ─────────────────────────────
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake meson ninja-build pkg-config \
    libspdlog-dev libboost-all-dev libnlohmann-json-dev \
    libsqlite3-dev libhiredis-dev

WORKDIR /app
COPY . .

RUN meson setup build --buildtype=release
RUN meson compile -C build

# ─────────────────────────────
# Stage 2: Runtime Stage
# ─────────────────────────────
FROM ubuntu:22.04

# Install only runtime dependencies (not dev headers)
RUN apt-get update && apt-get install -y \
    libboost-system-dev libboost-thread-dev \
    libspdlog-dev libnlohmann-json-dev \
    libsqlite3-0 libhiredis-dev

# Default environment values (overridable by .env)
ENV REDIS_HOST=127.0.0.1 \
    REDIS_PORT=6379 \
    ALERT_RULES=/config/alert_rules.json \
    SQLITE_DB=alerts.db

# Copy compiled binary + config
COPY --from=builder /app/build/alert-service /usr/local/bin/alert-service
COPY config /config
COPY .env /usr/local/bin/.env

# Optional: entrypoint script for .env loading
COPY docker-entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

WORKDIR /usr/local/bin
ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
