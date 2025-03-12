FROM gcc::latest as build

RUN apt-get update && \
    apt-get install -y \
    cmake

ADD ./source /app/source

WORKDIR /app/build

RUN cmake ../source && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/main .

ENTRYPOINT [ "./main" ]
