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
#include "core/protocol/wolkabout/WolkaboutPlatformStatusProtocol.h"
#undef private
#undef protected

#include "core/utility/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;

class WolkaboutPlatformStatusProtocolTests : public ::testing::Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutPlatformStatusProtocol>(); }

    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    std::shared_ptr<WolkaboutPlatformStatusProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";
};

TEST_F(WolkaboutPlatformStatusProtocolTests, GetInboundChannels)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannels());
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/connection_status") != channels.cend());
}

TEST_F(WolkaboutPlatformStatusProtocolTests, GetInboundChannelsForDevice)
{
    EXPECT_TRUE(protocol->getInboundChannelsForDevice(DEVICE_KEY).empty());
}

TEST_F(WolkaboutPlatformStatusProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/platform_status"}), DEVICE_KEY);
}

TEST_F(WolkaboutPlatformStatusProtocolTests, GetDeviceType)
{
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/" + DEVICE_KEY + "/platform_status"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutPlatformStatusProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/connection_status"}), MessageType::PLATFORM_CONNECTION_STATUS);
}

TEST_F(WolkaboutPlatformStatusProtocolTests, GetResponseChannelForAnything)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(MessageType::PLATFORM_CONNECTION_STATUS, DEVICE_KEY).empty());
}

TEST_F(WolkaboutPlatformStatusProtocolTests, DeserializePlatformStatusNotRightTopic)
{
    // Make a message with the topic that is not right
    auto topic = "p2d/is_platform_here";
    auto payload = "CONNECTED";
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parsePlatformStatusMessage(message), nullptr);
}

TEST_F(WolkaboutPlatformStatusProtocolTests, DeserializePlatformStatusInvalidStatus)
{
    // Make a message with the topic that is not right
    auto topic = "p2d/connection_status";
    auto payload = "BEEPS_AND_BOOPS";
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parsePlatformStatusMessage(message), nullptr);
}

TEST_F(WolkaboutPlatformStatusProtocolTests, DeserializePlatformStatusValid)
{
    // Make a completely valid message
    auto topic = "p2d/connection_status";
    auto payload = "CONNECTED";
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::unique_ptr<PlatformStatusMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parsePlatformStatusMessage(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check that the connection status is valid
    EXPECT_EQ(parsedMessage->getStatus(), ConnectivityStatus::CONNECTED);
}
