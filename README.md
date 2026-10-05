# Catch2 template

Provisioned from [`Qode-Fleet-Control/fleet-template-v1`](https://github.com/Qode-Fleet-Control/fleet-template-v1) — the fleet
lifecycle contract (`bin/`, `fleet.conf`, `compose.yaml`, deploy workflows) with a Catch2 v3 starter laid on top.

A small C++17 library (`calc`: gcd, is_prime, fibonacci, split) and its Catch2 v3 test suite, built with CMake and run with CTest. The suite shows `TEST_CASE`s with tags, `SECTION`s, `REQUIRE`/`CHECK`, `CHECK_THROWS_AS`, a data-driven `GENERATE(table<...>)` and matchers (`CHECK_THAT` + `Equals`). The image's default command runs the suite; it exits 0 only when every test passes.

## Origin

    hand-written (Catch2 ships no project generator) — CMakeLists.txt follows Catch2's docs/cmake-integration.md: FetchContent_Declare(Catch2 ...) + FetchContent_MakeAvailable, Catch2::Catch2WithMain, include(CTest) + include(Catch) + catch_discover_tests()


## Run it

### On the fleet

The fleet runs it as containers (the docker runtime): `bin/run` builds the image with
`docker compose build` and then stops — this is a job, so `DOCKER_START_CMD` is empty and nothing listens on `$PORT`.

### With docker

```sh
docker compose build
docker compose run --rm app            # runs the job; exit code = result
```

### Without docker

```sh
# Debian/Ubuntu: sudo apt install build-essential cmake   (CMake downloads Catch2 itself)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
ctest --test-dir build --output-on-failure    # or run ./build/tests directly
```

`fleet.conf` drives every script in `bin/`:

| step | docker runtime (fleet) | `FLEET_RUNTIME=process` |
|---|---|---|
| install | — | `(none)` |
| build | `docker compose build` | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j` |
| start | `(none — a job)` | `(none — a job)` |

## Layout

- `include/calc/calc.h`, `src/calc.cpp` — the library under test (target `calc`).
- `tests/calc_test.cpp` — the suite (target `tests`, each `TEST_CASE` registered with CTest by `catch_discover_tests`).
- `CMakeLists.txt` — Catch2 v3.8.1 by FetchContent, pinned by SHA-256.
- `Dockerfile` — one `debian:trixie` stage: compiles library and tests at build time as non-root user `app`; `CMD ["ctest", "--test-dir", "build", "--output-on-failure"]`.
- `compose.yaml` — service `app`, no ports (a job), fleet variables passed through by name.

## Deviations from stock, and why

- Catch2 is fetched as the v3.8.1 release tarball with a `URL_HASH`, not the docs' `GIT_REPOSITORY`/`GIT_TAG`: the build needs no git and the download is pinned.
- The Docker image is single-stage on purpose: `ctest` needs CMake and the build tree at run time.
- `PORT`, `HEALTH_PATH`, `START_CMD` and `DOCKER_START_CMD` are empty by design: nothing listens on a port, and `bin/run` builds the image and stops there.

## Verified

2026-10-05, Docker 29.8 on linux/amd64, from the scaffold directory:

- `docker compose build` → built (Catch2 fetched and compiled inside the build).
- `docker compose run --rm app` → exit 0: `100% tests passed, 0 tests failed out of 5`.
- `docker compose down --rmi local -v` → clean.

The no-docker path (`FLEET_RUNTIME=process`) was not run on a host toolchain; it is the same CMake build the image runs.

See `docs/fleet-lifecycle.md` for the lifecycle contract.
