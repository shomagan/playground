FROM ubuntu:24.04

WORKDIR /app

RUN apt-get update && apt-get install -y \
        git cmake build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY sources /app/sources

RUN cmake -S /app/sources -B /app/build && cmake --build /app/build

# UDP broadcast discovery on port 65500 requires host networking (Linux only):
#   docker run --network host listen_server:test
CMD ["/app/build/listen_server", "-d", "/app/sources/devices_description.json"]
