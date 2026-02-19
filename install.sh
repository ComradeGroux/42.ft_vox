#!/bin/bash

mkdir -p deps && cd deps
git clone --depth=1 --branch 3.4 git@github.com:glfw/glfw.git
mkdir -p ../build/glfw && cd ../build/glfw
cmake -DBUILD_SHARED_LIBS=OFF ../../deps/glfw
make -s

cd ../../deps
git clone --depth=1 --branch 1.0.3 git@github.com:g-truc/glm.git
mkdir -p ../build/glm && cd ../build/glm
cmake -DGLM_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF ../../deps/glm
make

cd ../../
python3 -m venv .venv
.venv/bin/pip install glad --quiet
.venv/bin/glad --profile core --api gl=4.6 --generator c --out-path build/glad