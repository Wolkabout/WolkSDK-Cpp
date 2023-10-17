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
#include "core/protocol/wolkabout/WolkaboutGatewaySubdeviceProtocol.h"
#undef private
#undef protected

#include "core/model/messages/DeviceRegistrationMessage.h"
#include "core/model/messages/FeedValuesMessage.h"
#include "core/protocol/wolkabout/WolkaboutDataProtocol.h"
#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/protocol/wolkabout/WolkaboutRegistrationProtocol.h"
#include "core/utility/Logger.h"
#include "core/utility/StringUtils.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class WolkaboutGatewaySubdeviceProtocolTests : public ::testing::Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutGatewaySubdeviceProtocol>(); }

    static void SetUpTestCase()
    {
        Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE);
        dataProtocol = std::make_shared<WolkaboutDataProtocol>();
        registrationProtocol = std::make_shared<WolkaboutRegistrationProtocol>();
    }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    static std::shared_ptr<WolkaboutDataProtocol> dataProtocol;

    static std::shared_ptr<WolkaboutRegistrationProtocol> registrationProtocol;

    std::shared_ptr<WolkaboutGatewaySubdeviceProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";

    const std::string SUBDEVICE_KEY = "WOLK_SDK_SUBDEVICE_TEST";
};

std::shared_ptr<WolkaboutDataProtocol> WolkaboutGatewaySubdeviceProtocolTests::dataProtocol;

std::shared_ptr<WolkaboutRegistrationProtocol> WolkaboutGatewaySubdeviceProtocolTests::registrationProtocol;

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(),
                          WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                            DEVICE_KEY + WolkaboutProtocol::CHANNEL_DELIMITER +
                            WolkaboutProtocol::WILDCARD_SINGLE_LEVEL) != channels.cend());
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2g/" + DEVICE_KEY + "/+"}), DEVICE_KEY);
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, GetDeviceType)
{
    // Test with some messages
    EXPECT_EQ(protocol->getDeviceType({"", "p2g/" + DEVICE_KEY + "/+"}), DeviceType::GATEWAY);
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/" + DEVICE_KEY + "/feed_values"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2g/" + DEVICE_KEY + "/feed_values"}), MessageType::FEED_VALUES);
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, GetResponseChannelForAnything)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(MessageType::GATEWAY_SUBDEVICE, DEVICE_KEY).empty());
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, MakeOutboundFromFeedValues)
{
    // Make the feed values message
    auto feed = FeedValuesMessage{{{"T", std::string{"123"}}}};
    auto parsedFeed = dataProtocol->makeOutboundMessage(SUBDEVICE_KEY, feed);
    LogMessage(*parsedFeed);

    // Make the subdevice message
    auto message = GatewaySubdeviceMessage{*parsedFeed};

    // Now parse the message
    auto parsedMessage = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->makeOutboundMessage(DEVICE_KEY, message));
    ASSERT_NE(parsedMessage, nullptr);
    LogMessage(*parsedMessage);
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, MakeOutboundFromPullFeeds)
{
    // Make the pull feed values message
    auto parsedFeed = dataProtocol->makeOutboundMessage(SUBDEVICE_KEY, PullFeedValuesMessage{});
    LogMessage(*parsedFeed);

    // Make the subdevice message
    auto message = GatewaySubdeviceMessage{*parsedFeed};

    // Now parse the message
    auto parsedMessage = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->makeOutboundMessage(DEVICE_KEY, message));
    ASSERT_NE(parsedMessage, nullptr);
    LogMessage(*parsedMessage);
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, MakeOutboundFromRegistrationMessage)
{
    auto registrationMessage = registrationProtocol->makeOutboundMessage(
      DEVICE_KEY, DeviceRegistrationMessage{{{"Test Device", "TD1", "", {}, {}, {}}}});
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, GatewaySubdeviceMessage{*registrationMessage}), nullptr);
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, ParseIncomingRegistrationMessage)
{
    // Make the incoming message
    const auto message = std::make_shared<wolkabout::Message>(
      R"({"timestampFrom":0,"deviceType":null,"externalId":null,"matchingDevices":[]})",
      "p2g/" + DEVICE_KEY + "/registered_devices");
    LogMessage(*message);

    // And parse the message
    auto parsedMessages = std::vector<GatewaySubdeviceMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessages = protocol->parseIncomingSubdeviceMessage(message));
    ASSERT_FALSE(parsedMessages.empty());
    for (const auto& parsedMessage : parsedMessages)
        LogMessage(parsedMessage.getMessage());
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, ParseIncomingSynchronizeParameters)
{
    // Make the incoming message
    const auto message = std::make_shared<wolkabout::Message>(
      R"([{"payload": {"GATEWAY": false, "GATEWAY_PARENT": null, "FIRMWARE_UPDATE_ENABLED": false, "FILE_TRANSFER_URL_ENABLED": false, "FIRMWARE_UPDATE_CHECK_TIME": null, "CONNECTIVITY_TYPE": "GATEWAY", "FIRMWARE_UPDATE_REPOSITORY": null, "FIRMWARE_VERSION": null, "OUTBOUND_DATA_MODE": "PULL", "EXTERNAL_ID": null, "MAXIMUM_MESSAGE_SIZE": 0, "OUTBOUND_DATA_RETENTION_TIME": 0, "FILE_TRANSFER_PLATFORM_ENABLED": false}, "device": "AD1"}])",
      "p2g/ADG/synchronize_parameters");
    LogMessage(*message);

    // And parse the message
    auto parsedMessages = std::vector<GatewaySubdeviceMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessages = protocol->parseIncomingSubdeviceMessage(message));
    ASSERT_FALSE(parsedMessages.empty());
    for (const auto& parsedMessage : parsedMessages)
        LogMessage(parsedMessage.getMessage());
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, ParseSingleMessage)
{
    // Make the incoming message
    const auto message = std::make_shared<wolkabout::Message>(
      R"({"payload": {"GATEWAY": false, "GATEWAY_PARENT": null, "FIRMWARE_UPDATE_ENABLED": false, "FILE_TRANSFER_URL_ENABLED": false, "FIRMWARE_UPDATE_CHECK_TIME": null, "CONNECTIVITY_TYPE": "GATEWAY", "FIRMWARE_UPDATE_REPOSITORY": null, "FIRMWARE_VERSION": null, "OUTBOUND_DATA_MODE": "PULL", "EXTERNAL_ID": null, "MAXIMUM_MESSAGE_SIZE": 0, "OUTBOUND_DATA_RETENTION_TIME": 0, "FILE_TRANSFER_PLATFORM_ENABLED": false}, "device": "AD1"})",
      "p2g/ADG/synchronize_parameters");
    LogMessage(*message);

    // And parse the message
    auto parsedMessages = std::vector<GatewaySubdeviceMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessages = protocol->parseIncomingSubdeviceMessage(message));
    ASSERT_EQ(parsedMessages.size(), 1);
    LogMessage(parsedMessages.front().getMessage());
}

TEST_F(WolkaboutGatewaySubdeviceProtocolTests, ParseIncomingFileBinaryResponse)
{
    // Make the incoming message
    const auto message = std::make_shared<wolkabout::Message>(
      R"([{"payload": ")" + StringUtils::base64Encode(ByteArray(64, 100)) + R"(", "device": "AD1"}])",
      "p2g/" + DEVICE_KEY + "/file_binary_response");
    LogMessage(*message);

    // And parse the message
    auto parsedMessages = std::vector<GatewaySubdeviceMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessages = protocol->parseIncomingSubdeviceMessage(message));
    ASSERT_FALSE(parsedMessages.empty());
    for (const auto& parsedMessage : parsedMessages)
        LogMessage(parsedMessage.getMessage());
}
