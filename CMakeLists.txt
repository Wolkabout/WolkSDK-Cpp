# Copyright 2018 WolkAbout Technology s.r.o.
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

cmake_minimum_required(VERSION 3.5)
project(WolkAboutCore C CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}    -Wall -Wextra -pedantic                                                    \
                                            -pedantic-errors -Wcast-align                                              \
                                            -Wcast-qual -Wconversion                                                   \
                                            -Wdisabled-optimization                                                    \
                                            -Wfloat-equal -Wformat=2                                                   \
                                            -Werror=import  -Werror=init-self                                          \
                                            -Werror=missing-field-initializers -Wmissing-format-attribute              \
                                            -Wmissing-include-dirs -Wmissing-noreturn                                  \
                                            -Werror=pointer-arith                                                      \
                                            -Wno-packed  -Wno-padded -Wredundant-decls                                 \
                                            -Werror=shadow -Werror=stack-protector                                     \
                                            -Wstrict-aliasing=2 -Wno-switch                                                       \
                                            -Werror=unreachable-code -Wunused                                          \
                                            -Wvariadic-macros                                                          \
                                            -Wwrite-strings -Werror=non-virtual-dtor -Werror=return-type")

# ssl
IF (NOT SSL_INCLUDE_DIR)
    message(FATAL_ERROR
            "SSL include dir must be specified with: -DSSL_INCLUDE_DIR=...")
ENDIF (NOT SSL_INCLUDE_DIR)

IF (NOT IS_ABSOLUTE ${SSL_INCLUDE_DIR})
    set(SSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/${SSL_INCLUDE_DIR}")
ENDIF (NOT IS_ABSOLUTE ${SSL_INCLUDE_DIR})

IF (NOT SSL_LIBS)
    message(FATAL_ERROR
            "SSL link path must be specified with: -DSSL_LIBS=...")
ENDIF (NOT SSL_LIBS)

add_definitions(-DOPENSSL)
add_subdirectory(mqtt lib)

link_directories(${CMAKE_BINARY_DIR}/lib)

# WolkAbout c++ SDK
include_directories("mqtt")
include_directories("core")
include_directories(SYSTEM "${SSL_INCLUDE_DIR}")

file(GLOB_RECURSE HEADER_FILES "core/*.h" "core/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "core/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SSL_LIBS} paho-mqtt pthread)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")

add_subdirectory(cmake)
