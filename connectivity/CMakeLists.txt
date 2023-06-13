# Include the util dependencies
include(cmake/dependencies.cmake)

# Create the library
add_library(${CONNECTIVITY_TARGET_NAME})
add_library(${NAMESPACE}::${CONNECTIVITY_TARGET_NAME} ALIAS ${CONNECTIVITY_TARGET_NAME})

# Link the libraries
target_link_directories(${CONNECTIVITY_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(${CONNECTIVITY_TARGET_NAME} ${UTIL_TARGET_NAME} paho-mqttpp3 paho-mqtt3as nlohmann_json::nlohmann_json nlohmann_json_schema_validator)

# Add the dependency
add_dependencies(${CONNECTIVITY_TARGET_NAME} pahomqttcpp)

# Set some target properties
set_target_properties(${CONNECTIVITY_TARGET_NAME} PROPERTIES OUTPUT_NAME "${NAMESPACE}${CONNECTIVITY_TARGET_NAME}")
set_target_properties(${CONNECTIVITY_TARGET_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
set_target_properties(${CONNECTIVITY_TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${CONNECTIVITY_TARGET_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

# Add the sources
target_sources(${CONNECTIVITY_TARGET_NAME} PRIVATE core/connectivity/ConnectivityService.cpp
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

# Add the header files
target_sources(${CONNECTIVITY_TARGET_NAME} PRIVATE core/connectivity/ConnectivityService.h
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

# Add the private include directories
target_include_directories(${CONNECTIVITY_TARGET_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/${CONNECTIVITY_SUBDIRECTORY_PATH})

# Add the system private include directories
target_include_directories(${CONNECTIVITY_TARGET_NAME}
        SYSTEM PRIVATE
        ${CMAKE_BINARY_DIR}/include)

# Add the public include directories
target_include_directories(${CONNECTIVITY_TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${CONNECTIVITY_SUBDIRECTORY_PATH}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Include tests if the options is enabled
if (${BUILD_TESTS})
    add_subdirectory(tests)
endif ()
