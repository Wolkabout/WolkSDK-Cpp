# Copyright 2021 WolkAbout Technology s.r.o.
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
OPTION(BUILD_AWS_LOG_UPLOADER "Build the library with AwsLogUploader." ON)

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
set(INSTALL_TARGETS)

# Utilities
set(UTILITY_SOURCE_FILES core/utilities/ByteUtils.cpp
        core/utilities/CommandBuffer.cpp
        core/utilities/FileSystemUtils.cpp
        core/utilities/Logger.cpp
        core/utilities/LogManager.cpp
        core/utilities/StringUtils.cpp
        core/utilities/Timer.cpp)
set(UTILITY_HEADER_FILES core/utilities/Buffer.h
        core/utilities/ByteUtils.h
        core/utilities/CommandBuffer.h
        core/utilities/FileSystemUtils.h
        core/utilities/json.hpp
        core/utilities/Logger.h
        core/utilities/LogManager.h
        core/utilities/LogUploader.h
        core/utilities/Service.h
        core/utilities/StringUtils.h
        core/utilities/Timer.h
        core/Version.h)

# path for interface files
file(COPY core/utilities/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/utilities PATTERN *.cpp EXCLUDE)
file(COPY core/Version.h DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/)

add_library(${PROJECT_NAME}Utility SHARED ${UTILITY_SOURCE_FILES} ${UTILITY_HEADER_FILES})
target_link_libraries(${PROJECT_NAME}Utility PUBLIC crypto)
target_include_directories(${PROJECT_NAME}Utility PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}Utility PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
set_target_properties(${PROJECT_NAME}Utility PROPERTIES INSTALL_RPATH "$ORIGIN")
add_dependencies(${PROJECT_NAME}Utility spdlog)

list(APPEND INSTALL_TARGETS ${PROJECT_NAME}Utility)

# Build the AwsLogUploader
if (BUILD_AWS_LOG_UPLOADER)
    set(ALU_SOURCE_FILES core/log_uploader/AwsLogUploader.cpp)
    set(ALU_HEADER_FILES core/log_uploader/AwsLogUploader.h)

    file(COPY core/log_uploader/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/log_uploader PATTERN *.cpp EXCLUDE)

    add_library(${PROJECT_NAME}Aws SHARED ${ALU_SOURCE_FILES} ${ALU_HEADER_FILES})
    target_link_libraries(${PROJECT_NAME}Aws ${PROJECT_NAME}Utility aws-cpp-sdk-core aws-cpp-sdk-s3)
    target_include_directories(${PROJECT_NAME}Aws PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories(${PROJECT_NAME}Aws PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
    set_target_properties(${PROJECT_NAME}Aws PROPERTIES INSTALL_RPATH "$ORIGIN")
    add_dependencies(${PROJECT_NAME}Aws ${PROJECT_NAME}Utility aws-sdk-cpp)

    list(APPEND INSTALL_TARGETS ${PROJECT_NAME}Aws)
endif ()

# Build the MQTT logic if Paho is enabled
if (BUILD_CONNECTIVITY)
    set(LIB_SOURCE_FILES
            core/connectivity/mqtt/MqttCallback.cpp
            core/connectivity/mqtt/MqttClient.cpp
            core/connectivity/mqtt/MqttConnectivityService.cpp
            core/connectivity/mqtt/PahoMqttClient.cpp
            core/connectivity/ConnectivityService.cpp
            core/model/Attribute.cpp
            core/model/Device.cpp
            core/model/Feed.cpp
            core/model/Message.cpp
            core/persistence/inmemory/InMemoryPersistence.cpp
            core/protocol/wolkabout/WolkaboutDataProtocol.cpp
            core/Types.cpp)
    set(LIB_HEADER_FILES core/connectivity/mqtt/MqttCallback.h
            core/connectivity/mqtt/MqttClient.h
            core/connectivity/mqtt/MqttConnectivityService.h
            core/connectivity/mqtt/PahoMqttClient.h
            core/connectivity/ConnectivityService.h
            core/model/Attribute.h
            core/model/Device.h
            core/model/Feed.h
            core/model/Message.h
            core/persistence/inmemory/InMemoryPersistence.h
            core/persistence/Persistence.h
            core/protocol/wolkabout/WolkaboutDataProtocol.h
            core/protocol/DataProtocol.h
            core/protocol/Protocol.h
            core/service/BinaryDataListener.h
            core/service/FirmwareInstaller.h
            core/service/FirmwareUpdateCommandListener.h
            core/InboundMessageHandler.h
            core/Types.h)

    file(COPY core/InboundMessageHandler.h DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core)
    file(COPY core/Types.h DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core)
    file(COPY core/connectivity/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/connectivity PATTERN *.cpp EXCLUDE)
    file(COPY core/model/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/model PATTERN *.cpp EXCLUDE)
    file(COPY core/persistence/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/persistence PATTERN *.cpp EXCLUDE)
    file(COPY core/protocol/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/protocol PATTERN *.cpp EXCLUDE)
    file(COPY core/service/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/service PATTERN *.cpp EXCLUDE)

    add_library(${PROJECT_NAME} SHARED ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}Utility paho-mqttpp3 paho-mqtt3as)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}Utility pahomqttc pahomqttcpp)

    list(APPEND INSTALL_TARGETS ${PROJECT_NAME})
endif ()

# Create the install rule
install(DIRECTORY ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/core/)
install(TARGETS ${INSTALL_TARGETS} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Prepare the format rule
file(GLOB_RECURSE HEADER_FILES ${UTILITY_HEADER_FILES} ${ALU_HEADER_FILES} ${LIB_HEADER_FILES})
file(GLOB_RECURSE SOURCE_FILES ${UTILITY_SOURCE_FILES} ${ALU_SOURCE_FILES} ${LIB_SOURCE_FILES})
add_subdirectory(cmake)
