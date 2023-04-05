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

#define private public
#define protected public
#include "core/protocol/wolkabout/WolkaboutDataProtocol.h"
#undef private
#undef protected

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"

#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

#include <regex>

using namespace ::testing;
using namespace nlohmann;
using namespace wolkabout;
using namespace wolkabout::legacy;

class WolkaboutDataProtocolTests : public Test
{
public:
    void SetUp() override { protocol = std::make_shared<WolkaboutDataProtocol>(); }

    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    static void LogMessage(const wolkabout::Message& message)
    {
        LOG(TRACE) << "Topic: '" << message.getChannel() << "' | Payload: '" << message.getContent() << "'";
    }

    std::shared_ptr<WolkaboutDataProtocol> protocol;

    const std::string DEVICE_KEY = "WOLK_SDK_TEST";

    const std::string TEMPERATURE = "T";

    const std::string ACCELERATION = "Acceleration";

    const std::string ROTATION = "Rotation";

    const std::string TRANSLATION = "Translation";

    const std::string TIMESTAMP = "timestamp";
};

TEST_F(WolkaboutDataProtocolTests, GetInboundChannels)
{
    EXPECT_TRUE(protocol->getInboundChannels().empty());
}

TEST_F(WolkaboutDataProtocolTests, GetInboundChannelsForDevice)
{
    // Make place for the channels
    auto channels = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(channels = protocol->getInboundChannelsForDevice(DEVICE_KEY));
    ASSERT_FALSE(channels.empty());

    // Analyze the channels
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/parameters") != channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/feed_values") != channels.cend());
    EXPECT_TRUE(std::find(channels.cbegin(), channels.cend(), "p2d/" + DEVICE_KEY + "/details_synchronization") !=
                channels.cend());
}

TEST_F(WolkaboutDataProtocolTests, ExtractDeviceKeyFromChannel)
{
    // Test with some random topic
    EXPECT_EQ(protocol->getDeviceKey({"", "p2d/" + DEVICE_KEY + "/parameters"}), DEVICE_KEY);
}

TEST_F(WolkaboutDataProtocolTests, GetMessageType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getMessageType({"", "p2d/" + DEVICE_KEY + "/parameters"}), MessageType::PARAMETER_SYNC);
}

TEST_F(WolkaboutDataProtocolTests, GetDeviceType)
{
    // Test with a simple example
    EXPECT_EQ(protocol->getDeviceType({"", "p2d/" + DEVICE_KEY + "/parameters"}), DeviceType::STANDALONE);
}

TEST_F(WolkaboutDataProtocolTests, ResponseChannelForUnknown)
{
    EXPECT_TRUE(protocol->getResponseChannelForMessage(wolkabout::MessageType::UNKNOWN, DEVICE_KEY).empty());
}

TEST_F(WolkaboutDataProtocolTests, ResponseChannelForFeedValues)
{
    EXPECT_EQ(protocol->getResponseChannelForMessage(wolkabout::MessageType::PULL_FEED_VALUES, DEVICE_KEY),
              "p2d/" + DEVICE_KEY + "/feed_values");
}

TEST_F(WolkaboutDataProtocolTests, ResponseChannelForPullParameters)
{
    EXPECT_EQ(protocol->getResponseChannelForMessage(wolkabout::MessageType::PULL_PARAMETERS, DEVICE_KEY),
              "p2d/" + DEVICE_KEY + "/parameters");
}

TEST_F(WolkaboutDataProtocolTests, ResponseChannelForSynchronizeParameters)
{
    EXPECT_EQ(protocol->getResponseChannelForMessage(wolkabout::MessageType::SYNCHRONIZE_PARAMETERS, DEVICE_KEY),
              "p2d/" + DEVICE_KEY + "/parameters");
}

TEST_F(WolkaboutDataProtocolTests, ResponseChannelForDeviceDetails)
{
    EXPECT_EQ(
      protocol->getResponseChannelForMessage(wolkabout::MessageType::DETAILS_SYNCHRONIZATION_REQUEST, DEVICE_KEY),
      "p2d/" + DEVICE_KEY + "/details_synchronization");
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedRegistrationSingle)
{
    // Make a single feed that needs to be registered
    auto feed = Feed{"Test", "T", FeedType::IN, Unit::TEXT};
    auto registration = FeedRegistrationMessage{{feed}};

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, registration));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/feed_registration)");
    const auto payloadRegex = std::regex(R"(\[\{"name":"\w+","reference":"\w+","type":"\w+","unitGuid":"\w+"\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedRegistrationInvalidFeedType)
{
    // Make a single feed that needs to be registered
    auto feed = Feed{"Invalid Feed", "INVALID", FeedType::IN_OUT, Unit::NUMERIC};
    auto registration = FeedRegistrationMessage{{feed}};

    // Expect that the return from the call is a `nullptr`
    registration.m_feeds.front().m_feedType = static_cast<FeedType>(1234);
    ASSERT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, registration), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedRemovalSingle)
{
    // Make a single feed that needs to be removed
    auto removal = FeedRemovalMessage({"T"});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, removal));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/feed_removal)");
    const auto payloadRegex = std::regex(R"(\["\w+"\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedRemovalEmpty)
{
    // Make an empty removal message
    auto removal = FeedRemovalMessage({});

    // Expect that the return from the call is a `nullptr`
    ASSERT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, removal), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedValuesSingle)
{
    // Make a single reading that will be sent out
    auto reading = Reading{TEMPERATURE, std::to_string(20)};
    auto values = FeedValuesMessage({reading});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, values));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/feed_values)");
    const auto payloadRegex = std::regex(R"(\[\{"\w+":\d+\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeMultiValueReading)
{
    // Make a single reading that will be sent out
    auto reading = Reading{ACCELERATION, std::vector<std::double_t>{12.1005, 0.2304, -0.128}};
    auto values = FeedValuesMessage({reading});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, values));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/feed_values)");
    const auto payloadRegex = std::regex(R"(\[\{"\w+":"\d+.\d+,\d+.\d+,\-\d+.\d+"\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedValuesMultiple)
{
    // Make a single reading that will be sent out
    auto boolReading = Reading("B", true, 1);
    auto uintReading = Reading("U", std::uint64_t(123), 1);
    auto intReading = Reading("I", std::int64_t(-100), 1);
    auto floatReading = Reading("F", 1.14f, 2);
    auto doubleReading = Reading("D", 20.23, 2);
    auto stringReading = Reading("S", std::string("SOME_STATUS_CODE"), 2);
    auto values = FeedValuesMessage({boolReading, uintReading, intReading, floatReading, doubleReading, stringReading});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, values));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/feed_values)");
    const auto payloadRegex = std::regex(
      R"(\[\{"\w+":(true|false),"\w+":-\d+,"\w+":\d+,"timestamp":\d+\},\{"\w+":\d+.\d+,"\w+":\d+.\d+,"\w+":"\w+","timestamp":\d+\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedValuesEmptyMap)
{
    // Make an empty message
    auto values = FeedValuesMessage(std::vector<Reading>{});

    // Expect that the return from the call is a `nullptr`
    ASSERT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, values), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeFeedValuesEmptyMapEntry)
{
    // Make an empty message
    auto values = FeedValuesMessage(std::vector<Reading>{});
    values.m_readings.emplace(123, std::vector<Reading>{});

    // Expect that the return from the call is a `nullptr`
    ASSERT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, values), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializePullFeedValues)
{
    // Make the message
    auto pull = PullFeedValuesMessage();

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, pull));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/pull_feed_values)");
    const auto payloadRegex = std::regex("");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeAttributeRegistrationMessageSingle)
{
    // Make a single attribute to register
    auto attribute = Attribute{"TestAttribute", DataType::STRING, "TestValue"};
    auto registration = AttributeRegistrationMessage({attribute});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, registration));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/attribute_registration)");
    const auto payloadRegex = std::regex(R"(\[\{"dataType":"\w+","name":"\w+","value":"\w+"\}\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeAttributeRegistrationInvalidDataType)
{
    // Make a single attribute with an invalid DataType value
    auto attribute = Attribute{"InvalidAttribute", DataType::STRING, "InvalidValue"};
    auto registration = AttributeRegistrationMessage({attribute});

    // Expect that the return is a nullptr
    registration.m_attributes.front().m_dataType = static_cast<DataType>(1234);
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, registration), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeParametersUpdateSingle)
{
    // Make a single parameter
    auto parameter = Parameter{ParameterName::FIRMWARE_VERSION, "1.0.0"};
    auto update = ParametersUpdateMessage({parameter});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, update));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/parameters)");
    const auto payloadRegex = std::regex(R"(\{"\w+":"\d+.\d+.\d+"\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeParametersUpdateMultiple)
{
    // Make multiple parameters that will be sent out
    auto boolParameter = Parameter{ParameterName::FIRMWARE_UPDATE_ENABLED, "true"};
    auto uintParameter = Parameter{ParameterName::MAXIMUM_MESSAGE_SIZE, std::to_string(123)};
    auto intParameter = Parameter{ParameterName::OUTBOUND_DATA_RETENTION_TIME, std::to_string(-100)};
    auto floatParameter = Parameter{ParameterName::FIRMWARE_UPDATE_CHECK_TIME, std::to_string(1.14f)};
    auto doubleParameter = Parameter{ParameterName::FIRMWARE_VERSION, std::to_string(20.23)};
    auto stringParameter = Parameter{ParameterName::FILE_TRANSFER_URL_ENABLED, "SOME_STATUS_CODE"};
    auto update = ParametersUpdateMessage(
      {boolParameter, uintParameter, intParameter, floatParameter, doubleParameter, stringParameter});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, update));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/parameters)");
    const auto payloadRegex =
      std::regex(R"(\{"\w+":"\w+","\w+":\d.\d+,"\w+":(true|false),"\w+":\d+.\d+,"\w+":\d+,"\w+":-\d+\})");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeParametersUpdateEmpty)
{
    // Make an empty message
    auto update = ParametersUpdateMessage({});

    // Expect that the parsing result is `nullptr`
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, update), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeParametersUpdateInvalidParameterName)
{
    // Make a parameter with an invalid name
    auto parameter = Parameter{static_cast<ParameterName>(1234), "TestValue"};
    auto update = ParametersUpdateMessage({parameter});

    // Expect the result to be null
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, update), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeParametersPullValues)
{
    // Make the message
    auto pull = ParametersPullMessage();

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, pull));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/pull_parameters)");
    const auto payloadRegex = std::regex("");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeSynchronizeParametersSingle)
{
    // Make the message
    auto synchronize = SynchronizeParametersMessage({ParameterName::FIRMWARE_VERSION});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, synchronize));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/synchronize_parameters)");
    const auto payloadRegex = std::regex(R"(\["\w+"\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeSynchronizeParametersEmpty)
{
    // Make the message
    auto synchronize = SynchronizeParametersMessage({});

    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message = protocol->makeOutboundMessage(DEVICE_KEY, synchronize));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/synchronize_parameters)");
    const auto payloadRegex = std::regex(R"(\[\])");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(std::regex_match(message->getContent(), payloadRegex));
}

TEST_F(WolkaboutDataProtocolTests, SerializeSynchronizeParametersInvalidParameterName)
{
    // Make the message
    auto synchronize = SynchronizeParametersMessage({static_cast<ParameterName>(1234)});

    // Expect the return to be a nullptr
    EXPECT_EQ(protocol->makeOutboundMessage(DEVICE_KEY, synchronize), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, SerializeDetailsSynchronization)
{
    // Make place for the payload
    auto message = std::unique_ptr<wolkabout::Message>{};
    ASSERT_NO_FATAL_FAILURE(message =
                              protocol->makeOutboundMessage(DEVICE_KEY, DetailsSynchronizationRequestMessage{}));
    ASSERT_NE(message, nullptr);
    LogMessage(*message);

    // Analyze both the topic and the content by regex
    const auto topicRegex = std::regex(R"(d2p\/\w+\/details_synchronization)");
    EXPECT_TRUE(std::regex_match(message->getChannel(), topicRegex));
    EXPECT_TRUE(message->getContent().empty());
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesNotArray)
{
    // Make the invalid payload
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::object({{"1", "2"}, {"3", "4"}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFeedValues(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesArrayMemberNotObject)
{
    // Make the invalid payload
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({1, 2, 3});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFeedValues(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesArrayMemberNoTimestamp)
{
    // Make the invalid payload
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{"1", "2"}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFeedValues(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesArrayMemberTimestampNotUnsignedInteger)
{
    // Make the invalid payload
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{"timestamp", "2"}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFeedValues(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesArrayMemberValueIsAnObject)
{
    // Make the invalid payload
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{"timestamp", 123}, {"test", json{{"1", "2"}}}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFeedValues(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesArrayMemberValueArrayContainsInvalidType)
{
    // Define the single feed value that is incoming
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{TEMPERATURE, json::array({"abc", "bcd"})}, {TIMESTAMP, 1623159800000}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that deserialization returns a nullptr
    EXPECT_EQ(protocol->parseFeedValues(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesSingle)
{
    // Define the single feed value that is incoming
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{TEMPERATURE, 20}, {TIMESTAMP, 1623159800000}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

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

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesMultiple)
{
    // Define the single feed value that is incoming
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{TRANSLATION, 2}, {TIMESTAMP, 1623160000000}},
                                {{ROTATION, 90}, {TRANSLATION, 2}, {TIMESTAMP, 1623161000000}},
                                {{ROTATION, -90}, {TIMESTAMP, 1623162000000}},
                                {{TRANSLATION, 4}, {TIMESTAMP, 1623163000000}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

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
}

TEST_F(WolkaboutDataProtocolTests, DeserializeFeedValuesVector)
{
    // Define the single feed value that is incoming
    auto topic = "p2d/" + DEVICE_KEY + "/feed_values";
    auto payload = json::array({json{{TEMPERATURE, json::array({1, 2, 3})}, {TIMESTAMP, 1623159800000}}});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

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
    EXPECT_EQ(reading.getTimestamp(), 1623159800000);
    ASSERT_TRUE(reading.isMulti());
    EXPECT_EQ(reading.getUIntValues().size(), 3);
    EXPECT_EQ(reading.getUIntValues()[0], 1);
    EXPECT_EQ(reading.getUIntValues()[1], 2);
    EXPECT_EQ(reading.getUIntValues()[2], 3);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeParametersNotAnObject)
{
    // Create an invalid message
    auto topic = "p2d/" + DEVICE_KEY + "/parameters";
    auto payload = json::array({"FIRMWARE_UPDATE_CHECK_TIME", 21600});
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that nothing will be return
    ASSERT_EQ(protocol->parseParameters(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeParametersInvalidParameterName)
{
    // Create an invalid message
    auto topic = "p2d/" + DEVICE_KEY + "/parameters";
    auto payload = json{{"TEST_PARAMETER", 21600}};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that nothing will be return
    ASSERT_EQ(protocol->parseParameters(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeParametersInvalidParameterValue)
{
    // Create an invalid message
    auto topic = "p2d/" + DEVICE_KEY + "/parameters";
    auto payload = json{{"FIRMWARE_UPDATE_CHECK_TIME", json{{"someValue", 123}}}};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Expect that nothing will be return
    ASSERT_EQ(protocol->parseParameters(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeParametersSingle)
{
    // Define the single parameters that is incoming
    auto topic = "p2d/" + DEVICE_KEY + "/parameters";
    auto payload = json{{"FIRMWARE_UPDATE_CHECK_TIME", 21600}};
    auto message = std::make_shared<wolkabout::Message>(payload.dump(), topic);
    LogMessage(*message);

    // Parse the message
    auto receivedMessage = std::shared_ptr<ParametersUpdateMessage>{};
    ASSERT_NO_THROW(receivedMessage = protocol->parseParameters(message));

    // Inspect inside the message
    ASSERT_NE(receivedMessage, nullptr);
    const auto& parameters = receivedMessage->getParameters();
    ASSERT_EQ(parameters.size(), 1);
    const auto& parameter = parameters.front();
    ASSERT_EQ(parameter.first, ParameterName::FIRMWARE_UPDATE_CHECK_TIME);
    ASSERT_EQ(std::stoull(parameter.second), 21600);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeDetailsSynchronizationWrongTopic)
{
    auto message = std::make_shared<wolkabout::Message>("", "p2d/" + DEVICE_KEY + "/ha?");
    LogMessage(*message);
    ASSERT_EQ(protocol->parseDetails(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeDetailsSynchronizationNotObject)
{
    auto message = std::make_shared<wolkabout::Message>("[]", "p2d/" + DEVICE_KEY + "/details_synchronization");
    LogMessage(*message);
    ASSERT_EQ(protocol->parseDetails(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeDetailsSynchronizationNoRequiredField)
{
    auto message =
      std::make_shared<wolkabout::Message>(R"({"feeds": []})", "p2d/" + DEVICE_KEY + "/details_synchronization");
    LogMessage(*message);
    ASSERT_EQ(protocol->parseDetails(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeDetailsSynchronizationFeedsEmptyStrings)
{
    auto message = std::make_shared<wolkabout::Message>(R"({"feeds": [""], "attributes": [""]})",
                                                        "p2d/" + DEVICE_KEY + "/details_synchronization");
    LogMessage(*message);
    ASSERT_EQ(protocol->parseDetails(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeDetailsSynchronizationAttributesEmptyStrings)
{
    auto message = std::make_shared<wolkabout::Message>(R"({"feeds": ["F1", "F2"], "attributes": [""]})",
                                                        "p2d/" + DEVICE_KEY + "/details_synchronization");
    LogMessage(*message);
    ASSERT_EQ(protocol->parseDetails(message), nullptr);
}

TEST_F(WolkaboutDataProtocolTests, DeserializeDetailsSynchronizationHappyFlow)
{
    auto message = std::make_shared<wolkabout::Message>(R"({"feeds": ["F1", "F2"], "attributes": ["A1"]})",
                                                        "p2d/" + DEVICE_KEY + "/details_synchronization");
    LogMessage(*message);
    auto parsedMessage = std::shared_ptr<DetailsSynchronizationResponseMessage>{};
    ASSERT_NO_FATAL_FAILURE(parsedMessage = protocol->parseDetails(message));
    ASSERT_NE(parsedMessage, nullptr);
    ASSERT_EQ(parsedMessage->getFeeds().size(), 2);
    ASSERT_EQ(parsedMessage->getAttributes().size(), 1);
}
