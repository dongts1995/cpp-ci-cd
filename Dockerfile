FROM ubuntu:24.04 AS builder

WORKDIR /app

RUN apt-get update && \
    apt-get install -y \
    g++ \
    cmake \
    make \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release

RUN cmake --build build --config Release


FROM ubuntu:24.04 AS runtime

WORKDIR /app

COPY --from=builder /app/build/cpp_ci_cd .

CMD ["./cpp_ci_cd"]