FROM gcc:latest AS build

RUN apt-get update && \
    apt-get install -y \
    git \
    make \
    cmake \
    clang-tidy \
    clang-format \
    libboost1.74-dev \
    libboost-system1.74-dev \
    nlohmann-json3-dev=3.11.2-2 \
    libpq-dev \ 
    && git clone https://github.com/jtv/libpqxx.git 

RUN cd libpqxx && \
    mkdir build && cd build && \ 
    cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make && \
    make install 

COPY /source /app/source

WORKDIR /app/build

RUN cmake ../source -DCMAKE_PREFIX_PATH=/usr/local && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/main .
COPY --from=build /usr/local/lib/libpqxx* /usr/local/lib/

RUN apt-get update && \
    apt-get install -y \
    libboost-system1.74.0 \
    libboost-filesystem1.74.0 \
    libpq5

ENTRYPOINT [ "./main" ]
