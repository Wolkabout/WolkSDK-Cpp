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

#include <any>
#include <sstream>

#define private public
#define protected public
#include "core/protocol/wolkabout/WolkaboutRegistrationProtocol.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

#include <regex>

using namespace wolkabout;

class WolkaboutRegistrationProtocolTests : public ::testing::Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutRegistrationProtocol>(); }

    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    std::shared_ptr<WolkaboutRegistrationProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";
};

TEST_F(WolkaboutRegistrationProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutRegistrationProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2g/" + DEVICE_KEY + "/registered_devices") !=
                channels.cend());
}

TEST_F(WolkaboutRegistrationProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->extractDeviceKeyFromChannel("p2g/" + DEVICE_KEY + "/registered_devices"), DEVICE_KEY);
}

TEST_F(WolkaboutRegistrationProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(
      protocol->getMessageType(std::make_shared<wolkabout::Message>("", "p2g/" + DEVICE_KEY + "/registered_devices")),
      MessageType::REGISTERED_DEVICES_RESPONSE);
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRegistrationNoDevices)
{
    // Create a message without any devices
    auto registration = DeviceRegistrationMessage({});

    // Expect that the parsed will return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, registration), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRegistrationDeviceWithoutAName)
{
    // Create a message with a no-named device
    auto registration = DeviceRegistrationMessage{{{"", "", "", {}, {}, {}}}};

    // Expect that the parsed will return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, registration), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRegistrationSingleEmptyDevice)
{
    // Create a message with a valid device that doesn't have anything
    auto registration = DeviceRegistrationMessage{{{"winter", "light", "", {}, {}, {}}}};

    // Make place for the payload
    auto message = std::unique_ptr<Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, registration));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(g2p\/\w+\/device_registration)");
    const auto payloadRegex =
      std::regex(R"(\[\{"attributes":\[\],"feeds":\[\],"key":"\w+","name":"\w+","parameters":\{\}\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRegistrationSingleWithEverything)
{
    // Create a message with a valid device that has everything
    auto parameters = std::map<ParameterName, std::string>{{ParameterName::FIRMWARE_VERSION, "3.0.0"}};
    auto feeds = std::map<std::string, FeedRegistrationData>{{"T", {"Temperature", FeedType::IN_OUT, "CELSIUS", "T"}}};
    auto attributes =
      std::map<std::string, AttributeRegistrationData>{{"Status", {"Status", DataType::STRING, "lazy"}}};
    auto registration = DeviceRegistrationMessage{
      {{"furnish", "high", "b42ff956-078e-4d6c-8500-791182149729", parameters, feeds, attributes}}};

    // Make place for the payload
    auto message = std::unique_ptr<Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, registration));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(g2p\/\w+\/device_registration)");
    const auto payloadRegex = std::regex(
      R"(\[\{"attributes":\[\{"dataType":"\w+","name":"\w+","value":"\w+"\}\],"feeds":\[\{"name":"\w+","reference":"\w+","type":"\w+","unitGuid":"\w+"\}\],"guid":"[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}","key":"\w+","name":"\w+","parameters":\{"\w+":"\d.\d.\d"\}\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRemovalEmptyArray)
{
    // Create a message with an empty array
    auto removal = DeviceRemovalMessage({});

    // Expect that parsing the payload would return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, removal), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRemovalEmptyKeyInArray)
{
    // Create a message with an empty key in the array
    auto removal = DeviceRemovalMessage({"123", ""});

    // Expect that parsing the payload would return a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, removal), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeDeviceRemovalSingle)
{
    // Create a message with an empty array
    auto removal = DeviceRemovalMessage({"TestKey"});

    // Make place for the payload
    auto message = std::unique_ptr<Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, removal));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(g2p\/\w+\/device_removal)");
    const auto payloadRegex = std::regex(R"(\["\w+"\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeRegisteredDevicesRequestNoParams)
{
    // Make a request with all default values
    auto devices = RegisteredDevicesRequestMessage{};

    // Make place for the payload
    auto message = std::unique_ptr<Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, devices));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(g2p\/\w+\/registered_devices)");
    const auto payloadRegex = std::regex(R"(\{"timestampFrom":\d+\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutRegistrationProtocolTests, SerializeRegisteredDevicesRequestWithAllParams)
{
    // Make a request with all set values
    auto devices = RegisteredDevicesRequestMessage{
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()),
      "iBeacon", "00:11:22:33:44:55"};

    // Make place for the payload
    auto message = std::unique_ptr<Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, devices));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(g2p\/\w+\/registered_devices)");
    const auto payloadRegex =
      std::regex(R"(\{"deviceType":"\w+","externalId":"\d+:\d+:\d+:\d+:\d+:\d+","timestampFrom":\d+\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutRegistrationProtocolTests, DeserializeRegisteredDevicesNotRightTopic)
{
    // Make a message with the topic that is not right
    auto topic = "p2g/" + DEVICE_KEY + "/devices_i_guess?";
    auto payload = std::string{};
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseRegisteredDevicesResponse(message), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, DeserializeRegisteredDevicesPayloadMalformed)
{
    // Make a message with the payload that is not an object
    auto topic = "p2g/" + DEVICE_KEY + "/registered_devices";
    auto payload = "[]";
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseRegisteredDevicesResponse(message), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, DeserializeRegisteredDevicesMalformedDeviceInfo)
{
    // Make a message with the `matchingDevices` field malformed
    auto topic = "p2g/" + DEVICE_KEY + "/registered_devices";
    auto payload = R"({"timestampFrom": 1625578239000,"deviceType":"iBeacon","matchingDevices":1234})";
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseRegisteredDevicesResponse(message), nullptr);
}

TEST_F(WolkaboutRegistrationProtocolTests, DeserializeRegisteredDevicesSingleDevice)
{
    // Make a message with a valid payload containing a single device
    auto topic = "p2g/" + DEVICE_KEY + "/registered_devices";
    auto payload =
      R"({"deviceType":"iBeacon","externalId":"00:11:22:33:44:55","matchingDevices":[{"deviceKey":"Beacon1","deviceType":"iBeacon","externalId":"00:11:22:33:44:55"}],"timestampFrom": 1625578239000})";
    auto message = std::make_shared<Message>(payload, topic);
    LogMessage(*message);

    // Make place for the parsed message
    auto parsedMessage = std::unique_ptr<RegisteredDevicesResponseMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseRegisteredDevicesResponse(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Now check the fields of the message
    EXPECT_EQ(parsedMessage->getDeviceType(), "iBeacon");
    EXPECT_EQ(parsedMessage->getExternalId(), "00:11:22:33:44:55");
    EXPECT_EQ(parsedMessage->getTimestampFrom().count(), 1625578239000);
    ASSERT_FALSE(parsedMessage->getMatchingDevices().empty());
    const auto& device = parsedMessage->getMatchingDevices().front();
    EXPECT_EQ(device.deviceKey, "Beacon1");
    EXPECT_EQ(device.deviceType, "iBeacon");
    EXPECT_EQ(device.externalId, "00:11:22:33:44:55");
}
