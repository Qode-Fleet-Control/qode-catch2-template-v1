# Built by .github/workflows/deploy.yml (context ., file Dockerfile) and pushed
# to Artifact Registry.
#
# A job image, not a server: the default command runs the Catch2 suite with
# CTest and exits 0 only when every test passes. It will never satisfy a $PORT
# health check. One stage on purpose: ctest needs CMake and the build tree at
# run time, so there is nothing to strip into a smaller runtime image.
# The library and its tests are compiled at build time (Catch2 is fetched
# then, by CMake's FetchContent); running the image only runs them.
FROM debian:trixie
RUN apt-get update \
 && apt-get install -y --no-install-recommends build-essential cmake ninja-build ca-certificates \
 && rm -rf /var/lib/apt/lists/* \
 && useradd -r -m -u 10001 app \
 && install -d -o app -g app /app
WORKDIR /app
COPY --chown=app:app . .
USER app
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build
ARG BUILD_ID=""
ENV BUILD_ID=$BUILD_ID
CMD ["ctest", "--test-dir", "build", "--output-on-failure"]
