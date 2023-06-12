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

add_executable(${PROJECT_NAME}Tests ${TESTS_SOURCE_FILES} ${TESTS_HEADER_FILES})
target_link_libraries(${PROJECT_NAME}Tests ${PROJECT_NAME} gtest gmock gtest_main Threads::Threads)
target_include_directories(${PROJECT_NAME}Tests PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}Tests SYSTEM PRIVATE ${CMAKE_PREFIX_PATH}/include)
target_include_directories(${PROJECT_NAME}Tests PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(${PROJECT_NAME}Tests PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
add_dependencies(${PROJECT_NAME}Tests libgtest)

enable_testing()
add_test(NAME "WolkSDK-Cpp_Tests" COMMAND ${PROJECT_NAME}Tests)
