# Stage 1: Build the engine
FROM alpine:latest AS builder

# Install build dependencies
RUN apk add --no-cache \
    build-base \
    qt5-qtbase-dev \
    qt5-qtscript-dev \
    qt5-qttools-dev \
    qt5-qtsvg-dev \
    pkgconfig \
    git \
    make \
    bash

WORKDIR /build

# Copy the engine source
COPY DIE-engine_src .

# Fix the variable shadowing bug
RUN sed -i 's/QString sResult = qApp->property/sResult = qApp->property/g' XOptions/xoptions.cpp

# Initialize the build environment
RUN cp -f build_tools/build.pri .

# Build the engine
RUN qmake die_source.pro "DEFINES+=QT_NO_DEBUG_OUTPUT" && \
    make -j$(nproc)

# Strip symbols
RUN strip build/release/diec

# --- Surgical Dependency Extraction ---
RUN mkdir -p /dist/bin /dist/lib /dist/share/die /dist/opt/detect-it-easy

# Copy the binary
RUN cp build/release/diec /dist/bin/

# Use ldd to find all dependencies and copy them to /dist/lib
# We handle both /usr/lib and /lib paths
RUN ldd build/release/diec | grep "=> /" | awk '{print $3}' | xargs -I {} cp -v {} /dist/lib/

# Copy the database and rules from this repo (context)
WORKDIR /build-context
COPY . .
RUN cp -r db db_extra db_custom yara_rules /dist/share/die/ && \
    cp -r DIE-engine_src/XInfoDB/info /dist/share/die/info && \
    mkdir -p /dist/share/die/signatures && \
    cp DIE-engine_src/signatures/crypto.db /dist/share/die/signatures/

# Create symlinks in the dist directory
RUN ln -s /usr/share/die/db /dist/opt/detect-it-easy/db && \
    ln -s /usr/share/die/db_extra /dist/opt/detect-it-easy/db_extra && \
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
RUN diec --showdatabase

ENTRYPOINT ["/usr/bin/diec"]
CMD ["--help"]
