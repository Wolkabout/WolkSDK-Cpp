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

#ifndef MQTTCALLBACK_H
#define MQTTCALLBACK_H

#include <functional>
#include <mqtt/callback.h>
#include <string>

namespace wolkabout
{
class MqttCallback : public mqtt::callback
{
public:
    MqttCallback(std::function<void()> onConnect, std::function<void()> onConnectionLost,
                 std::function<void(mqtt::const_message_ptr)> onMessageArrived,
                 std::function<void(mqtt::delivery_token_ptr)> onDeliveryComplete);
    void connected(const mqtt::string& cause) override;
    void connection_lost(const mqtt::string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr tok) override;

private:
    std::function<void()> m_onConnectCallback;
    std::function<void()> m_onConnectionLostCallback;
    std::function<void(mqtt::const_message_ptr)> m_onMessageArrivedCallback;
    std::function<void(mqtt::delivery_token_ptr)> m_onDeliveryCompleteCallback;
};
}    // namespace wolkabout

#endif
