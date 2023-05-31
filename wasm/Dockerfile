FROM emscripten/emsdk:3.1.25

# Install from sources CGLM library
WORKDIR /opt
RUN git clone https://github.com/recp/cglm.git \
  && mkdir /opt/cglm/build
WORKDIR /opt/cglm/build
RUN emcmake cmake .. \
  -DCMAKE_EXE_LINKER_FLAGS="-s STANDALONE_WASM" \
  -DCGLM_STATIC=ON
RUN emmake make install

# Install Bullet Physics
WORKDIR /opt
RUN git clone https://github.com/bulletphysics/bullet3.git \
  && mkdir /opt/bullet3/build
WORKDIR /opt/bullet3/build
RUN emcmake cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_CPU_DEMOS=OFF \
  -DBUILD_EXTRAS=OFF \
  -DBUILD_BULLET2_DEMOS=OFF \
  -DBUILD_UNIT_TESTS=OFF \
  .. \
  && emmake make \
  && emmake make install

