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
#include "core/protocol/wolkabout/WolkaboutGatewayPlatformStatusProtocol.h"
#undef private
#undef protected

#include "core/utility/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class WolkaboutGatewayPlatformStatusProtocolTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    void SetUp() override
    {
        protocol = std::unique_ptr<WolkaboutGatewayPlatformStatusProtocol>{new WolkaboutGatewayPlatformStatusProtocol};
    }

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";

    std::unique_ptr<WolkaboutGatewayPlatformStatusProtocol> protocol;
};

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, GetInboundChannelsForDevice)
{
    EXPECT_TRUE(protocol->getInboundChannelsForDevice(DEVICE_KEY).empty());
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/error"}), DEVICE_KEY);
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, GetDeviceType)
{
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/error"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/connection_status"}), MessageType::PLATFORM_CONNECTION_STATUS);
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, GetResponseChannelForAnything)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(MessageType::PLATFORM_CONNECTION_STATUS, DEVICE_KEY).empty());
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, ParseMessageInvalidStatus)
{
    // Make the message with an invalid status
    auto status = PlatformStatusMessage{static_cast<ConnectivityStatus>(0x1234)};

    // Parse the message
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, status), nullptr);
}

TEST_F(WolkaboutGatewayPlatformStatusProtocolTests, ParseMessage)
{
    // Make the message
    auto status = PlatformStatusMessage{ConnectivityStatus::CONNECTED};

    // Parse the message
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, status));
    ASSERT_NE(message, nullptr);

    // Analyze the contents of the message
    EXPECT_EQ(message->getChannel(), "p2d/connection_status");
    EXPECT_EQ(message->getContent(), "CONNECTED");
}
