/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#include "MqttClient.h"

#include <utility>

namespace wolkabout
{
MqttClient::MqttClient() : m_trustStore(""), m_lastWillTopic(""), m_lastWillMessage(""), m_lastWillRetain(false) {}

void MqttClient::onMessageReceived(MqttClient::OnMessageReceivedCallback callback)
{
    m_onMessageReceived = std::move(callback);
}

void MqttClient::onConnectionLost(MqttClient::OnConnectionLostCallback callback)
{
    m_onConnectionLost = std::move(callback);
}

void MqttClient::setLastWill(const std::string& topic, const std::string& message, bool retained)
{
    std::lock_guard<decltype(m_variableLock)> lg{m_variableLock};
    m_lastWillTopic = topic;
    m_lastWillMessage = message;
    m_lastWillRetain = retained;
}

std::string MqttClient::getLastWillTopic() const
{
    std::lock_guard<decltype(m_variableLock)> lg{m_variableLock};
    return m_lastWillTopic;
}

std::string MqttClient::getLastWillMessage() const
{
    std::lock_guard<decltype(m_variableLock)> lg{m_variableLock};
    return m_lastWillMessage;
}

bool MqttClient::getLastWillRetain() const
{
    std::lock_guard<decltype(m_variableLock)> lg{m_variableLock};
    return m_lastWillRetain;
}

void MqttClient::setTrustStore(const std::string& trustStore)
{
    std::lock_guard<decltype(m_variableLock)> lg{m_variableLock};
    m_trustStore = trustStore;
}

std::string MqttClient::getTrustStore() const
{
    std::lock_guard<decltype(m_variableLock)> lg{m_variableLock};
    return m_trustStore;
}

}    // namespace wolkabout
