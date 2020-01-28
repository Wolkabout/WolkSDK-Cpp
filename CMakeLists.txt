# Copyright 2020 WolkAbout Technology s.r.o.
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

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}    -Wall -Wextra                                                              \
                                             -Wcast-align                                                              \
                                            -Wcast-qual -Wconversion                                                   \
                                            -Wdisabled-optimization                                                    \
                                            -Wfloat-equal -Wformat=2                                                   \
                                            -Werror=init-self                                                          \
                                            -Werror=missing-field-initializers -Wmissing-format-attribute              \
                                            -Wmissing-include-dirs -Wmissing-noreturn                                  \
                                            -Werror=pointer-arith                                                      \
                                            -Wno-packed  -Wno-padded -Wredundant-decls                                 \
                                            -Werror=shadow -Werror=stack-protector                                     \
                                            -Wstrict-aliasing=2 -Wno-switch                                            \
                                            -Werror=unreachable-code -Wunused                                          \
                                            -Wvariadic-macros                                                          \
                                            -Wwrite-strings -Werror=non-virtual-dtor -Werror=return-type")

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)

if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/build")
endif(NOT DEFINED CMAKE_PREFIX_PATH)

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

add_subdirectory(dependencies)

include_directories(SYSTEM "${CMAKE_PREFIX_PATH}/include")
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


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
include_directories(SYSTEM "${CMAKE_PREFIX_PATH}/include")
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
link_directories(${CMAKE_BINARY_DIR}/lib)

# WolkAbout c++ SDK
include_directories(SYSTEM "${SSL_INCLUDE_DIR}" "${CMAKE_PREFIX_PATH}/include") 

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/core/*.h" "${CMAKE_CURRENT_LIST_DIR}/core/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/core/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${SSL_LIBS} paho-mqttpp3 paho-mqtt3as pthread)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")

add_dependencies(${PROJECT_NAME} pahomqttcpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/core")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_PREFIX_PATH}/include" "${CMAKE_PREFIX_PATH}/include/mqtt")

add_subdirectory(cmake)
