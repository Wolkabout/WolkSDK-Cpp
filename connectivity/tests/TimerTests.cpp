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
#include "core/utilities/Timer.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class TimerTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    void SetUp() override { timer = std::make_shared<Timer>(); }

    std::shared_ptr<Timer> timer;

    std::mutex mutex;
    std::unique_lock<std::mutex> lock{mutex};
    std::condition_variable conditionVariable;
};

TEST_F(TimerTests, StartTest)
{
    // Define the callback and when it will be invoked
    std::atomic_bool called{false};
    auto end = std::chrono::system_clock::now();
    const auto delay = std::chrono::milliseconds{100};
    timer->start(delay, [&]() {
        called = true;
        end = std::chrono::system_clock::now();
        conditionVariable.notify_one();
    });

    // Start the countdown
    const auto start = std::chrono::system_clock::now();
    if (!called)
    {
        const auto wait = std::chrono::seconds{1};
        conditionVariable.wait_for(lock, wait);
        const auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        EXPECT_GE(durationMs, delay);
        EXPECT_LT(durationMs, wait);
    }
    EXPECT_TRUE(called);
}

TEST_F(TimerTests, StartTestAndStop)
{
    // Define the callback and when it will be invoked
    std::atomic_bool called{false};
    const auto delay = std::chrono::milliseconds{100};
    timer->start(delay, [&] {
        called = true;
        conditionVariable.notify_one();
    });
    EXPECT_TRUE(timer->running());

    // Start the countdown
    const auto start = std::chrono::system_clock::now();
    conditionVariable.wait_for(lock, delay / 2);
    timer->stop();
    EXPECT_FALSE(timer->running());
    const auto wait = std::chrono::milliseconds{200};
    conditionVariable.wait_for(lock, wait);
    const auto durationMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    EXPECT_GE(durationMs, delay);
    EXPECT_FALSE(called);
}

TEST_F(TimerTests, RunCodeSixTimes)
{
    // Define the callback that will be invoked
    std::atomic_uint16_t calls{0};
    auto end = std::chrono::system_clock::now();
    const auto delay = std::chrono::milliseconds{16};
    timer->run(delay, [&] {
        ++calls;
        if (calls == 6)
        {
            end = std::chrono::system_clock::now();
            conditionVariable.notify_one();
        }
    });

    // Start the countdown
    const auto start = std::chrono::system_clock::now();
    if (calls != 6)
    {
        const auto wait = std::chrono::seconds{1};
        conditionVariable.wait_for(lock, wait);
        timer->stop();
        const auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        EXPECT_GE(durationMs, delay);
        EXPECT_LT(durationMs, wait);
    }
    EXPECT_EQ(calls, 6);
}
