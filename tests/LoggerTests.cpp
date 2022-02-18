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
#include "core/utilities/Logger.h"
#undef private
#undef protected

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace ::testing;

class LoggerTests : public ::testing::Test
{
public:
    Logger logger;
};

TEST_F(LoggerTests, AppendLogButItsOff)
{
    auto log = Log{LogLevel::OFF};
    log << "Hello World!";
    logger += log;
    EXPECT_TRUE(logger.buffer().empty());
}

TEST_F(LoggerTests, LogLevelTest)
{
    const auto map =
      std::map<LogLevel, std::string>{{LogLevel::TRACE, "TRACE"}, {LogLevel::DEBUG, "DEBUG"}, {LogLevel::INFO, "INFO"},
                                      {LogLevel::WARN, "WARN"},   {LogLevel::ERROR, "ERROR"}, {LogLevel::OFF, "OFF"}};

    // Check the reverse function
    for (const auto& pair : map)
        EXPECT_EQ(from_string(pair.second), pair.first);
}
