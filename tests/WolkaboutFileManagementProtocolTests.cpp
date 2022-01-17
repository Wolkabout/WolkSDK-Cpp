/**
 * Copyright 2022 Adriateh d.o.o.
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

#define private public
#define protected public
#include "core/protocol/wolkabout/WolkaboutFileManagementProtocol.h"
#undef private
#undef protected

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

#include <gtest/gtest.h>

#include <regex>

using namespace ::testing;
using namespace nlohmann;
using namespace wolkabout;

class WolkaboutFileManagementProtocolTests : public Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutFileManagementProtocol>(); }

    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    std::shared_ptr<WolkaboutFileManagementProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";

    const std::string TEST_FILE = "test_file.bin";

    const std::string TEST_URL = "http://test.url/test_file.bin";
};

TEST_F(WolkaboutFileManagementProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutFileManagementProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_upload_initiate") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_upload_abort") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_binary_response") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_url_download_initiate") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_url_download_abort") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_list") != channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_delete") != channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/file_purge") != channels.cend());
}

TEST_F(WolkaboutFileManagementProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/file_upload_initiate"}), DEVICE_KEY);
}

TEST_F(WolkaboutFileManagementProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/" + DEVICE_KEY + "/file_upload_initiate"}),
              MessageType::FILE_UPLOAD_INIT);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUploadStatusEmptyName)
{
    // Make a message with an empty name
    auto status = FileUploadStatusMessage{{}, FileTransferStatus::FILE_READY};

    // Expect that protocol will return nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUploadStatusInvalidStatus)
{
    // Make a message with an invalid status
    auto status = FileUploadStatusMessage{TEST_FILE, static_cast<FileTransferStatus>(1234)};

    // Expect that protocol will return nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUploadStatusInvalidError)
{
    // Make a message with an invalid error
    auto status = FileUploadStatusMessage{TEST_FILE, FileTransferStatus::ERROR, static_cast<FileTransferError>(1234)};

    // Expect that protocol will return nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUploadStatusSingleNonError)
{
    // Make a message with a non-error status
    auto status = FileUploadStatusMessage{TEST_FILE, FileTransferStatus::FILE_READY};

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/file_upload_status)");
    const auto payloadRegex = std::regex(R"(\{"name":"\w+.\w+","status":"\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUploadStatusSingleError)
{
    // Make a message with a non-error status
    auto status =
      FileUploadStatusMessage{TEST_FILE, FileTransferStatus::ERROR, FileTransferError::TRANSFER_PROTOCOL_DISABLED};

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/file_upload_status)");
    const auto payloadRegex = std::regex(R"(\{"error":"\w+","name":"\w+.\w+","status":"\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileBinaryRequestEmptyName)
{
    // Make a message with an empty name
    auto request = FileBinaryRequestMessage{{}, 0};

    // Expect the protocol to return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, request), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileBinaryRequestSingle)
{
    // Make a valid message
    auto request = FileBinaryRequestMessage{TEST_FILE, 3};

    // Make place for the parsed message
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, request));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/file_binary_request)");
    const auto payloadRegex = std::regex(R"(\{"chunkIndex":\d+,"name":"\w+.\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUrlDownloadStatusEmptyName)
{
    // Make a message with an empty name
    auto status = FileUrlDownloadStatusMessage{{}, {}, FileTransferStatus::FILE_READY};

    // Expect that the protocol returns a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUrlDownloadStatusEmptyUrl)
{
    // Make a message with an empty url
    auto status = FileUrlDownloadStatusMessage{{}, TEST_FILE, FileTransferStatus::FILE_READY};

    // Expect that the protocol returns a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUrlDownloadStatusInvalidStatus)
{
    // Make a message with an invalid status
    auto status = FileUrlDownloadStatusMessage{TEST_URL, TEST_FILE, static_cast<FileTransferStatus>(1234)};

    // Expect that the protocol returns a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUrlDownloadStatusInvalidError)
{
    // Make a message with an invalid error
    auto status = FileUrlDownloadStatusMessage{TEST_URL, TEST_FILE, FileTransferStatus::ERROR,
                                               static_cast<FileTransferError>(1234)};

    // Expect that the protocol returns a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUrlDownloadStatusSingleNonError)
{
    // Make a valid message that is not an error
    auto status = FileUrlDownloadStatusMessage{TEST_URL, TEST_FILE, FileTransferStatus::FILE_READY};

    // Make place for the parsed message
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/file_url_download_status)");
    const auto payloadRegex =
      std::regex(R"(\{"fileName":"\w+.\w+","fileUrl":"http://\w+.\w+/\w+.\w+","status":"\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileUrlDownloadStatusSingleError)
{
    // Make a valid message that is not an error
    auto status = FileUrlDownloadStatusMessage{TEST_URL, TEST_FILE, FileTransferStatus::ERROR,
                                               FileTransferError::TRANSFER_PROTOCOL_DISABLED};

    // Make place for the parsed message
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/file_url_download_status)");
    const auto payloadRegex =
      std::regex(R"(\{"error":"\w+","fileName":"\w+.\w+","fileUrl":"http://\w+.\w+/\w+.\w+","status":"\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFileManagementProtocolTests, SerializeFileListResponseSingle)
{
    // Make a valid message
    auto fileInformation = FileInformation{TEST_FILE, UINT16_MAX, TEST_FILE};
    auto fileList = FileListResponseMessage{{fileInformation}};

    // Make place for the parsed message
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, fileList));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/file_list)");
    const auto payloadRegex = std::regex(R"(\[\{"hash":"\w+.\w+","name":"\w+.\w+","size":\d+\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadInitInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_not_initiate";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadInitNotObject)
{
    // Make a message where the payload is not an object
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_initiate";
    auto payload = json::array();
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadInitInvalidName)
{
    // Make a message where the name is missing
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_initiate";
    auto payload = json{};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);

    // Make a message where the name is not a string
    payload = json{{"name", 123}};
    message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadInitInvalidSize)
{
    // Make a message where the size is missing
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_initiate";
    auto payload = json{{"name", TEST_FILE}};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);

    // Make a message where the size is not an unsigned integer
    payload = json{{"name", TEST_FILE}, {"size", "123"}};
    message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadInitInvalidHash)
{
    // Make a message where the hash is missing
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_initiate";
    auto payload = json{{"name", TEST_FILE}, {"size", 123}};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);

    // Make a message where the hash is not a string
    payload = json{{"name", TEST_FILE}, {"size", 123}, {"hash", 123}};
    message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadInit(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadInitSingle)
{
    // Make a message
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_initiate";
    auto payload = json{{"name", TEST_FILE}, {"size", 123}, {"hash", "123"}};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileUploadInitiateMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileUploadInit(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_UPLOAD_INIT);
    EXPECT_EQ(parsedMessage->getName(), TEST_FILE);
    EXPECT_EQ(parsedMessage->getSize(), 123);
    EXPECT_EQ(parsedMessage->getHash(), "123");
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadAbortInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_not_abort";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUploadAbort(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUploadAbortSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_upload_abort";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileUploadAbortMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileUploadAbort(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_UPLOAD_ABORT);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileBinaryResponseInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_binary_not_response";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileBinaryResponse(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileBinaryResponseSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_binary_response";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileBinaryResponseMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileBinaryResponse(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_BINARY_RESPONSE);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUrlDownloadInitInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_url_download_not_initiate";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUrlDownloadInit(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUrlDownloadInitSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_url_download_initiate";
    auto message = std::make_shared<wolkabout::Message>(TEST_URL, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileUrlDownloadInitMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileUrlDownloadInit(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_URL_DOWNLOAD_INIT);
    EXPECT_EQ(parsedMessage->getPath(), TEST_URL);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUrlDownloadAbortInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_url_download_not_abort";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileUrlDownloadAbort(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileUrlDownloadAbortSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_url_download_abort";
    auto message = std::make_shared<wolkabout::Message>(TEST_URL, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileUrlDownloadAbortMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileUrlDownloadAbort(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_URL_DOWNLOAD_ABORT);
    EXPECT_EQ(parsedMessage->getPath(), TEST_URL);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileListRequestInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_not_list";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileListRequest(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileListRequestSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_list";
    auto message = std::make_shared<wolkabout::Message>(TEST_URL, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileListRequestMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileListRequest(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_LIST_REQUEST);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileDeleteInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_not_delete";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileDelete(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileDeleteNotArray)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_delete";
    auto payload = json{};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileDelete(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileDeleteArrayContainsNotString)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_delete";
    auto payload = json::array({1, 2, 3});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFileDelete(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFileDeleteSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_delete";
    auto payload = json::array({TEST_FILE});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FileDeleteMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFileDelete(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_DELETE);
    ASSERT_FALSE(parsedMessage->getFiles().empty());
    EXPECT_EQ(parsedMessage->getFiles().front(), TEST_FILE);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFilePurgeInvalidTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_not_purge";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFilePurge(message), nullptr);
}

TEST_F(WolkaboutFileManagementProtocolTests, DeserializeFilePurgeSingle)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/file_purge";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FilePurgeMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFilePurge(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message values are valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FILE_PURGE);
}
