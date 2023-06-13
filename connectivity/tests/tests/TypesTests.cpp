/**
 * Copyright 2023 Wolkabout Technology s.r.o.
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
#include "core/Types.h"
#undef private
#undef protected

#include "core/utility/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class TypesTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }
};

TEST_F(TypesTests, DataTypeTest)
{
    const auto map =
      std::map<DataType, std::string>{{DataType::BOOLEAN, "BOOLEAN"},   {DataType::NUMERIC, "NUMERIC"},
                                      {DataType::STRING, "STRING"},     {DataType::HEXADECIMAL, "HEXADECIMAL"},
                                      {DataType::LOCATION, "LOCATION"}, {DataType::ENUM, "ENUM"},
                                      {DataType::VECTOR, "VECTOR"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);

    // Check the reverse function
    for (const auto& pair : map)
        EXPECT_EQ(dataTypeFromString(pair.second), pair.first);
}

TEST_F(TypesTests, DataTypeCornerCases)
{
    EXPECT_TRUE(toString(static_cast<DataType>(0x1234)).empty());
    EXPECT_EQ(dataTypeFromString({}), DataType::NONE);
}

TEST_F(TypesTests, FeedTypeTest)
{
    const auto map = std::map<FeedType, std::string>{{FeedType::IN, "IN"}, {FeedType::IN_OUT, "IN_OUT"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);

    // Check the reverse function
    for (const auto& pair : map)
        EXPECT_EQ(feedTypeFromString(pair.second), pair.first);
}

TEST_F(TypesTests, FeedTypeCornerCases)
{
    EXPECT_TRUE(toString(static_cast<FeedType>(0x1234)).empty());
    EXPECT_EQ(feedTypeFromString({}), FeedType::NONE);
}

TEST_F(TypesTests, ParameterNameTest)
{
    const auto map = std::map<ParameterName, std::string>{
      {ParameterName::CONNECTIVITY_TYPE, "CONNECTIVITY_TYPE"},
      {ParameterName::OUTBOUND_DATA_MODE, "OUTBOUND_DATA_MODE"},
      {ParameterName::OUTBOUND_DATA_RETENTION_TIME, "OUTBOUND_DATA_RETENTION_TIME"},
      {ParameterName::MAXIMUM_MESSAGE_SIZE, "MAXIMUM_MESSAGE_SIZE"},
      {ParameterName::FILE_TRANSFER_PLATFORM_ENABLED, "FILE_TRANSFER_PLATFORM_ENABLED"},
      {ParameterName::FILE_TRANSFER_URL_ENABLED, "FILE_TRANSFER_URL_ENABLED"},
      {ParameterName::FIRMWARE_UPDATE_ENABLED, "FIRMWARE_UPDATE_ENABLED"},
      {ParameterName::FIRMWARE_UPDATE_CHECK_TIME, "FIRMWARE_UPDATE_CHECK_TIME"},
      {ParameterName::FIRMWARE_UPDATE_REPOSITORY, "FIRMWARE_UPDATE_REPOSITORY"},
      {ParameterName::FIRMWARE_VERSION, "FIRMWARE_VERSION"},
      {ParameterName::GATEWAY, "GATEWAY"},
      {ParameterName::GATEWAY_PARENT, "GATEWAY_PARENT"},
      {ParameterName::EXTERNAL_ID, "EXTERNAL_ID"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);

    // Check the reverse function
    for (const auto& pair : map)
        EXPECT_EQ(parameterNameFromString(pair.second), pair.first);
}

TEST_F(TypesTests, ParameterNameCornerCases)
{
    EXPECT_TRUE(toString(static_cast<ParameterName>(0x1234)).empty());
    EXPECT_EQ(parameterNameFromString({}), ParameterName::UNKNOWN);
}

TEST_F(TypesTests, MessageTypeTest)
{
    const auto map = std::map<MessageType, std::string>{
      {MessageType::FEED_VALUES, "feed_values"},
      {MessageType::PULL_FEED_VALUES, "pull_feed_values"},
      {MessageType::FEED_REGISTRATION, "feed_registration"},
      {MessageType::FEED_REMOVAL, "feed_removal"},
      {MessageType::ATTRIBUTE_REGISTRATION, "attribute_registration"},
      {MessageType::PARAMETER_SYNC, "parameters"},
      {MessageType::PULL_PARAMETERS, "pull_parameters"},
      {MessageType::SYNCHRONIZE_PARAMETERS, "synchronize_parameters"},
      {MessageType::DETAILS_SYNCHRONIZATION_REQUEST, "details_synchronization"},
      {MessageType::DETAILS_SYNCHRONIZATION_RESPONSE, "details_synchronization"},
      {MessageType::TIME_SYNC, "time"},
      {MessageType::FILE_UPLOAD_INIT, "file_upload_initiate"},
      {MessageType::FILE_UPLOAD_STATUS, "file_upload_status"},
      {MessageType::FILE_UPLOAD_ABORT, "file_upload_abort"},
      {MessageType::FILE_BINARY_REQUEST, "file_binary_request"},
      {MessageType::FILE_BINARY_RESPONSE, "file_binary_response"},
      {MessageType::FILE_URL_DOWNLOAD_INIT, "file_url_download_initiate"},
      {MessageType::FILE_URL_DOWNLOAD_ABORT, "file_url_download_abort"},
      {MessageType::FILE_URL_DOWNLOAD_STATUS, "file_url_download_status"},
      {MessageType::FILE_LIST_REQUEST, "file_list"},
      {MessageType::FILE_LIST_RESPONSE, "file_list"},
      {MessageType::FILE_DELETE, "file_delete"},
      {MessageType::FILE_PURGE, "file_purge"},
      {MessageType::FIRMWARE_UPDATE_INSTALL, "firmware_update_install"},
      {MessageType::FIRMWARE_UPDATE_STATUS, "firmware_update_status"},
      {MessageType::FIRMWARE_UPDATE_ABORT, "firmware_update_abort"},
      {MessageType::DEVICE_REGISTRATION, "device_registration"},
      {MessageType::DEVICE_REGISTRATION_RESPONSE, "device_registration_response"},
      {MessageType::DEVICE_REMOVAL, "device_removal"},
      {MessageType::CHILDREN_SYNCHRONIZATION_REQUEST, "children_synchronization"},
      {MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE, "children_synchronization"},
      {MessageType::REGISTERED_DEVICES_REQUEST, "registered_devices"},
      {MessageType::REGISTERED_DEVICES_RESPONSE, "registered_devices"},
      {MessageType::PLATFORM_CONNECTION_STATUS, "connection_status"},
      {MessageType::ERROR_MESSAGE, "error"},
    };

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);

    // Check the reverse function
    const auto duplicates =
      std::vector<MessageType>{MessageType::DETAILS_SYNCHRONIZATION_REQUEST, MessageType::FILE_LIST_RESPONSE,
                               MessageType::CHILDREN_SYNCHRONIZATION_REQUEST, MessageType::REGISTERED_DEVICES_REQUEST};
    for (const auto& pair : map)
    {
        if (std::find(duplicates.cbegin(), duplicates.cend(), pair.first) != duplicates.cend())
            continue;
        EXPECT_EQ(messageTypeFromString(pair.second), pair.first);
    }
}

TEST_F(TypesTests, MessageTypeCornerCases)
{
    EXPECT_TRUE(toString(static_cast<MessageType>(0x1234)).empty());
    EXPECT_EQ(messageTypeFromString({}), MessageType::UNKNOWN);
}

TEST_F(TypesTests, FileTransferStatusTest)
{
    const auto map = std::map<FileTransferStatus, std::string>{{FileTransferStatus::AWAITING_DEVICE, "AWAITING_DEVICE"},
                                                               {FileTransferStatus::FILE_TRANSFER, "FILE_TRANSFER"},
                                                               {FileTransferStatus::FILE_READY, "FILE_READY"},
                                                               {FileTransferStatus::ERROR_TRANSFER, "ERROR"},
                                                               {FileTransferStatus::ABORTED, "ABORTED"},
                                                               {FileTransferStatus::UNKNOWN, "UNKNOWN"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);
}

TEST_F(TypesTests, FileTransferStatusCornerCases)
{
    EXPECT_TRUE(toString(static_cast<FileTransferStatus>(0x1234)).empty());
}

TEST_F(TypesTests, FileTransferErrorTest)
{
    const auto map = std::map<FileTransferError, std::string>{
      {FileTransferError::UNKNOWN, "UNKNOWN"},
      {FileTransferError::TRANSFER_PROTOCOL_DISABLED, "TRANSFER_PROTOCOL_DISABLED"},
      {FileTransferError::UNSUPPORTED_FILE_SIZE, "UNSUPPORTED_FILE_SIZE"},
      {FileTransferError::MALFORMED_URL, "MALFORMED_URL"},
      {FileTransferError::FILE_HASH_MISMATCH, "FILE_HASH_MISMATCH"},
      {FileTransferError::FILE_SYSTEM_ERROR, "FILE_SYSTEM_ERROR"},
      {FileTransferError::RETRY_COUNT_EXCEEDED, "RETRY_COUNT_EXCEEDED"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);
}

TEST_F(TypesTests, FileTransferErrorCornerCases)
{
    EXPECT_TRUE(toString(static_cast<FileTransferError>(0x1234)).empty());
}

TEST_F(TypesTests, FirmwareUpdateStatusTest)
{
    const auto map =
      std::map<FirmwareUpdateStatus, std::string>{{FirmwareUpdateStatus::AWAITING_DEVICE, "AWAITING_DEVICE"},
                                                  {FirmwareUpdateStatus::INSTALLING, "INSTALLING"},
                                                  {FirmwareUpdateStatus::SUCCESS, "SUCCESS"},
                                                  {FirmwareUpdateStatus::ERROR_UPDATE, "ERROR"},
                                                  {FirmwareUpdateStatus::ABORTED, "ABORTED"},
                                                  {FirmwareUpdateStatus::UNKNOWN, "UNKNOWN"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);
}

TEST_F(TypesTests, FirmwareUpdateStatusCornerCases)
{
    EXPECT_TRUE(toString(static_cast<FirmwareUpdateStatus>(0x1234)).empty());
}

TEST_F(TypesTests, FirmwareUpdateErrorTest)
{
    const auto map =
      std::map<FirmwareUpdateError, std::string>{{FirmwareUpdateError::UNKNOWN, "UNKNOWN"},
                                                 {FirmwareUpdateError::UNKNOWN_FILE, "UNKNOWN_FILE"},
                                                 {FirmwareUpdateError::INSTALLATION_FAILED, "INSTALLATION_FAILED"}};

    // Check the `toString` function
    for (const auto& pair : map)
        EXPECT_EQ(toString(pair.first), pair.second);
}

TEST_F(TypesTests, FirmwareUpdateErrorCornerCases)
{
    EXPECT_TRUE(toString(static_cast<FirmwareUpdateError>(0x1234)).empty());
}
