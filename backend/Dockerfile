FROM gcc:latest AS build

RUN apt-get update && \
    apt-get install -y \
    cmake \
    clang-tidy \
    clang-format \
    libboost-dev \
    libboost-system-dev

COPY /source /app/source

WORKDIR /app/build

RUN cmake ../source && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/main .

ENTRYPOINT [ "./main" ]
