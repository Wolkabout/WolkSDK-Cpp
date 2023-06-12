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
#include "core/connectivity/OutboundRetryMessageHandler.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"
#include "tests/mocks/OutboundMessageHandlerMock.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class OutboundRetryMessageHandlerTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    void SetUp() override { service = std::make_shared<OutboundRetryMessageHandler>(outboundMessageHandlerMock); }

    OutboundMessageHandlerMock outboundMessageHandlerMock;

    std::shared_ptr<OutboundRetryMessageHandler> service;

    std::mutex mutex;
    std::unique_lock<std::mutex> lock{mutex};
    std::condition_variable conditionVariable;
};

TEST_F(OutboundRetryMessageHandlerTests, AddSingleMessageAndAwaitFail)
{
    // Set up the service to send the message
    std::atomic_uint16_t counter{0};
    EXPECT_CALL(outboundMessageHandlerMock, addMessage)
      .Times(4)
      .WillRepeatedly([&](const std::shared_ptr<wolkabout::Message>&) {
          ++counter;
          conditionVariable.notify_one();
      });
    ASSERT_NO_FATAL_FAILURE(
      service->addMessage({std::make_shared<wolkabout::Message>("Hello!", "message"), "message_response",
                           [&](const std::shared_ptr<wolkabout::Message>&) {}, 3, std::chrono::milliseconds{50}}));
    conditionVariable.wait_for(lock, std::chrono::seconds(1), [&]() { return counter == 4; });
}

TEST_F(OutboundRetryMessageHandlerTests, AddSingleMessageAndGetItRightAway)
{
    EXPECT_CALL(outboundMessageHandlerMock, addMessage).Times(1);
    ASSERT_NO_FATAL_FAILURE(
      service->addMessage({std::make_shared<wolkabout::Message>("Hello!", "message"), "message_response",
                           [&](const std::shared_ptr<wolkabout::Message>&) {}, 3, std::chrono::milliseconds{300}}));
    ASSERT_NO_FATAL_FAILURE(
      service->messageReceived(std::make_shared<wolkabout::Message>("Hiya!", "message_response")));
}

TEST_F(OutboundRetryMessageHandlerTests, AddSingleMessageAndGetItAfter2Sends)
{
    EXPECT_CALL(outboundMessageHandlerMock, addMessage).Times(2);
    ASSERT_NO_FATAL_FAILURE(
      service->addMessage({std::make_shared<wolkabout::Message>("Hello!", "message"), "message_response",
                           [&](const std::shared_ptr<wolkabout::Message>&) {}, 5, std::chrono::milliseconds{300}}));
    std::this_thread::sleep_for(std::chrono::milliseconds{400});
    ASSERT_NO_FATAL_FAILURE(
      service->messageReceived(std::make_shared<wolkabout::Message>("Hiya!", "message_response")));
}

TEST_F(OutboundRetryMessageHandlerTests, AddSingleMessageGetsRemovedFromMessages)
{
    EXPECT_CALL(outboundMessageHandlerMock, addMessage).Times(2);
    ASSERT_NO_FATAL_FAILURE(
      service->addMessage({std::make_shared<wolkabout::Message>("Hello!", "message"), "message_response",
                           [&](const std::shared_ptr<wolkabout::Message>&) {}, 5, std::chrono::milliseconds{300}}));
    std::this_thread::sleep_for(std::chrono::milliseconds{600});
    ASSERT_NO_FATAL_FAILURE(service->m_messages.clear());
}

TEST_F(OutboundRetryMessageHandlerTests, AddSingleMessageGetsFlagged)
{
    EXPECT_CALL(outboundMessageHandlerMock, addMessage).Times(2);
    ASSERT_NO_FATAL_FAILURE(
      service->addMessage({std::make_shared<wolkabout::Message>("Hello!", "message"), "message_response",
                           [&](const std::shared_ptr<wolkabout::Message>&) {}, 5, std::chrono::milliseconds{300}}));
    std::this_thread::sleep_for(std::chrono::milliseconds{600});
    auto& tuple = service->m_messages.begin()->second;
    ASSERT_NO_FATAL_FAILURE(std::get<3>(tuple) = true);
}
