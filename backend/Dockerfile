FROM gcc:latest AS build

RUN apt-get update && \
    apt-get install -y \
    cmake \
    clang-tidy \
    clang-format \
    libboost1.74-dev \
    libboost-system1.74-dev \
    nlohmann-json3-dev=3.10.5-1

COPY /source /app/source

WORKDIR /app/build

RUN cmake ../source && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/main .

ENTRYPOINT [ "./main" ]
