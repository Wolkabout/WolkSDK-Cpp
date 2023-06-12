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
#include "core/utilities/ByteUtils.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

#include <regex>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class ByteUtilsTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }
};

TEST_F(ByteUtilsTests, StringToByteArray)
{
    const auto stringData = std::string{"Hello!"};
    const auto byteData = ByteArray{72, 101, 108, 108, 111, 33};

    // Generate the ByteArray
    auto byteArray = ByteArray{};
    ASSERT_NO_FATAL_FAILURE(byteArray = ByteUtils::toByteArray(stringData));
    ASSERT_EQ(byteData.size(), byteArray.size());
    for (auto i = std::size_t{0}; i < byteData.size(); ++i)
        ASSERT_EQ(byteData[i], byteArray[i]);
}

TEST_F(ByteUtilsTests, GenerateRandomBytes)
{
    EXPECT_EQ(ByteUtils::generateRandomBytes(16).size(), 16);
}

TEST_F(ByteUtilsTests, ByteArrayToString)
{
    EXPECT_EQ(ByteUtils::toString({72, 101, 108, 108, 111, 33}), "Hello!");
}

TEST_F(ByteUtilsTests, ByteArrayToHexString)
{
    EXPECT_EQ(ByteUtils::toHexString({0, 15, 16, 160, 161, 255}), "000f10a0a1ff");
}

TEST_F(ByteUtilsTests, GenerateUUIDStringWrongCountOfBytes)
{
    ASSERT_TRUE(ByteUtils::toUUIDString(ByteUtils::generateRandomBytes(3)).empty());
}

TEST_F(ByteUtilsTests, GenerateUUIDString)
{
    auto uuidString = std::string{};
    ASSERT_NO_FATAL_FAILURE(uuidString =
                              ByteUtils::toUUIDString(ByteUtils::generateRandomBytes(ByteUtils::UUID_VECTOR_SIZE)));

    // Check with regex
    ASSERT_TRUE(std::regex_match(
      uuidString, std::regex(R"([a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12})")));
}

TEST_F(ByteUtilsTests, SHA256Test)
{
    auto hashString = std::string{};
    ASSERT_NO_FATAL_FAILURE(hashString = ByteUtils::toHexString(ByteUtils::hashSHA256({65, 65, 65, 65, 65})));
    ASSERT_EQ(hashString, "11770b3ea657fe68cba19675143e4715c8de9d763d3c21a85af6b7513d43997d");
}

TEST_F(ByteUtilsTests, MDA5Test)
{
    auto hashString = std::string{};
    ASSERT_NO_FATAL_FAILURE(hashString = ByteUtils::toHexString(ByteUtils::hashMDA5({65, 65, 65, 65, 65})));
    ASSERT_EQ(hashString, "f6a6263167c92de8644ac998b3c4e4d1");
}
