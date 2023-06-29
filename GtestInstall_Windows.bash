#!/usr/bin/env bash

throw_error() {
    echo ""
    echo $1
    exit 1
}

URL=https://github.com/google/googletest.git
TAG=v1.13.0

git clone $URL -b $TAG
if [[ ! $? -eq 0 ]]; then
    throw_error "Error: Unable to clone from $URL with tag $TAG."
fi

mkdir googletest/build
cd googletest/build
trap 'cd ../.. && rm -rf googletest' EXIT

if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
elif command -v make >/dev/null 2>&1; then
    GENERATOR="Unix Makefiles"
elif command -v mingw32-make >/dev/null 2>&1; then
    GENERATOR="MinGW Makefiles"
else
    throw_error "Error: No CMake generator found"
fi

cmake -G "$GENERATOR" ..
if [[ ! $? -eq 0 ]]; then
    throw_error "Error: Failed to create $GENERATOR build system."
fi

cmake --build .
if [[ ! $? -eq 0 ]]; then
    throw_error "Error: Failed to build GoogleTest."
fi

cmake --install .
if [[ ! $? -eq 0 ]]; then
    throw_error "Error: Failed to install GoogleTest. Ensure the shell is run as administrator."
fi

setx GTEST_ROOT "C:\\Program Files (x86)\\googletest-distribution\\"

echo ""
echo "GoogleTest was successfully installed."
