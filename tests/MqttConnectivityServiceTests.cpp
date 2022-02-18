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
#include "core/connectivity/mqtt/MqttConnectivityService.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"
#include "tests/mocks/MessagePersistenceMock.h"
#include "tests/mocks/PahoMqttClientMock.h"

#include <gtest/gtest.h>

using namespace wolkabout;
using namespace ::testing;

class MqttConnectivityServiceTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    void SetUp() override
    {
        mqttClientMock = std::make_shared<StrictMock<PahoMqttClientMock>>();
        messagePersistenceMock = std::make_shared<StrictMock<MessagePersistenceMock>>();
        EXPECT_CALL(*mqttClientMock, isConnected).WillOnce(Return(false));
        service = std::make_shared<MqttConnectivityService>(mqttClientMock, "", "", "", "ca.crt", "");
    }

    std::shared_ptr<MqttConnectivityService> service;

    std::shared_ptr<PahoMqttClientMock> mqttClientMock;

    std::shared_ptr<MessagePersistenceMock> messagePersistenceMock;
};

TEST_F(MqttConnectivityServiceTests, HelloWorld)
{
    LOG(INFO) << "Hello World!";
}
