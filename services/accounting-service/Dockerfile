# ───────────────────────────────────────────────
# Stage 1: Build Stage
# ───────────────────────────────────────────────
FROM debian:bullseye AS builder

RUN apt-get update && apt-get install -y \
    git cmake build-essential ninja-build \
    pkg-config libboost-all-dev libspdlog-dev \
    libpq-dev libpqxx-dev libgrpc++-dev libprotobuf-dev protobuf-compiler-grpc \
    python3 python3-pip meson

# Set working directory
WORKDIR /app

# Copy everything
COPY . .

# Build the project
RUN meson setup builddir && \
    meson compile -C builddir

# ───────────────────────────────────────────────
# Stage 2: Runtime Image
# ───────────────────────────────────────────────
FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    libpq5 libpqxx-7 libboost-system1.74.0 libprotobuf32 && \
    rm -rf /var/lib/apt/lists/*

# Working directory
WORKDIR /app

# Copy only the binary and minimal files
COPY --from=builder /app/builddir/accounting-service /app/accounting-service
COPY --from=builder /app/data /app/data
COPY --from=builder /app/include/httplib.h /app/include/httplib.h

# Expose ports
EXPOSE 6008  
EXPOSE 8081   

# Run the service
ENTRYPOINT ["./accounting-service"]
