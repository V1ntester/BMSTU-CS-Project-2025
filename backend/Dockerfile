FROM gcc:9 AS build

RUN apt-get update && \
    apt-get install -y \
    cmake \
    clang-tidy \
    clang-format \
    libboost1.74-dev \
    libboost-system1.74-dev \
    nlohmann-json3-dev

COPY /source /app/source

WORKDIR /app/build

RUN cmake ../source && \
    cmake --build .

FROM ubuntu:20.04

WORKDIR /app

COPY --from=build /app/build/main .

RUN apt-get update && \
    apt-get install -y \
    libboost-system1.74.0 \
    libboost-filesystem1.74.0 \
    rm -rf /var/lib/apt/lists/*

ENTRYPOINT [ "./main" ]
