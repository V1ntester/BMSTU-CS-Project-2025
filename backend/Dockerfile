FROM gcc:latest AS build

RUN apt-get install -y software-properties-common && \
    add-apt-repository ppa:mhier/libboost-latest -y && \
    apt-get update && \
    apt-get install -y \
    cmake \
    clang-tidy \
    clang-format \
    libboost1.87-dev \
    libboost-system1.87-dev

COPY /source /app/source

WORKDIR /app/build

RUN cmake ../source && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/main .

ENTRYPOINT [ "./main" ]
