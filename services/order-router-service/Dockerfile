FROM gcc:13

RUN apt-get update && apt-get install -y \
    git cmake ninja-build python3-pip libhiredis-dev \
    && pip3 install conan meson

WORKDIR /app
COPY . .

RUN ./build.sh

CMD ["./build/order-router-service"]

