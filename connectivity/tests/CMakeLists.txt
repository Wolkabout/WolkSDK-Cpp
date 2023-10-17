# Place the sub-folder this target is placed into a variable
set(SUBDIRECTORY_PATH connectivity/tests)

# Include the dependencies
include(cmake/dependencies.cmake)

# Create the target
add_executable(${PROJECT_NAME}Tests)

# Link the libraries
target_link_directories(${PROJECT_NAME}Tests PUBLIC ${CMAKE_PREFIX_PATH}/lib)
target_link_libraries(${PROJECT_NAME}Tests ${CONNECTIVITY_TARGET_NAME} ${UTIL_TARGET_NAME} gtest gmock gtest_main)

# Set some target properties
set_target_properties(${PROJECT_NAME}Tests PROPERTIES OUTPUT_NAME "${NAMESPACE}${PROJECT_NAME}Tests")
set_target_properties(${PROJECT_NAME}Tests PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")

target_sources(${PROJECT_NAME}Tests PRIVATE
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

target_sources(${PROJECT_NAME}Tests PRIVATE
        mocks/ConnectivityServiceMock.h
        mocks/DataProtocolMock.h
        mocks/ErrorProtocolMock.h
        mocks/FileManagementProtocolMock.h
        mocks/FirmwareUpdateProtocolMock.h
        mocks/GatewayPlatformStatusProtocolMock.h
        mocks/GatewayRegistrationProtocolMock.h
        mocks/GatewaySubdeviceProtocolMock.h
        mocks/InboundMessageHandlerMock.h
        mocks/MessageListenerMock.h
        mocks/MessagePersistenceMock.h
        mocks/OutboundMessageHandlerMock.h
        mocks/OutboundRetryMessageHandlerMock.h
        mocks/PahoMqttClientMock.h
        mocks/PersistenceMock.h
        mocks/PlatformStatusProtocolMock.h
        mocks/ProtocolMock.h
        mocks/RegistrationProtocolMock.h)

# Add the private include directories
target_include_directories(${PROJECT_NAME}Tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/${SUBDIRECTORY_PATH})

# Add the system private include directories
target_include_directories(${PROJECT_NAME}Tests
        SYSTEM PRIVATE
        ${CMAKE_PREFIX_PATH}/include)

# Add it to CTest
add_test(NAME "WolkSDK-Cpp_Tests" COMMAND ${PROJECT_NAME}Tests)
