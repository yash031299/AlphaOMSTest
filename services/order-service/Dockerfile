FROM gcc:13

WORKDIR /app

# Install system dependencies
RUN apt-get update && apt-get install -y \
    python3 python3-pip \
    pkg-config libhiredis-dev libprotobuf-dev protobuf-compiler \
    libgrpc++-dev grpc-proto libspdlog-dev libboost-all-dev \
    && pip3 install conan meson ninja \
    && conan profile detect

COPY . .

# Install Conan dependencies
RUN conan install . --output-folder=build --build=missing

# Configure & build with Meson
RUN meson setup build --buildtype=release && meson compile -C build

CMD ["./build/order_service"]
