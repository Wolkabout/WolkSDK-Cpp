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
#include "core/protocol/wolkabout/WolkaboutFirmwareUpdateProtocol.h"
#undef private
#undef protected

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/nlohmann/json.hpp"

#include <gtest/gtest.h>

#include <regex>

using namespace ::testing;
using namespace nlohmann;
using namespace wolkabout;

class WolkaboutFirmwareUpdateProtocolTests : public Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutFirmwareUpdateProtocol>(); }

    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    std::shared_ptr<WolkaboutFirmwareUpdateProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";
};

TEST_F(WolkaboutFirmwareUpdateProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/firmware_update_install") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/firmware_update_abort") !=
                channels.cend());
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/firmware_update_install"}), DEVICE_KEY);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, GetDeviceType)
{
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/" + DEVICE_KEY + "/firmware_update_install"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/" + DEVICE_KEY + "/firmware_update_install"}),
              MessageType::FIRMWARE_UPDATE_INSTALL);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, GetResponseChannelForAnything)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(MessageType::FIRMWARE_UPDATE_INSTALL, DEVICE_KEY).empty());
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, SerializeFirmwareUpdateStatusInvalidStatus)
{
    // Create a message with an invalid status
    auto status = FirmwareUpdateStatusMessage{static_cast<FirmwareUpdateStatus>(1234)};

    // Expect that parsing this message will return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, SerializeFirmwareUpdateStatusInvalidError)
{
    // Create a message with an invalid error
    auto status =
      FirmwareUpdateStatusMessage{FirmwareUpdateStatus::ERROR_UPDATE, static_cast<FirmwareUpdateError>(1234)};

    // Expect that parsing this message will return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, SerializeFirmwareUpdateSingleNonError)
{
    // In this test we expect that even if we set the error, status isn't going to be error and the value of error is
    // going to be ignored while serializing.
    // Make a message with status different that error and an error value
    auto status = FirmwareUpdateStatusMessage{FirmwareUpdateStatus::SUCCESS, FirmwareUpdateError::UNKNOWN};

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/firmware_update_status)");
    const auto payloadRegex = std::regex(R"(\{"status":"\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, SerializeFirmwareUpdateSingleError)
{
    // In this test we expect that when the error is set as status, the error value shows up in the serialized payload.
    // Make a message with status different that error and an error value
    auto status =
      FirmwareUpdateStatusMessage{FirmwareUpdateStatus::ERROR_UPDATE, FirmwareUpdateError::INSTALLATION_FAILED};

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(d2p\/\w+\/firmware_update_status)");
    const auto payloadRegex = std::regex(R"(\{"error":"\w+","status":"\w+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, DeserializeFirmwareInstallNotFirmwareInstallTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/firmware_update_not_install";
    auto payload = "what? me? no?";
    auto message = std::make_shared<wolkabout::Message>(payload, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFirmwareUpdateInstall(message), nullptr);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, DeserializeFirmwareInstallSingle)
{
    // Make a message
    auto topic = "p2d/" + DEVICE_KEY + "/firmware_update_install";
    auto payload = "firmware_file.bin";
    auto message = std::make_shared<wolkabout::Message>(payload, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FirmwareUpdateInstallMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFirmwareUpdateInstall(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the file name is the same
    EXPECT_EQ(parsedMessage->getFile(), payload);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, DeserializeFirmwareAbortNotFirmwareAbortTopic)
{
    // Make a message where the topic is not the right one
    auto topic = "p2d/" + DEVICE_KEY + "/firmware_update_not_abort";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFirmwareUpdateAbort(message), nullptr);
}

TEST_F(WolkaboutFirmwareUpdateProtocolTests, DeserializeFirmwareAbortSingle)
{
    // Make a message
    auto topic = "p2d/" + DEVICE_KEY + "/firmware_update_abort";
    auto message = std::make_shared<wolkabout::Message>(std::string{}, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::shared_ptr<FirmwareUpdateAbortMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseFirmwareUpdateAbort(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the message type is valid
    EXPECT_EQ(parsedMessage->getMessageType(), MessageType::FIRMWARE_UPDATE_ABORT);
}
