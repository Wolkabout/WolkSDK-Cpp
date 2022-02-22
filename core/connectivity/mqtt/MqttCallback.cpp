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

#include "core/connectivity/mqtt/MqttCallback.h"

#include <utility>

namespace wolkabout
{
MqttCallback::MqttCallback(std::function<void()> onConnect, std::function<void()> onConnectionLost,
                           std::function<void(mqtt::const_message_ptr)> onMessageArrived,
                           std::function<void(mqtt::delivery_token_ptr)> onDeliveryComplete)
: m_onConnectCallback(std::move(onConnect))
, m_onConnectionLostCallback(std::move(onConnectionLost))
, m_onMessageArrivedCallback(std::move(onMessageArrived))
, m_onDeliveryCompleteCallback(std::move(onDeliveryComplete))
{
}

void MqttCallback::connected(const mqtt::string& /* cause */)
{
    m_onConnectCallback();
}

void MqttCallback::connection_lost(const mqtt::string& /* cause */)
{
    m_onConnectionLostCallback();
}

void MqttCallback::message_arrived(mqtt::const_message_ptr msg)
{
    m_onMessageArrivedCallback(msg);
}

void MqttCallback::delivery_complete(mqtt::delivery_token_ptr tok)
{
    m_onDeliveryCompleteCallback(tok);
}
}    // namespace wolkabout
