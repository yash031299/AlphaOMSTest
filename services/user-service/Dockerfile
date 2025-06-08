FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    g++ cmake meson ninja-build \
    libhiredis-dev libspdlog-dev \
    libboost-all-dev pkg-config

WORKDIR /app

COPY . .

RUN meson setup build --buildtype=release && \
    meson compile -C build

FROM ubuntu:22.04 AS runtime

RUN apt-get update && apt-get install -y \
    libhiredis-dev libspdlog-dev libboost-all-dev

COPY --from=build /app/build/user-service /usr/local/bin/user-service

CMD ["/usr/local/bin/user-service"]
