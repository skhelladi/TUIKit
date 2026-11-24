#!/usr/bin/env bash
#
# install.sh - Install dependencies and build TUIKit
# Based on README.md
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

OS="$(uname -s)"

# --- Detect package manager / install helper ---
install_pkg() {
    # install_pkg <cmd-to-check> <brew-name> <apt-name> <yum-name>
    local cmd="$1" brew_name="$2" apt_name="$3" yum_name="$4"
    if command -v "$cmd" >/dev/null 2>&1; then
        return 0
    fi
    echo "Installing $cmd..."
    case "$OS" in
        Darwin)
            if command -v brew >/dev/null 2>&1; then
                brew install "$brew_name"
            else
                echo "Homebrew not found. Please install $cmd manually." >&2
                exit 1
            fi
            ;;
        Linux)
            if command -v apt-get >/dev/null 2>&1; then
                sudo apt-get update && sudo apt-get install -y "$apt_name"
            elif command -v dnf >/dev/null 2>&1; then
                sudo dnf install -y "$yum_name"
            elif command -v yum >/dev/null 2>&1; then
                sudo yum install -y "$yum_name"
            else
                echo "No supported package manager. Please install $cmd manually." >&2
                exit 1
            fi
            ;;
        *)
            echo "Unsupported OS: $OS. Please install $cmd manually." >&2
            exit 1
            ;;
    esac
}

# --- Prerequisites ---
# C++ compiler
if ! command -v g++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1; then
    echo "No C++ compiler found. Installing build-essential toolchain..."
    case "$OS" in
        Darwin) install_pkg clang++ llvm llvm clang ;; # noop-ish; Xcode CLT required
        Linux) sudo apt-get install -y build-essential 2>/dev/null || sudo dnf install -y gcc-c++ 2>/dev/null || true ;;
    esac
fi

install_pkg cmake cmake cmake cmake3
install_pkg git git git git
install_pkg wget wget wget wget
# xxd ships with vim-common on Linux
if ! command -v xxd >/dev/null 2>&1; then
    case "$OS" in
        Darwin) ;; # xxd is part of system (vim) on macOS
        Linux)
            if command -v apt-get >/dev/null 2>&1; then sudo apt-get install -y vim-common
            elif command -v dnf >/dev/null 2>&1; then sudo dnf install -y vim-common
            elif command -v yum >/dev/null 2>&1; then sudo yum install -y vim-common
            fi
            ;;
    esac
fi

# --- External dependencies ---
mkdir -p external

# FTXUI
if [ ! -f external/ftxui/CMakeLists.txt ]; then
    echo "Cloning FTXUI into external/ftxui..."
    rm -rf external/ftxui
    git clone https://github.com/ArthurSonzogni/FTXUI.git external/ftxui
else
    echo "FTXUI already present."
fi

# nlohmann/json
mkdir -p external/json
if [ ! -f external/json/json.hpp ]; then
    echo "Downloading nlohmann/json v3.12.0..."
    wget -q https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp -O external/json/json.hpp
else
    echo "nlohmann/json already present."
fi

# --- Build ---
BUILD_DIR="${BUILD_DIR:-build}"
echo "Configuring CMake in $BUILD_DIR..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=RelWithDebInfo

echo "Building..."
cmake --build "$BUILD_DIR" --parallel

echo
echo "Done. Run examples with:"
echo "  ./$BUILD_DIR/example_1"
echo "  ./$BUILD_DIR/example_2"
