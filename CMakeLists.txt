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

cmake_minimum_required(VERSION 3.5)
project(WolkAboutCore C CXX)

# Options for our library
OPTION(BUILD_SHARED "Build this library as a shared library" ON)
OPTION(BUILD_CONNECTIVITY "Build the library with Paho MQTT and allow MQTT connection to the platform." ON)
OPTION(BUILD_POCO "Build the library with Poco." ON)
OPTION(BUILD_GTEST "Build the library with GTest." ON)
OPTION(BUILD_AWS_LOG_UPLOADER "Build the library with AwsLogUploader." ON)
OPTION(BUILD_TESTS "Build the unit tests" ON)

# Check the GTEST and TESTS option
if (NOT ${BUILD_GTEST} AND ${BUILD_TESTS})
    message(FATAL_ERROR "The library can not build TESTS without GTEST.")
endif ()

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
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif ()

if (NOT DEFINED CMAKE_LIBRARY_INCLUDE_DIRECTORY)
    set(CMAKE_LIBRARY_INCLUDE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
endif ()

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif ()

if (NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/build")
endif ()

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

if (${BUILD_TESTS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
endif ()

if (${BUILD_SHARED})
    set(LIB_TYPE SHARED)
else ()
    set(LIB_TYPE STATIC)
endif ()

# Import the dependencies
add_subdirectory(dependencies)
link_directories(${CMAKE_PREFIX_PATH}/lib)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
include_directories(SYSTEM ${CMAKE_PREFIX_PATH}/include)

find_package(Threads REQUIRED)

# Wolkabout C++ SDK, in their separated packages
set(INSTALL_TARGETS)
set(TESTS_HEADER_FILES)
set(TESTS_SOURCE_FILES)

# Define the version of the library
set(WOLKSDK_VERSION_MAJOR 3)
set(WOLKSDK_VERSION_MINOR 2)
set(WOLKSDK_VERSION_PATCH 0)
set(WOLKSDK_VERSION_SUFFIX)

set(WOLKSDK_VERSION_STRING "${WOLKSDK_VERSION_MAJOR}.${WOLKSDK_VERSION_MINOR}.${WOLKSDK_VERSION_PATCH}")
if (NOT "${WOLKSDK_VERSION_SUFFIX}" STREQUAL "")
    set(WOLKSDK_VERSION_STRING "${WOLKSDK_VERSION_STRING}-${WOLKSDK_VERSION_SUFFIX}")
endif ()

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
        core/utilities/Logger.h
        core/utilities/LogManager.h
        core/utilities/LogUploader.h
        core/utilities/Service.h
        core/utilities/StringUtils.h
        core/utilities/Timer.h)

# path for interface files
file(COPY core/utilities/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/utilities PATTERN *.cpp EXCLUDE)

add_library(${PROJECT_NAME}Utility ${LIB_TYPE} ${UTILITY_SOURCE_FILES} ${UTILITY_HEADER_FILES})
target_link_libraries(${PROJECT_NAME}Utility crypto Threads::Threads)
target_include_directories(${PROJECT_NAME}Utility PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}Utility PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
set_target_properties(${PROJECT_NAME}Utility PROPERTIES INSTALL_RPATH "$ORIGIN")
set_target_properties(${PROJECT_NAME}Utility PROPERTIES VERSION ${WOLKSDK_VERSION_STRING} SOVERSION ${WOLKSDK_VERSION_MAJOR}.${WOLKSDK_VERSION_MINOR}.${WOLKSDK_VERSION_PATCH})
add_dependencies(${PROJECT_NAME}Utility spdlog)

list(APPEND INSTALL_TARGETS ${PROJECT_NAME}Utility)

# Build the AwsLogUploader
if (BUILD_AWS_LOG_UPLOADER)
    set(ALU_SOURCE_FILES core/log_uploader/AwsLogUploader.cpp)
    set(ALU_HEADER_FILES core/log_uploader/AwsLogUploader.h)

    file(COPY core/log_uploader/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/log_uploader PATTERN *.cpp EXCLUDE)

    add_library(${PROJECT_NAME}Aws ${LIB_TYPE} ${ALU_SOURCE_FILES} ${ALU_HEADER_FILES})
    target_link_libraries(${PROJECT_NAME}Aws ${PROJECT_NAME}Utility aws-cpp-sdk-core aws-cpp-sdk-s3)
    target_include_directories(${PROJECT_NAME}Aws PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories(${PROJECT_NAME}Aws PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
    set_target_properties(${PROJECT_NAME}Aws PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${PROJECT_NAME}Aws PROPERTIES VERSION ${WOLKSDK_VERSION_STRING} SOVERSION ${WOLKSDK_VERSION_MAJOR}.${WOLKSDK_VERSION_MINOR}.${WOLKSDK_VERSION_PATCH})
    add_dependencies(${PROJECT_NAME}Aws ${PROJECT_NAME}Utility aws-sdk-cpp)

    list(APPEND INSTALL_TARGETS ${PROJECT_NAME}Aws)
endif ()

# Build the MQTT logic if Paho is enabled
if (BUILD_CONNECTIVITY)
    set(LIB_SOURCE_FILES core/connectivity/ConnectivityService.cpp
            core/connectivity/InboundPlatformMessageHandler.cpp
            core/connectivity/OutboundRetryMessageHandler.cpp
            core/connectivity/mqtt/MqttCallback.cpp
            core/connectivity/mqtt/MqttClient.cpp
            core/connectivity/mqtt/MqttConnectivityService.cpp
            core/connectivity/mqtt/PahoMqttClient.cpp
            core/model/Attribute.cpp
            core/model/Device.cpp
            core/model/Feed.cpp
            core/model/Message.cpp
            core/model/Reading.cpp
            core/model/messages/AttributeRegistrationMessage.cpp
            core/model/messages/ChildrenSynchronizationRequestMessage.cpp
            core/model/messages/ChildrenSynchronizationResponseMessage.cpp
            core/model/messages/DetailsSynchronizationRequestMessage.cpp
            core/model/messages/DetailsSynchronizationResponseMessage.cpp
            core/model/messages/DeviceRegistrationMessage.cpp
            core/model/messages/DeviceRegistrationResponseMessage.cpp
            core/model/messages/DeviceRemovalMessage.cpp
            core/model/messages/ErrorMessage.cpp
            core/model/messages/FeedRegistrationMessage.cpp
            core/model/messages/FeedRemovalMessage.cpp
            core/model/messages/FeedValuesMessage.cpp
            core/model/messages/FileBinaryRequestMessage.cpp
            core/model/messages/FileBinaryResponseMessage.cpp
            core/model/messages/FileDeleteMessage.cpp
            core/model/messages/FileListRequestMessage.cpp
            core/model/messages/FileListResponseMessage.cpp
            core/model/messages/FilePurgeMessage.cpp
            core/model/messages/FileUploadAbortMessage.cpp
            core/model/messages/FileUploadInitiateMessage.cpp
            core/model/messages/FileUploadStatusMessage.cpp
            core/model/messages/FileUrlDownloadAbortMessage.cpp
            core/model/messages/FileUrlDownloadInitMessage.cpp
            core/model/messages/FileUrlDownloadStatusMessage.cpp
            core/model/messages/FirmwareUpdateAbortMessage.cpp
            core/model/messages/FirmwareUpdateInstallMessage.cpp
            core/model/messages/FirmwareUpdateStatusMessage.cpp
            core/model/messages/GatewaySubdeviceMessage.cpp
            core/model/messages/ParametersPullMessage.cpp
            core/model/messages/ParametersUpdateMessage.cpp
            core/model/messages/PlatformStatusMessage.cpp
            core/model/messages/PullFeedValuesMessage.cpp
            core/model/messages/RegisteredDevicesRequestMessage.cpp
            core/model/messages/RegisteredDevicesResponseMessage.cpp
            core/model/messages/PullFeedValuesMessage.cpp
            core/model/messages/SynchronizeParametersMessage.cpp
            core/persistence/filesystem/CircularFileSystemMessagePersistence.cpp
            core/persistence/filesystem/FileSystemMessagePersistence.cpp
            core/persistence/filesystem/MessagePersister.cpp
            core/persistence/inmemory/InMemoryMessagePersistence.cpp
            core/persistence/inmemory/InMemoryPersistence.cpp
            core/protocol/wolkabout/WolkaboutDataProtocol.cpp
            core/protocol/wolkabout/WolkaboutErrorProtocol.cpp
            core/protocol/wolkabout/WolkaboutFileManagementProtocol.cpp
            core/protocol/wolkabout/WolkaboutFirmwareUpdateProtocol.cpp
            core/protocol/wolkabout/WolkaboutGatewayPlatformStatusProtocol.cpp
            core/protocol/wolkabout/WolkaboutGatewayRegistrationProtocol.cpp
            core/protocol/wolkabout/WolkaboutGatewaySubdeviceProtocol.cpp
            core/protocol/wolkabout/WolkaboutPlatformStatusProtocol.cpp
            core/protocol/wolkabout/WolkaboutProtocol.cpp
            core/protocol/wolkabout/WolkaboutRegistrationProtocol.cpp
            core/Types.cpp)
    set(LIB_HEADER_FILES core/connectivity/ConnectivityService.h
            core/connectivity/InboundMessageHandler.h
            core/connectivity/InboundPlatformMessageHandler.h
            core/connectivity/OutboundMessageHandler.h
            core/connectivity/OutboundRetryMessageHandler.h
            core/connectivity/mqtt/MqttCallback.h
            core/connectivity/mqtt/MqttClient.h
            core/connectivity/mqtt/MqttConnectivityService.h
            core/connectivity/mqtt/PahoMqttClient.h
            core/model/Attribute.h
            core/model/Device.h
            core/model/Feed.h
            core/model/Message.h
            core/model/Reading.h
            core/model/messages/AttributeRegistrationMessage.h
            core/model/messages/ChildrenSynchronizationRequestMessage.h
            core/model/messages/ChildrenSynchronizationResponseMessage.h
            core/model/messages/DetailsSynchronizationRequestMessage.h
            core/model/messages/DetailsSynchronizationResponseMessage.h
            core/model/messages/DeviceRegistrationMessage.h
            core/model/messages/DeviceRegistrationResponseMessage.h
            core/model/messages/DeviceRemovalMessage.h
            core/model/messages/ErrorMessage.h
            core/model/messages/FeedRegistrationMessage.h
            core/model/messages/FeedRemovalMessage.h
            core/model/messages/FeedValuesMessage.h
            core/model/messages/FileBinaryRequestMessage.h
            core/model/messages/FileBinaryResponseMessage.h
            core/model/messages/FileDeleteMessage.h
            core/model/messages/FileListRequestMessage.h
            core/model/messages/FileListResponseMessage.h
            core/model/messages/FilePurgeMessage.h
            core/model/messages/FileUploadAbortMessage.h
            core/model/messages/FileUploadInitiateMessage.h
            core/model/messages/FileUploadStatusMessage.h
            core/model/messages/FileUrlDownloadAbortMessage.h
            core/model/messages/FileUrlDownloadInitMessage.h
            core/model/messages/FileUrlDownloadStatusMessage.h
            core/model/messages/FirmwareUpdateAbortMessage.h
            core/model/messages/FirmwareUpdateInstallMessage.h
            core/model/messages/FirmwareUpdateStatusMessage.h
            core/model/messages/GatewaySubdeviceMessage.h
            core/model/messages/MessageModel.h
            core/model/messages/ParametersPullMessage.h
            core/model/messages/ParametersUpdateMessage.h
            core/model/messages/PlatformStatusMessage.h
            core/model/messages/PullFeedValuesMessage.h
            core/model/messages/RegisteredDevicesRequestMessage.h
            core/model/messages/RegisteredDevicesResponseMessage.h
            core/model/messages/SynchronizeParametersMessage.h
            core/persistence/filesystem/CircularFileSystemMessagePersistence.h
            core/persistence/filesystem/FileSystemMessagePersistence.h
            core/persistence/filesystem/MessagePersister.h
            core/persistence/inmemory/InMemoryMessagePersistence.h
            core/persistence/inmemory/InMemoryPersistence.h
            core/persistence/MessagePersistence.h
            core/persistence/Persistence.h
            core/protocol/wolkabout/WolkaboutDataProtocol.h
            core/protocol/wolkabout/WolkaboutErrorProtocol.h
            core/protocol/wolkabout/WolkaboutFileManagementProtocol.h
            core/protocol/wolkabout/WolkaboutFirmwareUpdateProtocol.h
            core/protocol/wolkabout/WolkaboutGatewayPlatformStatusProtocol.h
            core/protocol/wolkabout/WolkaboutGatewayRegistrationProtocol.h
            core/protocol/wolkabout/WolkaboutGatewaySubdeviceProtocol.h
            core/protocol/wolkabout/WolkaboutPlatformStatusProtocol.h
            core/protocol/wolkabout/WolkaboutProtocol.h
            core/protocol/wolkabout/WolkaboutRegistrationProtocol.h
            core/protocol/DataProtocol.h
            core/protocol/ErrorProtocol.h
            core/protocol/FileManagementProtocol.h
            core/protocol/FirmwareUpdateProtocol.h
            core/protocol/GatewayPlatformStatusProtocol.h
            core/protocol/GatewayRegistrationProtocol.h
            core/protocol/GatewaySubdeviceProtocol.h
            core/protocol/PlatformStatusProtocol.h
            core/protocol/Protocol.h
            core/protocol/RegistrationProtocol.h
            core/connectivity/InboundMessageHandler.h
            core/MessageListener.h
            core/Types.h)

    file(GLOB ROOT_HEADERS core/*.h)
    file(COPY ${ROOT_HEADERS} DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core)
    file(COPY core/connectivity/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/connectivity PATTERN *.cpp EXCLUDE)
    file(COPY core/model/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/model PATTERN *.cpp EXCLUDE)
    file(COPY core/persistence/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/persistence PATTERN *.cpp EXCLUDE)
    file(COPY core/protocol/ DESTINATION ${CMAKE_LIBRARY_INCLUDE_DIRECTORY}/core/protocol PATTERN *.cpp EXCLUDE)

    add_library(${PROJECT_NAME} ${LIB_TYPE} ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}Utility paho-mqttpp3 paho-mqtt3as nlohmann_json_schema_validator)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${WOLKSDK_VERSION_STRING} SOVERSION ${WOLKSDK_VERSION_MAJOR}.${WOLKSDK_VERSION_MINOR}.${WOLKSDK_VERSION_PATCH})
    add_dependencies(${PROJECT_NAME} pahomqttcpp json-schema)

    set(TESTS_SOURCE_FILES ${TESTS_SOURCE_FILES}
            tests/BufferTests.cpp
            tests/ByteUtilsTests.cpp
            tests/CommandBufferTests.cpp
            tests/FileSystemUtils.cpp
            tests/InboundPlatformMessageHandlerTests.cpp
            tests/LoggerTests.cpp
            tests/LogManagerTests.cpp
            tests/ModelMessagesTests.cpp
            tests/ModelsTests.cpp
            tests/MqttConnectivityServiceTests.cpp
            tests/OutboundRetryMessageHandlerTests.cpp
            tests/PahoMqttClientTests.cpp
            tests/StringUtilsTests.cpp
            tests/TimerTests.cpp
            tests/TypesTests.cpp
            tests/WolkaboutDataProtocolTests.cpp
            tests/WolkaboutErrorProtocolTests.cpp
            tests/WolkaboutFileManagementProtocolTests.cpp
            tests/WolkaboutFirmwareUpdateProtocolTests.cpp
            tests/WolkaboutGatewayPlatformStatusProtocolTests.cpp
            tests/WolkaboutGatewayRegistrationProtocolTests.cpp
            tests/WolkaboutGatewaySubdeviceProtocolTests.cpp
            tests/WolkaboutPlatformStatusProtocolTests.cpp
            tests/WolkaboutRegistrationProtocolTests.cpp)
    set(TESTS_HEADER_FILES ${TESTS_HEADER_FILES}
            tests/mocks/ConnectivityServiceMock.h
            tests/mocks/DataProtocolMock.h
            tests/mocks/ErrorProtocolMock.h
            tests/mocks/FileManagementProtocolMock.h
            tests/mocks/FirmwareUpdateProtocolMock.h
            tests/mocks/GatewayPlatformStatusProtocolMock.h
            tests/mocks/GatewayRegistrationProtocolMock.h
            tests/mocks/GatewaySubdeviceProtocolMock.h
            tests/mocks/InboundMessageHandlerMock.h
            tests/mocks/MessageListenerMock.h
            tests/mocks/MessagePersistenceMock.h
            tests/mocks/OutboundMessageHandlerMock.h
            tests/mocks/OutboundRetryMessageHandlerMock.h
            tests/mocks/PahoMqttClientMock.h
            tests/mocks/PersistenceMock.h
            tests/mocks/PlatformStatusProtocolMock.h
            tests/mocks/ProtocolMock.h
            tests/mocks/RegistrationProtocolMock.h)

    list(APPEND INSTALL_TARGETS ${PROJECT_NAME})
endif ()

if (${BUILD_TESTS} AND (NOT ("${TESTS_SOURCE_FILES}" STREQUAL "")))
    add_executable(${PROJECT_NAME}Tests ${TESTS_SOURCE_FILES} ${TESTS_HEADER_FILES})
    target_link_libraries(${PROJECT_NAME}Tests ${PROJECT_NAME} gtest gmock gtest_main Threads::Threads)
    target_include_directories(${PROJECT_NAME}Tests PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories(${PROJECT_NAME}Tests PUBLIC ${CMAKE_LIBRARY_INCLUDE_DIRECTORY})
    set_target_properties(${PROJECT_NAME}Tests PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
    add_dependencies(${PROJECT_NAME}Tests libgtest)

    enable_testing()
    add_test(NAME "WolkSDK-Cpp_Tests" COMMAND ${PROJECT_NAME}Tests)
endif ()

# Create the install rule
install(DIRECTORY ${CMAKE_LIBRARY_INCLUDE_DIRECTORY} DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN *.h)
install(DIRECTORY ${CMAKE_PREFIX_PATH}/include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN *.h)
install(DIRECTORY ${CMAKE_PREFIX_PATH}/lib DESTINATION ${CMAKE_INSTALL_PREFIX})
install(TARGETS ${INSTALL_TARGETS} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Add the format rule
file(GLOB_RECURSE ALL_FILES ${CMAKE_SOURCE_DIR}/**/*.cpp ${CMAKE_SOURCE_DIR}/**/*.hpp ${CMAKE_SOURCE_DIR}/**/*.h)
list(FILTER ALL_FILES EXCLUDE REGEX "/out")
list(FILTER ALL_FILES EXCLUDE REGEX "json.hpp")
add_custom_target(format
        COMMAND "clang-format" -i -sort-includes -style=file ${ALL_FILES}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "[Formatting source code]"
        VERBATIM)
