# Stage 1: Build the engine
FROM alpine:latest AS builder

ARG X_WITH_PEID=OFF

# Install build dependencies
RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    qt5-qtbase-dev \
    qt5-qtscript-dev \
    pkgconfig \
    git \
    bash

WORKDIR /ctx
COPY . .

# Copy the engine source (supports DIE-engine root or Detect-It-Easy parent with DIE-engine_src/)
RUN if [ -d /ctx/DIE-engine_src ]; then \
        ln -s /ctx/DIE-engine_src /build; \
    else \
        ln -s /ctx /build; \
    fi && \
    cd /build && \
    if [ ! -f dep/XArchive/CMakeLists.txt ] && [ -d .git ]; then \
        git submodule update --init --depth 1 --recursive || git submodule update --init --recursive; \
    fi

WORKDIR /build

# Build the engine (binaries land in build/release like before)
RUN cmake -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DWITH_PEID="$X_WITH_PEID" \
        -DDIE_BUILD_GUI=OFF \
        -DDIE_BUILD_LITE=OFF \
        -DDIE_BUILD_UPDATER=OFF \
        -DDIE_BUILD_TRANSLATIONS=OFF \
        -DCMAKE_CXX_FLAGS="-DQT_NO_DEBUG_OUTPUT -DQT_NO_GUI" \
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=/build/build/release && \
    cmake --build build --target diec -j"$(nproc)"

# Strip symbols
RUN strip build/release/diec

# --- Surgical Dependency Extraction ---
RUN mkdir -p /dist/bin /dist/lib /dist/share/die /dist/opt/detect-it-easy

# Copy the binary
RUN cp build/release/diec /dist/bin/

# Use ldd to find all dependencies and copy them to /dist/lib
# We handle both /usr/lib and /lib paths
RUN ldd build/release/diec | grep "=> /" | awk '{print $3}' | grep -v "ld-musl" | sort -u | xargs -I {} cp -Lv {} /dist/lib/

# Copy the database and rules from this repo (context) or dep/Detect-It-Easy submodule
RUN DB_SRC="/build/dep/Detect-It-Easy"; \
    if [ -d /ctx/db ]; then DB_SRC="/ctx"; fi && \
    cp -r "$DB_SRC/db" "$DB_SRC/db_custom" "$DB_SRC/yara_rules" /dist/share/die/ && \
    if [ "$X_WITH_PEID" = "ON" ] && [ -d "$DB_SRC/peid_rules" ]; then \
        cp -r "$DB_SRC/peid_rules" /dist/share/die/peid_rules && \
        ln -s /usr/share/die/peid_rules /dist/opt/detect-it-easy/peid_rules && \
        ln -s /usr/share/die/peid_rules/PE /dist/opt/detect-it-easy/peid; \
    else \
        rm -f /dist/share/die/yara_rules/peid.yar; \
    fi && \
    cp -r /build/dep/XInfoDB/info /dist/share/die/info && \
    mkdir -p /dist/share/die/signatures && \
    cp /build/dep/signatures/crypto.db /dist/share/die/signatures/

# Create symlinks in the dist directory
RUN ln -s /usr/share/die/db /dist/opt/detect-it-easy/db && \
    ln -s /usr/share/die/db_custom /dist/opt/detect-it-easy/db_custom && \
    ln -s /usr/share/die/yara_rules /dist/opt/detect-it-easy/yara_rules && \
    ln -s /usr/share/die/info /dist/opt/detect-it-easy/info && \
    ln -s /usr/share/die/signatures /dist/opt/detect-it-easy/signatures

# Stage 2: Final ultra-minimal image
FROM alpine:latest

# Copy everything from the dist directory
COPY --from=builder /dist/bin/ /usr/bin/
COPY --from=builder /dist/lib/ /usr/lib/
COPY --from=builder /dist/share/die/ /usr/share/die/
COPY --from=builder /dist/opt/detect-it-easy/ /opt/detect-it-easy/

# Set environment variables
ENV DIE_DB_PATH=/usr/share/die/db

# Verify
RUN diec --version && diec --showdatabase

ENTRYPOINT ["/usr/bin/diec"]
CMD ["--help"]
