/**
 * Copyright 2021 Wolkabout s.r.o.
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
#include "core/protocol/wolkabout/WolkaboutDataProtocol.h"
#undef private
#undef protected

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace nlohmann;
using namespace wolkabout;

class WolkaboutDataProtocolTests : public Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutDataProtocol>(); }

    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const std::shared_ptr<wolkabout::MqttMessage>& message)
    {
        LOG(TRACE) << "Topic: '" << message->getChannel() << "' | Payload: '" << message->getContent() << "'";
    }

    std::shared_ptr<WolkaboutDataProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";

    const std::string TEMPERATURE = "T";

    const std::string ROTATION = "Rotation";

    const std::string TRANSLATION = "Translation";

    const std::string TIMESTAMP = "timestamp";
};

TEST_F(WolkaboutDataProtocolTests, GenerateOutboundFeedValues)
{
    LOG(DEBUG) << "Hello World!";
}

TEST_F(WolkaboutDataProtocolTests, ParseIncomingFeedValuesSingle)
{
    // Define the single feed value that is incoming
    auto topic = PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + DEVICE_KEY + CHANNEL_DELIMITER +
                 toString(MessageType::FEED_VALUES);
    auto payload = json::array({json{{TEMPERATURE, 20}, {TIMESTAMP, 1623159800000}}});
    auto message = std::make_shared<wolkabout::MqttMessage>(payload.dump(), topic);
    LogMessage(message);

    // Parse the message
    auto receivedMessage = std::shared_ptr<FeedValuesMessage>{};
    ASSERT_NO_THROW(receivedMessage = protocol->parseFeedValues(message));

    // Inspect the reading inside the received message
    ASSERT_NE(receivedMessage, nullptr);
    ASSERT_EQ(receivedMessage->getReadings().size(), 1);
    const auto& timestamp = receivedMessage->getReadings().cbegin()->first;
    const auto& readings = receivedMessage->getReadings().cbegin()->second;
    EXPECT_EQ(timestamp, 1623159800000);
    ASSERT_EQ(readings.size(), 1);
    const auto& reading = readings.front();
    EXPECT_EQ(reading.getReference(), TEMPERATURE);
    EXPECT_EQ(reading.getDoubleValue(), 20);
    EXPECT_EQ(reading.getTimestamp(), 1623159800000);
}

TEST_F(WolkaboutDataProtocolTests, ParseIncomingFeedValuesMultiple)
{
    // Define the single feed value that is incoming
    auto topic = PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + DEVICE_KEY + CHANNEL_DELIMITER +
                 toString(MessageType::FEED_VALUES);
    auto payload = json::array({json{{TRANSLATION, 2}, {TIMESTAMP, 1623160000000}},
                                {{ROTATION, 90}, {TRANSLATION, 2}, {TIMESTAMP, 1623161000000}},
                                {{ROTATION, -90}, {TIMESTAMP, 1623162000000}},
                                {{TRANSLATION, 4}, {TIMESTAMP, 1623163000000}}});
    auto message = std::make_shared<wolkabout::MqttMessage>(payload.dump(), topic);
    LogMessage(message);

    // Parse the message
    auto receivedMessage = std::shared_ptr<FeedValuesMessage>{};
    ASSERT_NO_THROW(receivedMessage = protocol->parseFeedValues(message));

    // Inspect the readings inside the received message
    ASSERT_NE(receivedMessage, nullptr);
    const auto& readings = receivedMessage->getReadings();
    ASSERT_EQ(readings.size(), 4);    // Because there's 4 timestamps

    // Inspect the first timestamp
    const auto& firstSet = readings.find(1623160000000);
    ASSERT_TRUE(firstSet != readings.cend());
    const auto& firstReadings = firstSet->second;


}
