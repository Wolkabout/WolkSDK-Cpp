#!/usr/bin/env bash

# Copyright 2022 Wolkabout Technology s.r.o.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Create the directory used
mkdir -p out

# Add the pre-commit hook to check code format with .clang-format
cp tools/git/pre-commit .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit

# Enter the directory and start the CMake build
pushd out || exit
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
popd || exit
