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
#include "core/model/Attribute.h"
#include "core/model/Device.h"
#include "core/model/Feed.h"
#include "core/model/Message.h"
#include "core/model/Reading.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class ModelsTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }
};

TEST_F(ModelsTests, CreateAttributeWithInvalidDataType)
{
    ASSERT_THROW(Attribute("TestName", static_cast<DataType>(0x1234), "TestValue"), std::runtime_error);
}

TEST_F(ModelsTests, AttributeTest)
{
    const auto attribute = Attribute{"TestName", DataType::STRING, "TestValue"};
    EXPECT_EQ(attribute.getName(), "TestName");
    EXPECT_EQ(attribute.getDataType(), DataType::STRING);
    EXPECT_EQ(attribute.getValue(), "TestValue");
}

TEST_F(ModelsTests, CreateDeviceWithInvalidOutboundDataMode)
{
    ASSERT_THROW(Device("TestDevice", "TestPassword", static_cast<OutboundDataMode>(0x1234)), std::runtime_error);
}

TEST_F(ModelsTests, DeviceTest)
{
    auto device = Device{"TestKey", "TestPassword", OutboundDataMode::PUSH, "TestName"};
    EXPECT_EQ(device.getName(), "TestName");
    EXPECT_EQ(device.getKey(), "TestKey");
    EXPECT_EQ(device.getPassword(), "TestPassword");
    EXPECT_TRUE(device.getFeeds().empty());
    EXPECT_TRUE(device.getAttributes().empty());
    EXPECT_TRUE(device.getParameters().empty());
    EXPECT_EQ(device.getType(), OutboundDataMode::PUSH);

    ASSERT_NO_FATAL_FAILURE(device.setName("NewName"));
    ASSERT_NO_FATAL_FAILURE(device.setFeeds({}));
    ASSERT_NO_FATAL_FAILURE(device.setAttributes({}));
    ASSERT_NO_FATAL_FAILURE(device.setParameters({}));
}

TEST_F(ModelsTests, CreateFeedWithInvalidFeedType)
{
    ASSERT_THROW(Feed("TestFeed", "TestReference", static_cast<FeedType>(0x1234), ""), std::runtime_error);
}

TEST_F(ModelsTests, FeedTest)
{
    const auto feed = Feed{"TestFeed", "TF", FeedType::IN, "NUMERIC"};
    EXPECT_EQ(feed.getName(), "TestFeed");
    EXPECT_EQ(feed.getReference(), "TF");
    EXPECT_EQ(feed.getFeedType(), FeedType::IN);
    EXPECT_EQ(feed.getUnit(), "NUMERIC");
}

TEST_F(ModelsTests, MessageTest)
{
    const auto message = wolkabout::Message{"TestContent", "TestChannel"};
    EXPECT_EQ(message.getContent(), "TestContent");
    EXPECT_EQ(message.getChannel(), "TestChannel");
}

TEST_F(ModelsTests, BoolFalseReading)
{
    const auto reading = Reading{"R", false};
    EXPECT_FALSE(reading.getBoolValue());
}

TEST_F(ModelsTests, BoolFromNonBool)
{
    const auto reading = Reading{"R", std::uint64_t{132}};
    ASSERT_THROW(reading.getBoolValue(), std::invalid_argument);
}

TEST_F(ModelsTests, GetHexFromStringFail)
{
    const auto reading = Reading{"R", std::string{"SomeValue"}};
    ASSERT_THROW(reading.getHexValue(), std::invalid_argument);
}

TEST_F(ModelsTests, GetHexReading)
{
    const auto reading = Reading{"R", std::uint64_t{64}};
    EXPECT_EQ(reading.getHexValue(), 100);
}

TEST_F(ModelsTests, ObtainLocationFromNonLocationReading)
{
    const auto reading = Reading{"R", 123.0f};
    ASSERT_THROW(reading.getLocationValue(), std::invalid_argument);
}

TEST_F(ModelsTests, LocationReadingTest)
{
    const auto reading = Reading{"R", Location{45.123, 18.123}};
    const auto location = reading.getLocationValue();
    EXPECT_EQ(location.latitude, 45.123f);
    EXPECT_EQ(location.longitude, 18.123f);
}

TEST_F(ModelsTests, ReadingSetTimestampTest)
{
    auto reading = Reading{"R", std::string{"Hello World!"}};
    EXPECT_EQ(reading.getTimestamp(), 0);
    ASSERT_NO_FATAL_FAILURE(
      reading.setTimestamp(static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count())));
    EXPECT_NE(reading.getTimestamp(), 0);
}

TEST_F(ModelsTests, MultiReadingTest)
{
    auto reading = Reading{"R", std::vector<std::double_t>{123, 456}};
    EXPECT_EQ(reading.getStringValues().size(), 2);
    EXPECT_EQ(reading.getUIntValues().size(), 2);
    EXPECT_EQ(reading.getIntValues().size(), 2);
    EXPECT_EQ(reading.getDoubleValues().size(), 2);

    reading = Reading{"R", std::vector<std::uint64_t>{123, 456}};
    EXPECT_EQ(reading.getStringValues().size(), 2);
    EXPECT_EQ(reading.getUIntValues().size(), 2);
    EXPECT_EQ(reading.getIntValues().size(), 2);
    EXPECT_EQ(reading.getDoubleValues().size(), 2);

    reading = Reading{"R", std::vector<std::int64_t>{123, 456}};
    EXPECT_EQ(reading.getStringValues().size(), 2);
    EXPECT_EQ(reading.getUIntValues().size(), 2);
    EXPECT_EQ(reading.getIntValues().size(), 2);
    EXPECT_EQ(reading.getDoubleValues().size(), 2);
}
