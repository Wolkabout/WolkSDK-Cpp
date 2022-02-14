/**
 * Copyright 2022 Wolkabout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <any>
#include <sstream>

#define private public
#define protected public
#include "core/model/messages/AttributeRegistrationMessage.h"
#include "core/model/messages/ChildrenSynchronizationRequestMessage.h"
#include "core/model/messages/ChildrenSynchronizationResponseMessage.h"
#include "core/model/messages/DetailsSynchronizationRequestMessage.h"
#include "core/model/messages/DetailsSynchronizationResponseMessage.h"
#include "core/model/messages/DeviceRegistrationMessage.h"
#include "core/model/messages/DeviceRegistrationResponseMessage.h"
#include "core/model/messages/DeviceRemovalMessage.h"
#include "core/model/messages/ErrorMessage.h"
#include "core/model/messages/FeedRegistrationMessage.h"
#include "core/model/messages/FeedRemovalMessage.h"
#include "core/model/messages/FeedValuesMessage.h"
#include "core/model/messages/FileBinaryRequestMessage.h"
#include "core/model/messages/FileBinaryResponseMessage.h"
#include "core/model/messages/FileDeleteMessage.h"
#include "core/model/messages/FileListRequestMessage.h"
#include "core/model/messages/FileListResponseMessage.h"
#include "core/model/messages/FilePurgeMessage.h"
#include "core/model/messages/FileUploadAbortMessage.h"
#include "core/model/messages/FileUploadInitiateMessage.h"
#include "core/model/messages/FileUploadStatusMessage.h"
#include "core/model/messages/FileUrlDownloadAbortMessage.h"
#include "core/model/messages/FileUrlDownloadInitMessage.h"
#include "core/model/messages/FileUrlDownloadStatusMessage.h"
#include "core/model/messages/FirmwareUpdateAbortMessage.h"
#include "core/model/messages/FirmwareUpdateInstallMessage.h"
#include "core/model/messages/FirmwareUpdateStatusMessage.h"
#include "core/model/messages/GatewaySubdeviceMessage.h"
#include "core/model/messages/ParametersPullMessage.h"
#include "core/model/messages/ParametersUpdateMessage.h"
#include "core/model/messages/PlatformStatusMessage.h"
#include "core/model/messages/PullFeedValuesMessage.h"
#include "core/model/messages/RegisteredDevicesRequestMessage.h"
#include "core/model/messages/RegisteredDevicesResponseMessage.h"
#include "core/model/messages/SynchronizeParametersMessage.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace ::testing;

class ModelMessagesTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }
};

TEST_F(ModelMessagesTests, MessageTypeForAttributeRegistrationMessage)
{
    EXPECT_EQ(AttributeRegistrationMessage{{}}.getMessageType(), MessageType::ATTRIBUTE_REGISTRATION);
}

TEST_F(ModelMessagesTests, AttributeRegistrationMessageGetAttributes)
{
    const auto message = AttributeRegistrationMessage{{{"name", wolkabout::DataType::STRING, "value"}}};
    ASSERT_FALSE(message.getAttributes().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForChildrenSynchronizationRequestMessage)
{
    EXPECT_EQ(ChildrenSynchronizationRequestMessage{}.getMessageType(), MessageType::CHILDREN_SYNCHRONIZATION_REQUEST);
}

TEST_F(ModelMessagesTests, MessageTypeForChildrenSynchronizationResponseMessage)
{
    EXPECT_EQ(ChildrenSynchronizationResponseMessage{{}}.getMessageType(),
              MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE);
}

TEST_F(ModelMessagesTests, ChildrenSynchronizationResponseMessageGetChildren)
{
    const auto message = ChildrenSynchronizationResponseMessage{{"C1"}};
    ASSERT_FALSE(message.getChildren().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForDetailsSynchronizationRequestMessage)
{
    EXPECT_EQ(DetailsSynchronizationRequestMessage{}.getMessageType(), MessageType::DETAILS_SYNCHRONIZATION_REQUEST);
}

TEST_F(ModelMessagesTests, MessageTypeForDetailsSynchronizationResponseMessage)
{
    EXPECT_EQ(DetailsSynchronizationResponseMessage({}, {}).getMessageType(),
              MessageType::DETAILS_SYNCHRONIZATION_RESPONSE);
}

TEST_F(ModelMessagesTests, DetailsSynchronizationResponseMessageGetFeedsAndAttributes)
{
    const auto message = DetailsSynchronizationResponseMessage{{"F1"}, {"A1"}};
    EXPECT_FALSE(message.getFeeds().empty());
    EXPECT_FALSE(message.getAttributes().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForDeviceRegistrationMessage)
{
    EXPECT_EQ(DeviceRegistrationMessage{{}}.getMessageType(), MessageType::DEVICE_REGISTRATION);
}

TEST_F(ModelMessagesTests, DeviceRegistrationMessageGetDevices)
{
    const auto message = DeviceRegistrationMessage{{{"TestDevice", "", "", {}, {}, {}}}};
    EXPECT_FALSE(message.getDevices().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForDeviceRegistrationResponseMessage)
{
    EXPECT_EQ(DeviceRegistrationResponseMessage({}, {}).getMessageType(), MessageType::DEVICE_REGISTRATION_RESPONSE);
}

TEST_F(ModelMessagesTests, DeviceRegistrationResponseMessageGetSuccessAndFailed)
{
    const auto message = DeviceRegistrationResponseMessage{{"D1"}, {"D2"}};
    EXPECT_FALSE(message.getSuccess().empty());
    EXPECT_FALSE(message.getFailed().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForDeviceRemovalMessage)
{
    EXPECT_EQ(DeviceRemovalMessage{{}}.getMessageType(), MessageType::DEVICE_REMOVAL);
}

TEST_F(ModelMessagesTests, DeviceRemovalMessageGetKeys)
{
    const auto message = DeviceRemovalMessage{{"D1"}};
    EXPECT_FALSE(message.getKeys().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForErrorMessage)
{
    EXPECT_EQ(ErrorMessage({}, {}).getMessageType(), MessageType::ERROR);
}

TEST_F(ModelMessagesTests, ErrorMessageGetValues)
{
    const auto message = ErrorMessage{"DeviceKey", "This Is The Error!"};
    EXPECT_EQ(message.getDeviceKey(), "DeviceKey");
    EXPECT_FALSE(message.getMessage().empty());
    EXPECT_GT(message.getArrivalTime().time_since_epoch().count(), 0);
}

TEST_F(ModelMessagesTests, MessageTypeForFeedRegistrationMessage)
{
    EXPECT_EQ(FeedRegistrationMessage{{}}.getMessageType(), MessageType::FEED_REGISTRATION);
}

TEST_F(ModelMessagesTests, FeedRegistrationMessageGetFeeds)
{
    const auto message = FeedRegistrationMessage{{{"", "", FeedType::IN, ""}}};
    EXPECT_FALSE(message.getFeeds().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFeedRemovalMessage)
{
    EXPECT_EQ(FeedRemovalMessage{{}}.getMessageType(), MessageType::FEED_REMOVAL);
}

TEST_F(ModelMessagesTests, FeedRemovalMessageGetReferences)
{
    const auto message = FeedRemovalMessage{{"F1"}};
    EXPECT_FALSE(message.getReferences().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFeedValuesMessage)
{
    EXPECT_EQ(FeedValuesMessage{{}}.getMessageType(), MessageType::FEED_VALUES);
}

TEST_F(ModelMessagesTests, FeedValuesMessageGetReadings)
{
    const auto message = FeedValuesMessage{{Reading{"", std::string{}}}};
    EXPECT_FALSE(message.getReadings().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileBinaryRequestMessage)
{
    EXPECT_EQ(FileBinaryRequestMessage("", 0).getMessageType(), MessageType::FILE_BINARY_REQUEST);
}

TEST_F(ModelMessagesTests, FileBinaryRequestMessageGetNameAndChunkIndex)
{
    const auto message = FileBinaryRequestMessage{"TestName", 123};
    EXPECT_FALSE(message.getName().empty());
    EXPECT_EQ(message.getChunkIndex(), 123);
}

TEST_F(ModelMessagesTests, MessageTypeForFileBinaryResponseMessage)
{
    EXPECT_EQ(FileBinaryResponseMessage{""}.getMessageType(), MessageType::FILE_BINARY_RESPONSE);
}

TEST_F(ModelMessagesTests, FileBinaryResponseMessageGetEverything)
{
    const auto message = FileBinaryResponseMessage{std::string(666, 'A')};
    EXPECT_FALSE(message.getPreviousHash().empty());
    EXPECT_FALSE(message.getData().empty());
    EXPECT_FALSE(message.getCurrentHash().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileDeleteMessage)
{
    EXPECT_EQ(FileDeleteMessage{{}}.getMessageType(), MessageType::FILE_DELETE);
}

TEST_F(ModelMessagesTests, FileDeleteMessageGetFiles)
{
    const auto message = FileDeleteMessage{{"F1"}};
    EXPECT_FALSE(message.getFiles().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileListRequestMessage)
{
    EXPECT_EQ(FileListRequestMessage{}.getMessageType(), MessageType::FILE_LIST_REQUEST);
}

TEST_F(ModelMessagesTests, MessageTypeForFileListResponseMessage)
{
    EXPECT_EQ(FileListResponseMessage{{}}.getMessageType(), MessageType::FILE_LIST_RESPONSE);
}

TEST_F(ModelMessagesTests, FileListResponseMessageGetFiles)
{
    const auto message = FileListResponseMessage{{FileInformation{"F1", 123, "hash"}}};
    EXPECT_FALSE(message.getFiles().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFilePurgeMessage)
{
    EXPECT_EQ(FilePurgeMessage{}.getMessageType(), MessageType::FILE_PURGE);
}

TEST_F(ModelMessagesTests, MessageTypeForFileUploadAbortMessage)
{
    EXPECT_EQ(FileUploadAbortMessage{{}}.getMessageType(), MessageType::FILE_UPLOAD_ABORT);
}

TEST_F(ModelMessagesTests, FileUploadAbortMessageGetName)
{
    const auto message = FileUploadAbortMessage{"TestFile"};
    EXPECT_FALSE(message.getName().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileUploadInitiateMessage)
{
    EXPECT_EQ(FileUploadInitiateMessage("", 0, "").getMessageType(), MessageType::FILE_UPLOAD_INIT);
}

TEST_F(ModelMessagesTests, FileUploadInitiateMessageGetEverything)
{
    const auto message = FileUploadInitiateMessage{"TestFile", 123, "TestHash"};
    EXPECT_FALSE(message.getName().empty());
    EXPECT_EQ(message.getSize(), 123);
    EXPECT_FALSE(message.getHash().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileUploadStatusMessage)
{
    EXPECT_EQ(FileUploadStatusMessage("", FileTransferStatus::FILE_READY).getMessageType(),
              MessageType::FILE_UPLOAD_STATUS);
}

TEST_F(ModelMessagesTests, FileUploadStatusMessageGetEverything)
{
    const auto message = FileUploadStatusMessage{"TestFile", FileTransferError::FILE_SYSTEM_ERROR};
    EXPECT_EQ(message.getName(), "TestFile");
    EXPECT_EQ(message.getStatus(), FileTransferStatus::ERROR);
    EXPECT_EQ(message.getError(), FileTransferError::FILE_SYSTEM_ERROR);
}

TEST_F(ModelMessagesTests, MessageTypeForFileUrlDownloadAbortMessage)
{
    EXPECT_EQ(FileUrlDownloadAbortMessage{""}.getMessageType(), MessageType::FILE_URL_DOWNLOAD_ABORT);
}

TEST_F(ModelMessagesTests, FileUrlDownloadAbortMessageGetEverything)
{
    const auto message = FileUrlDownloadAbortMessage{"TestPath"};
    EXPECT_FALSE(message.getPath().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileUrlDownloadInitMessage)
{
    EXPECT_EQ(FileUrlDownloadInitMessage{""}.getMessageType(), MessageType::FILE_URL_DOWNLOAD_INIT);
}

TEST_F(ModelMessagesTests, FileUrlDownloadInitMessageGetEverything)
{
    const auto message = FileUrlDownloadInitMessage{"TestPath"};
    EXPECT_FALSE(message.getPath().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForFileUrlDownloadStatusMessage)
{
    EXPECT_EQ(FileUrlDownloadStatusMessage("", "", FileTransferStatus::FILE_READY).getMessageType(),
              MessageType::FILE_URL_DOWNLOAD_STATUS);
}

TEST_F(ModelMessagesTests, FileUrlDownloadStatusMessageGetEverything)
{
    const auto message = FileUrlDownloadStatusMessage{"TestPath", FileTransferError::FILE_SYSTEM_ERROR};
    EXPECT_EQ(message.getFileUrl(), "TestPath");
    EXPECT_TRUE(message.getFileName().empty());
    EXPECT_EQ(message.getStatus(), FileTransferStatus::ERROR);
    EXPECT_EQ(message.getError(), FileTransferError::FILE_SYSTEM_ERROR);
}

TEST_F(ModelMessagesTests, MessageTypeForFirmwareUpdateAbortMessage)
{
    EXPECT_EQ(FirmwareUpdateAbortMessage{}.getMessageType(), MessageType::FIRMWARE_UPDATE_ABORT);
}

TEST_F(ModelMessagesTests, MessageTypeForFirmwareUpdateInstallMessage)
{
    EXPECT_EQ(FirmwareUpdateInstallMessage{""}.getMessageType(), MessageType::FIRMWARE_UPDATE_INSTALL);
}

TEST_F(ModelMessagesTests, FirmwareUpdateInstallMessageGetFile)
{
    const auto message = FirmwareUpdateInstallMessage{"TestFile"};
    EXPECT_EQ(message.getFile(), "TestFile");
}

TEST_F(ModelMessagesTests, MessageTypeForFirmwareUpdateStatusMessage)
{
    EXPECT_EQ(FirmwareUpdateStatusMessage{FirmwareUpdateStatus::INSTALLING}.getMessageType(),
              MessageType::FIRMWARE_UPDATE_STATUS);
}

TEST_F(ModelMessagesTests, FirmwareUpdateStatusMessageGetStatusAndError)
{
    const auto message =
      FirmwareUpdateStatusMessage{FirmwareUpdateStatus::ERROR, FirmwareUpdateError::INSTALLATION_FAILED};
    EXPECT_EQ(message.getStatus(), FirmwareUpdateStatus::ERROR);
    EXPECT_EQ(message.getError(), FirmwareUpdateError::INSTALLATION_FAILED);
}

TEST_F(ModelMessagesTests, GatewaySubdeviceMessageGetMessage)
{
    const auto message = GatewaySubdeviceMessage{wolkabout::Message{"TestContent", "TestChannel"}};
    EXPECT_FALSE(message.getMessage().getContent().empty());
    EXPECT_FALSE(message.getMessage().getChannel().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForParametersPullMessage)
{
    EXPECT_EQ(ParametersPullMessage{}.getMessageType(), MessageType::PULL_PARAMETERS);
}

TEST_F(ModelMessagesTests, MessageTypeForParametersUpdateMessage)
{
    EXPECT_EQ(ParametersUpdateMessage{{}}.getMessageType(), MessageType::PARAMETER_SYNC);
}

TEST_F(ModelMessagesTests, ParametersUpdateMessageGetParameters)
{
    const auto message = ParametersUpdateMessage{{{ParameterName::EXTERNAL_ID, "TestExternalId"}}};
    EXPECT_FALSE(message.getParameters().empty());
}

TEST_F(ModelMessagesTests, ConnectivityStatusToString)
{
    EXPECT_EQ(toString(ConnectivityStatus::CONNECTED), "CONNECTED");
    EXPECT_EQ(toString(ConnectivityStatus::OFFLINE), "OFFLINE");
    EXPECT_TRUE(toString(static_cast<ConnectivityStatus>(0x1234)).empty());
}

TEST_F(ModelMessagesTests, ConnectivityStatusFromString)
{
    EXPECT_EQ(connectivityStatusFromString("CONNECTED"), ConnectivityStatus::CONNECTED);
    EXPECT_EQ(connectivityStatusFromString("offline"), ConnectivityStatus::OFFLINE);
    EXPECT_EQ(connectivityStatusFromString("status?"), ConnectivityStatus::NONE);
}

TEST_F(ModelMessagesTests, MessageTypeForPlatformStatusMessage)
{
    EXPECT_EQ(PlatformStatusMessage{ConnectivityStatus::CONNECTED}.getMessageType(),
              MessageType::PLATFORM_CONNECTION_STATUS);
}

TEST_F(ModelMessagesTests, PlatformStatusMessageGetStatus)
{
    const auto message = PlatformStatusMessage{ConnectivityStatus::OFFLINE};
    EXPECT_EQ(message.getStatus(), ConnectivityStatus::OFFLINE);
}

TEST_F(ModelMessagesTests, MessageTypeForPullFeedValuesMessage)
{
    EXPECT_EQ(PullFeedValuesMessage{}.getMessageType(), MessageType::PULL_FEED_VALUES);
}

TEST_F(ModelMessagesTests, MessageTypeForRegisteredDevicesRequestMessage)
{
    EXPECT_EQ(RegisteredDevicesRequestMessage{}.getMessageType(), MessageType::REGISTERED_DEVICES_REQUEST);
}

TEST_F(ModelMessagesTests, RegisteredDevicesRequestMessageGetEverything)
{
    const auto message =
      RegisteredDevicesRequestMessage{std::chrono::milliseconds{100}, "TestDeviceType", "TestExternalId"};
    EXPECT_GT(message.getTimestampFrom().count(), 0);
    EXPECT_FALSE(message.getDeviceType().empty());
    EXPECT_FALSE(message.getExternalId().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForRegisteredDevicesResponseMessage)
{
    EXPECT_EQ(RegisteredDevicesResponseMessage({}, {}, {}, {}).getMessageType(),
              MessageType::REGISTERED_DEVICES_RESPONSE);
}

TEST_F(ModelMessagesTests, RegisteredDevicesResponseMessageGetEverything)
{
    const auto message =
      RegisteredDevicesResponseMessage{std::chrono::milliseconds{123},
                                       "TestDeviceType",
                                       "TestExternalId",
                                       {RegisteredDeviceInformation{"TestKey", "TestExternalId", "TestDeviceType"}}};
    EXPECT_GT(message.getTimestampFrom().count(), 0);
    EXPECT_FALSE(message.getDeviceType().empty());
    EXPECT_FALSE(message.getExternalId().empty());
    EXPECT_FALSE(message.getMatchingDevices().empty());
}

TEST_F(ModelMessagesTests, MessageTypeForSynchronizeParametersMessage)
{
    EXPECT_EQ(SynchronizeParametersMessage{{}}.getMessageType(), MessageType::SYNCHRONIZE_PARAMETERS);
}

TEST_F(ModelMessagesTests, SynchronizeParametersMessageGetParameters)
{
    const auto message = SynchronizeParametersMessage{{ParameterName::EXTERNAL_ID}};
    EXPECT_FALSE(message.getParameters().empty());
}
