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
#include "core/protocol/wolkabout/WolkaboutErrorProtocol.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class WolkaboutErrorProtocolTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    void SetUp() override { protocol = std::unique_ptr<WolkaboutErrorProtocol>{new WolkaboutErrorProtocol}; }

    std::unique_ptr<WolkaboutErrorProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";
};

TEST_F(WolkaboutErrorProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutErrorProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/error") != channels.cend());
}

TEST_F(WolkaboutErrorProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/error"}), DEVICE_KEY);
}

TEST_F(WolkaboutErrorProtocolTests, GetDeviceType)
{
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/" + DEVICE_KEY + "/error"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutErrorProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/" + DEVICE_KEY + "/error"}), MessageType::ERROR_MESSAGE);
}

TEST_F(WolkaboutErrorProtocolTests, GetResponseChannelForAnything)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(MessageType::ERROR_MESSAGE, DEVICE_KEY).empty());
}

TEST_F(WolkaboutErrorProtocolTests, ParseErrorMessageTypeIsNotError)
{
    ASSERT_EQ(protocol->parseError(std::make_shared<wolkabout::Message>("", "p2d/" + DEVICE_KEY + "/what?")), nullptr);
}

TEST_F(WolkaboutErrorProtocolTests, ParseErrorMessageEmptyDeviceKey)
{
    ASSERT_EQ(protocol->parseError(std::make_shared<wolkabout::Message>("", "p2d//error")), nullptr);
}

TEST_F(WolkaboutErrorProtocolTests, ParseError)
{
    // Make the message that needs to be parsed
    const auto error = "This is what happened!";
    auto message = std::make_shared<wolkabout::Message>(error, "p2d/" + DEVICE_KEY + "/error");

    // Parse the message
    auto parsedMessage = std::unique_ptr<ErrorMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseError(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check some values
    EXPECT_EQ(parsedMessage->getMessage(), error);
    EXPECT_EQ(parsedMessage->getDeviceKey(), DEVICE_KEY);
    EXPECT_GT(parsedMessage->getArrivalTime().time_since_epoch().count(), 0);
}
