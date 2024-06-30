#!/bin/bash

cmake . -B build -G Ninja -DCONV_COMPILE_TESTS=ON

ninja -C build

cp /path/to/your/obj/files/*.obj build/tests/

./build/tests/3dconv_test