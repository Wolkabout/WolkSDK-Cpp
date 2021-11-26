/*
 * Copyright 2021 Adriateh d.o.o.
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

    static void LogMessage(const wolkabout::MqttMessage& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    std::shared_ptr<WolkaboutFileManagementProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";
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
    EXPECT_EQ(protocol->extractDeviceKeyFromChannel("p2d/" + DEVICE_KEY + "/file_upload_initiate"), DEVICE_KEY);
}

TEST_F(WolkaboutFileManagementProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(
      protocol->getMessageType(std::make_shared<MqttMessage>("", "p2d/" + DEVICE_KEY + "/file_upload_initiate")),
      MessageType::FILE_UPLOAD_INIT);
}
