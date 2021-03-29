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

# Options for our library
OPTION(BUILD_CONNECTIVITY "Build the library with Paho MQTT and allow MQTT connection to the platform." ON)
OPTION(BUILD_POCO "Build the library with Poco." ON)
OPTION(BUILD_GTEST "Build the library with GTest." ON)

# Options for Poco
if (${BUILD_POCO})

    # The Utility bundle
    OPTION(POCO_BUILD_UTIL "" ON)
    if (${POCO_BUILD_UTIL})
        OPTION(POCO_ENABLE_UTIL "" ON)
    endif ()

    # The Network bundle
    OPTION(POCO_BUILD_NET "" OFF)
    if (${POCO_BUILD_NET})
        OPTION(POCO_ENABLE_NET "" ON)
        OPTION(POCO_ENABLE_NETSSL "" ON)
        OPTION(POCO_ENABLE_CRYPTO "" ON)
        if (WIN32)
            OPTION(POCO_ENABLE_NETSSL_WIN "" OFF)
        else ()
            OPTION(POCO_ENABLE_NETSSL_WIN "" ON)
        endif ()
    endif ()

    # The Data bundle
    OPTION(POCO_BUILD_DATA "" OFF)
    if (${POCO_BUILD_DATA})
        OPTION(POCO_ENABLE_DATA "" ON)
        OPTION(POCO_ENABLE_DATA_SQLITE "" ON)
        OPTION(POCO_ENABLE_DATA_MYSQL "" OFF)
        OPTION(POCO_ENABLE_DATA_ODBC "" ON)
    endif ()

    # The Encodings bundle
    OPTION(POCO_BUILD_ENCODINGS "" OFF)
    if (${POCO_BUILD_ENCODINGS})
        OPTION(POCO_ENABLE_ENCODINGS "" ON)
        OPTION(POCO_ENABLE_ENCODINGS_COMPILER "" ON)
        OPTION(POCO_ENABLE_XML "" ON)
        OPTION(POCO_ENABLE_JSON "" ON)
    endif ()

    # The MongoDB bundle
    OPTION(POCO_BUILD_MONGODB "" OFF)
    if (${POCO_BUILD_MONGODB})
        OPTION(POCO_ENABLE_MONGODB "" ON)
    endif ()

    # The Redis bundle
    OPTION(POCO_BUILD_REDIS "" OFF)
    if (${POCO_BUILD_REDIS})
        OPTION(POCO_ENABLE_REDIS "" ON)
    endif ()

    # The ZIP bundle
    OPTION(POCO_BUILD_ZIP "" OFF)
    if (${POCO_BUILD_ZIP})
        OPTION(POCO_ENABLE_SEVENZIP "" ON)
        OPTION(POCO_ENABLE_ZIP "" ON)
    endif ()

    # The PageCompiler bundle
    OPTION(POCO_BUILD_PAGECOMPILER "" OFF)
    if (${POCO_BUILD_PAGECOMPILER})
        OPTION(POCO_ENABLE_PAGECOMPILER "" ON)
        OPTION(POCO_ENABLE_PAGECOMPILER_FILE2PAGE "" ON)
    endif ()

    # The PDF bundle
    OPTION(POCO_BUILD_PDF "" OFF)
    if (${POCO_BUILD_PDF})
        OPTION(POCO_ENABLE_PDF "" ON)
    endif ()

    # The Apache connection bundle
    OPTION(POCO_BUILD_APACHECONNECTOR "" OFF)
    if (${POCO_BUILD_APACHECONNECTOR})
        OPTION(POCO_ENABLE_APACHECONNECTOR "" ON)
    endif ()

    # The CppParser bundle
    OPTION(POCO_BUILD_CPPPARSER "" OFF)
    if (${POCO_BUILD_CPPPARSER})
        OPTION(POCO_ENABLE_CPPPARSER "" ON)
    endif ()

    # The PocoDoc bundle
    OPTION(POCO_BUILD_POCODOC "" OFF)
    if (${POCO_BUILD_POCODOC})
        OPTION(POCO_ENABLE_POCODOC "" ON)
    endif ()
endif ()

# Configure the paths for output
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif ()

if (NOT DEFINED CMAKE_LIBRARY_INCLUDE_DIRECTORY)
    set(CMAKE_LIBRARY_INCLUDE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
endif ()

if (NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/build")
endif ()

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

add_subdirectory(dependencies)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}     -Wall -Wextra -pedantic                                                    \
                                            -pedantic-errors -Wcast-align                                              \
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

include_directories(SYSTEM "${CMAKE_PREFIX_PATH}/include")
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

find_package(Threads REQUIRED)

# WolkAbout C++ SDK, in their separated packages
include_directories(core)
set(INSTALL_TARGETS)

# Utilities
file(GLOB_RECURSE UTILITY_HEADER_FILES core/utilities/*.h core/utilities/*.hpp core/Version.h)
file(GLOB_RECURSE UTILITY_SOURCE_FILES core/utilities/*.cpp)

add_library(${PROJECT_NAME}Utility SHARED ${UTILITY_SOURCE_FILES} ${UTILITY_HEADER_FILES})
target_include_directories(${PROJECT_NAME}Utility PUBLIC core/utilities)
target_link_libraries(${PROJECT_NAME}Utility PUBLIC crypto)
set_target_properties(${PROJECT_NAME}Utility PROPERTIES INSTALL_RPATH "$ORIGIN")
add_dependencies(${PROJECT_NAME}Utility spdlog)

list(APPEND INSTALL_TARGETS ${PROJECT_NAME}Utility)

# Build the MQTT logic if Paho is enabled
if (BUILD_CONNECTIVITY)
    file(GLOB CORE_HEADER_FILES core/*.h)
    file(GLOB_RECURSE CONNECTIVITY_HEADER_FILES core/connectivity/*.h)
    file(GLOB_RECURSE MODEL_HEADER_FILES core/model/*.h)
    file(GLOB_RECURSE PERSISTENCE_HEADER_FILES core/persistence/*.h)
    file(GLOB_RECURSE PROTOCOL_HEADER_FILES core/protocol/*.h)
    file(GLOB_RECURSE SERVICE_HEADER_FILES core/service/*.h)
    file(GLOB_RECURSE MQTT_SOURCE_FILES core/connectivity/*.cpp
            core/model/*.cpp
            core/persistence/*.cpp
            core/protocol/*.cpp
            core/service/*.cpp)

    add_library(${PROJECT_NAME} SHARED ${MQTT_SOURCE_FILES} ${CORE_HEADER_FILES} ${CONNECTIVITY_HEADER_FILES}
            ${MODEL_HEADER_FILES} ${PERSISTENCE_HEADER_FILES} ${PROTOCOL_HEADER_FILES} ${SERVICE_HEADER_FILES})
    target_include_directories(${PROJECT_NAME} PUBLIC core core/connectivity core/model core/persistence core/protocol core/service)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}Utility paho-mqttpp3 paho-mqtt3as)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}Utility spdlog pahomqttc pahomqttcpp)

    list(APPEND INSTALL_TARGETS ${PROJECT_NAME})
endif ()

# Create the install rule
install(TARGETS ${INSTALL_TARGETS} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
if (BUILD_CONNECTIVITY)
    install(FILES ${CORE_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk)
    install(FILES ${CONNECTIVITY_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk/connectivity)
    install(FILES ${MODEL_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk/model)
    install(FILES ${PERSISTENCE_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk/persistence)
    install(FILES ${PROTOCOL_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk/protocol)
    install(FILES ${SERVICE_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk/service)
endif ()
install(FILES ${UTILITY_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/wolksdk/utilities)

# Prepare the format rule
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/core/*.h" "${CMAKE_CURRENT_LIST_DIR}/core/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/core/*.cpp")
add_subdirectory(cmake)
