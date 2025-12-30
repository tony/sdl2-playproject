# justfile for sdl2-playproject
# https://just.systems/

set shell := ["bash", "-uc"]

# File patterns
watch_files := "find . -type f -not -path '*/\\.*' -and -not -path '*/build/*' | grep -i '.*[.][cc,cpp,c,h,hh]\\|CMakeLists.txt$' 2> /dev/null"

# List all available commands
default:
    @just --list

# ============================================================================
# Build
# ============================================================================

# Build all (default)
all: build

# Build with CMake + Make
build: _mkdir-build
    cd build && cmake ..
    make -C build

# Build with CMake + Ninja (faster)
build-ninja: _mkdir-build
    cd build && cmake -GNinja ..
    ninja -C build

# Debug build with Make
debug: _mkdir-build
    cd build && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    make -C build

# Debug build with Ninja (recommended)
debug-ninja: _mkdir-build
    cd build && cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    ninja -C build

# ============================================================================
# Run
# ============================================================================

# Run the game
run:
    ./build/game

# ============================================================================
# Code Quality
# ============================================================================

# Format code with clang-format (Chromium style)
format:
    clang-format -style=Chromium src/* -i || clang-format38 -style=Chromium src/* -i

# Run clang-tidy for static analysis
tidy:
    clang-tidy -format-style=google -header-filter=. -p build src/*.cpp -fix

# Run cpplint checks
cpplint:
    cpplint --filter=-build/include src/*

# Run cppcheck analysis
cppcheck:
    cppcheck --quiet --enable=all --inconclusive --std=c11 --language=c++ src/*

# ============================================================================
# Watch Commands
# ============================================================================

# Watch files and run cpplint on change
watch-cpplint:
    #!/usr/bin/env bash
    set -euo pipefail
    if command -v entr > /dev/null; then
        {{ watch_files }} | entr -c just cpplint
    else
        just cpplint
        just _entr-warn
    fi

# Watch files and run cppcheck on change
watch-cppcheck:
    #!/usr/bin/env bash
    set -euo pipefail
    if command -v entr > /dev/null; then
        {{ watch_files }} | entr -c just cppcheck
    else
        just cppcheck
        just _entr-warn
    fi

# Watch files and rebuild debug on change
watch-debug:
    #!/usr/bin/env bash
    set -euo pipefail
    if command -v entr > /dev/null; then
        {{ watch_files }} | entr -c just debug-ninja
    else
        just debug-ninja
        just _entr-warn
    fi

# Watch files, rebuild, and run on change
watch-run:
    #!/usr/bin/env bash
    set -euo pipefail
    if command -v entr > /dev/null; then
        {{ watch_files }} | entr -c just debug run
    else
        just debug run
        just _entr-warn
    fi

# ============================================================================
# Dependencies
# ============================================================================

# Install dependencies on macOS
install-osx-deps:
    brew install cmake ninja sdl2 sdl2_image sdl2_ttf entr

# Install dependencies on Ubuntu
install-ubuntu-deps:
    sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

# ============================================================================
# Private helpers
# ============================================================================

[private]
_mkdir-build:
    @test -d ./build || mkdir -p build

[private]
_entr-warn:
    @echo "----------------------------------------------------------"
    @echo "     ! File watching functionality non-operational !      "
    @echo "                                                          "
    @echo "Install entr(1) to automatically run tasks on file change."
    @echo "See https://eradman.com/entrproject/                      "
    @echo "----------------------------------------------------------"
