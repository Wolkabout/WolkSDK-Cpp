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
#include "core/protocol/wolkabout/WolkaboutGatewayRegistrationProtocol.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

#include <regex>

using namespace wolkabout;
using namespace ::testing;

class WolkaboutGatewayRegistrationProtocolTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    void SetUp() override
    {
        protocol = std::unique_ptr<WolkaboutGatewayRegistrationProtocol>{new WolkaboutGatewayRegistrationProtocol};
    }

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";

    std::unique_ptr<WolkaboutGatewayRegistrationProtocol> protocol;
};

TEST_F(WolkaboutGatewayRegistrationProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "d2p/" + DEVICE_KEY + "/device_registration") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "d2p/" + DEVICE_KEY + "/device_removal") !=
                channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "d2p/" + DEVICE_KEY + "/registered_devices") !=
                channels.cend());
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/device_registration"}), DEVICE_KEY);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, GetDeviceType)
{
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/" + DEVICE_KEY + "/device_registration"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/" + DEVICE_KEY + "/device_registration"}),
              MessageType::DEVICE_REGISTRATION);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, GetResponseChannelForAnything)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(MessageType::DEVICE_REGISTRATION, DEVICE_KEY).empty());
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseOutgoingDeviceRegistrationResponseMessage)
{
    // Make the message
    auto response = DeviceRegistrationResponseMessage{{"D1", "D2"}, {"D3"}};

    // Parse it into a message
    auto parsedMessage = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->makeOutboundMessage(DEVICE_KEY, response));
    ASSERT_NE(parsedMessage, nullptr);
    LogMessage(*parsedMessage);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(p2d\/\w+\/device_registration_response)");
    const auto payloadRegex = std::regex(R"(\{"failed":\["\w+"\],"success":\["\w+","\w+"\]\})");
    EXPECT_TRUE(std::regex_match(parsedMessage->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(parsedMessage->getContent(), payloadRegex));
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseOutgoingRegisteredDevicesResponseMessage)
{
    // Make the message
    auto response = RegisteredDevicesResponseMessage{
      std::chrono::milliseconds{123456789}, "TestType", "TestId", {{"TestKey", "TestId", "TestType"}}};

    // Parse it into a message
    auto parsedMessage = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->makeOutboundMessage(DEVICE_KEY, response));
    ASSERT_NE(parsedMessage, nullptr);
    LogMessage(*parsedMessage);

    // Check both the topic and the payload with some regexes
    const auto topicRegex = std::regex(R"(p2d\/\w+\/registered_devices)");
    const auto payloadRegex = std::regex(
      R"(\{"deviceType":"\w+","externalId":"\w+","matchingDevices":\[\{"deviceKey":"\w+","deviceType":"\w+","externalId":"\w+"\}\],"timestampFrom":\d+\})");
    EXPECT_TRUE(std::regex_match(parsedMessage->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(parsedMessage->getContent(), payloadRegex));
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageNotDeviceRegistrationMessage)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":[{"name":"Temperature","reference":"T","type":"IN_OUT","unitGuid":"CELSIUS"}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/i_want_new_device_please");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageMissingKeys)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageFeedsNotArray)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":{},"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageFeedMissingKeys)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":[{"name":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageFeedsEmptyName)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":[{"name":"","reference":"","type":"","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageFeedReferenceEmptyName)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":[{"name":"test-feed","reference":"","type":"","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageFeedInvalidType)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":[{"name":"test-feed","reference":"tf","type":"stupid","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageAttributesNotArray)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":{},"feeds":[{"name":"test-feed","reference":"tf","type":"IN_OUT","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageAttributesMissingKeys)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"name":""}],"feeds":[{"name":"test-feed","reference":"tf","type":"IN_OUT","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageAttributesEmptyName)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"name":"","dataType":"","value":""}],"feeds":[{"name":"test-feed","reference":"tf","type":"IN_OUT","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessageAttributesEmptyDataType)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"name":"test-attribute","dataType":"","value":""}],"feeds":[{"name":"test-feed","reference":"tf","type":"IN_OUT","unitGuid":""}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect it to return a nullptr
    EXPECT_EQ(protocol->parseDeviceRegistrationMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRegistrationMessage)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(
      R"([{"attributes":[{"dataType":"STRING","name":"Status","value":"lazy"}],"feeds":[{"name":"Temperature","reference":"T","type":"IN_OUT","unitGuid":"CELSIUS"}],"guid":"b42ff956-078e-4d6c-8500-791182149729","key":"high","name":"furnish","parameters":{"FIRMWARE_VERSION":"3.0.0"}}])",
      "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Parse the message
    auto parsedMessage = std::unique_ptr<DeviceRegistrationMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseDeviceRegistrationMessage(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check the fields of the message
    ASSERT_FALSE(parsedMessage->getDevices().empty());
    ASSERT_EQ(parsedMessage->getDevices().size(), 1);

    // Take the first device
    const auto& firstDevice = parsedMessage->getDevices().front();
    EXPECT_EQ(firstDevice.name, "furnish");
    EXPECT_EQ(firstDevice.key, "high");
    EXPECT_EQ(firstDevice.guid, "b42ff956-078e-4d6c-8500-791182149729");

    // Check the feeds, attributes and parameters
    const auto& parameters = firstDevice.parameters;
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_NE(parameters.find(ParameterName::FIRMWARE_VERSION), parameters.cend());
    EXPECT_EQ(parameters.at(ParameterName::FIRMWARE_VERSION), "3.0.0");

    const auto& feeds = firstDevice.feeds;
    ASSERT_EQ(feeds.size(), 1);
    ASSERT_NE(feeds.find("T"), feeds.cend());
    const auto& feed = feeds.at("T");
    EXPECT_EQ(feed.getName(), "Temperature");
    EXPECT_EQ(feed.getReference(), "T");
    EXPECT_EQ(feed.getFeedType(), FeedType::IN_OUT);
    EXPECT_EQ(feed.getUnit(), "CELSIUS");

    const auto& attributes = firstDevice.attributes;
    ASSERT_EQ(attributes.size(), 1);
    ASSERT_NE(attributes.find("Status"), attributes.cend());
    const auto& attribute = attributes.at("Status");
    EXPECT_EQ(attribute.getName(), "Status");
    EXPECT_EQ(attribute.getDataType(), DataType::STRING);
    EXPECT_EQ(attribute.getValue(), "lazy");
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRemovalNotDeviceRemovalMessage)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(R"([])", "d2p/WOLK_SDK_TEST/device_registration");
    LogMessage(*message);

    // Expect a nullptr
    EXPECT_EQ(protocol->parseDeviceRemovalMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRemovalMessageEmptyKey)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(R"([""])", "d2p/WOLK_SDK_TEST/device_removal");
    LogMessage(*message);

    // Expect a nullptr
    EXPECT_EQ(protocol->parseDeviceRemovalMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingDeviceRemovalMessage)
{
    // Create the message
    auto message = std::make_shared<wolkabout::Message>(R"(["TestKey"])", "d2p/WOLK_SDK_TEST/device_removal");
    LogMessage(*message);

    // Parse the message
    auto parsedMessage = std::unique_ptr<DeviceRemovalMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseDeviceRemovalMessage(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check the contents of the file
    ASSERT_FALSE(parsedMessage->getKeys().empty());
    ASSERT_EQ(parsedMessage->getKeys().size(), 1);
    EXPECT_EQ(parsedMessage->getKeys().front(), "TestKey");
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests,
       ParseIncomingRegisteredDevicesRequestMessageNotRegisteredDevicesMessage)
{
    // Prepare the message
    auto message = std::make_shared<wolkabout::Message>(
      R"({"deviceType":"iBeacon","externalId":"00:11:22:33:44:55","timestampFrom":1642172260746})",
      "g2p/WOLK_SDK_TEST/can_i_see_please");
    LogMessage(*message);

    // Expect a nullptr to be returned
    EXPECT_EQ(protocol->parseRegisteredDevicesRequestMessage(message), nullptr);
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingRegisteredDevicesRequestMessageOnlyRequired)
{
    // Prepare the message
    auto message = std::make_shared<wolkabout::Message>(R"({"timestampFrom":1642172260746})",
                                                        "g2p/WOLK_SDK_TEST/registered_devices");
    LogMessage(*message);

    // Parse the message
    auto parsedMessage = std::unique_ptr<RegisteredDevicesRequestMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseRegisteredDevicesRequestMessage(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check the fields
    EXPECT_EQ(parsedMessage->getTimestampFrom().count(), 1642172260746);
    EXPECT_TRUE(parsedMessage->getDeviceType().empty());
    EXPECT_TRUE(parsedMessage->getExternalId().empty());
}

TEST_F(WolkaboutGatewayRegistrationProtocolTests, ParseIncomingRegisteredDevicesRequestMessage)
{
    // Prepare the message
    auto message = std::make_shared<wolkabout::Message>(
      R"({"deviceType":"iBeacon","externalId":"00:11:22:33:44:55","timestampFrom":1642172260746})",
      "g2p/WOLK_SDK_TEST/registered_devices");
    LogMessage(*message);

    // Parse the message
    auto parsedMessage = std::unique_ptr<RegisteredDevicesRequestMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseRegisteredDevicesRequestMessage(message));
    ASSERT_NE(parsedMessage, nullptr);

    // Check the fields
    EXPECT_EQ(parsedMessage->getTimestampFrom().count(), 1642172260746);
    EXPECT_EQ(parsedMessage->getDeviceType(), "iBeacon");
    EXPECT_EQ(parsedMessage->getExternalId(), "00:11:22:33:44:55");
}
