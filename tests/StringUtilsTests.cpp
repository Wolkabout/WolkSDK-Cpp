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
#include "core/utilities/StringUtils.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class StringUtilsTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }
};

TEST_F(StringUtilsTests, ContainsTest)
{
    EXPECT_TRUE(StringUtils::contains("Hello", 'l'));
    EXPECT_FALSE(StringUtils::contains("Aloha", 'S'));
    EXPECT_TRUE(StringUtils::contains("Hello world!", "world"));
    EXPECT_FALSE(StringUtils::contains("Welcome!", "Hello"));
}

TEST_F(StringUtilsTests, TokenizeEmptyString)
{
    EXPECT_TRUE(StringUtils::tokenize("", "").empty());
}

TEST_F(StringUtilsTests, TokenizeTest)
{
    auto tokens = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(tokens = StringUtils::tokenize("hello.world", "."));
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0], "hello");
    ASSERT_EQ(tokens[1], "world");
}

TEST_F(StringUtilsTests, TokenizeCharTest)
{
    auto tokens = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(tokens = StringUtils::tokenize("hello.world", '.'));
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0], "hello");
    ASSERT_EQ(tokens[1], "world");
}

TEST_F(StringUtilsTests, StartsWithEndsWithTest)
{
    ASSERT_TRUE(StringUtils::startsWith("Hello World!", "Hello"));
    ASSERT_FALSE(StringUtils::startsWith("Hello World!", "World"));
    ASSERT_TRUE(StringUtils::endsWith("Hello World!", "World!"));
    ASSERT_FALSE(StringUtils::endsWith("Hello World!", "world"));
}

TEST_F(StringUtilsTests, RemoveTrailingWhitespace)
{
    auto value = std::string{"Hello world!     "};
    ASSERT_NO_FATAL_FAILURE(StringUtils::removeTrailingWhitespace(value));
    EXPECT_EQ(value.size(), 12);
}

TEST_F(StringUtilsTests, RemoveTrailingWhitespaceNoWhitespaces)
{
    auto value = std::string{"Aloha!"};
    ASSERT_NO_FATAL_FAILURE(StringUtils::removeTrailingWhitespace(value));
    EXPECT_EQ(value.size(), 6);
}

TEST_F(StringUtilsTests, RemovePrefixTest)
{
    EXPECT_EQ(StringUtils::removePrefix("Hello! Ana! Hello!", "Hello! "), "Ana! Hello!");
    EXPECT_EQ(StringUtils::removePrefix("Hello Ana! Hello!", "Hello! "), "Hello Ana! Hello!");
}

TEST_F(StringUtilsTests, RemoveSufixTest)
{
    EXPECT_EQ(StringUtils::removeSufix("Hello! Ana! Hello!", " Hello!"), "Hello! Ana!");
    EXPECT_EQ(StringUtils::removeSufix("Hello Ana! Hello", " Hello!"), "Hello Ana! Hello");
}

TEST_F(StringUtilsTests, RemoveSubStringTest)
{
    EXPECT_EQ(StringUtils::removeSubstring("Hello! Ana! Hello!", "Hello!"), " Ana! Hello!");
    EXPECT_EQ(StringUtils::removeSubstring("Hello Ana! Hello", "Hello!"), "Hello Ana! Hello");
}

TEST_F(StringUtilsTests, ToUpperCaseTest)
{
    EXPECT_EQ(StringUtils::toUpperCase("Hello!"), "HELLO!");
}
