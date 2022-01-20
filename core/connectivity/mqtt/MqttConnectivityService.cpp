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

#include "core/connectivity/mqtt/MqttConnectivityService.h"

#include "core/model/Message.h"

namespace wolkabout
{
MqttConnectivityService::MqttConnectivityService(std::shared_ptr<MqttClient> mqttClient, std::string key,
                                                 std::string password, std::string host, std::string trustStore,
                                                 std::string clientId)
: m_mqttClient(std::move(mqttClient))
, m_key(std::move(key))
, m_password(std::move(password))
, m_host(std::move(host))
, m_trustStore(std::move(trustStore))
, m_clientId(std::move(clientId))
, m_lastWillRetain(false)
, m_connectedState{*this}
, m_disconnectedState{*this}
, m_currentState{&m_disconnectedState}
, m_run{true}
, m_worker{new std::thread(&MqttConnectivityService::run, this)}
{
    m_mqttClient->onMessageReceived([this](const std::string& topic, const std::string& message) -> void {
        if (auto handler = m_inboundMessageHandler.lock())
        {
            handler->messageReceived(topic, message);
        }
    });

    m_mqttClient->onConnectionLost([this]() -> void {
        if (m_onConnectionLost)
            m_onConnectionLost();

        changeToState(&m_disconnectedState);
    });

    if (!m_trustStore.empty())
    {
        m_mqttClient->setTrustStore(m_trustStore);
    }
}

MqttConnectivityService::~MqttConnectivityService()
{
    m_run = false;
    m_buffer.stop();
    if (m_worker->joinable())
        m_worker->join();
}

bool MqttConnectivityService::connect()
{
    m_mqttClient->setLastWill(m_lastWillChannel, m_lastWillPayload, m_lastWillRetain);
    bool isConnected = m_mqttClient->connect(m_key, m_password, m_host, m_clientId);
    if (isConnected)
    {
        if (auto handler = m_inboundMessageHandler.lock())
        {
            const auto& topics = handler->getChannels();
            for (const std::string& topic : topics)
            {
                m_mqttClient->subscribe(topic);
            }
        }

        changeToState(&m_connectedState);
    }

    return isConnected;
}

void MqttConnectivityService::disconnect()
{
    m_mqttClient->disconnect();
    changeToState(&m_disconnectedState);
}

bool MqttConnectivityService::reconnect()
{
    disconnect();
    return connect();
}

bool MqttConnectivityService::isConnected()
{
    return m_mqttClient->isConnected();
}

bool MqttConnectivityService::publish(std::shared_ptr<Message> outboundMessage)
{
    return m_mqttClient->publish(outboundMessage->getChannel(), outboundMessage->getContent());
}

void MqttConnectivityService::addMessage(std::shared_ptr<Message> message)
{
    LOG(TRACE) << "MqttConnectivityService: Message added. Channel: '" << message->getChannel() << "' Payload: '"
               << message->getContent() << "'";
    m_buffer.push(std::move(message));
}

void MqttConnectivityService::changeToState(State* state)
{
    // Check if the current state is not the wanted state
    if (m_currentState != state)
    {
        m_currentState = state;
        m_buffer.notify();
    }
}

void MqttConnectivityService::run()
{
    while (m_run)
    {
        auto state = m_currentState.load();
        state->run();
    }

    m_buffer.notify();
}

MqttConnectivityService::State::State(MqttConnectivityService& service) : m_service{service} {}

void MqttConnectivityService::DisconnectedState::run()
{
    while (m_service.m_run && !m_service.isConnected() && !m_service.m_buffer.isEmpty())
    {
        const auto message = m_service.m_buffer.pop();
        if (!message)
            break;

        if (!m_service.m_persistence->push(message))
        {
            LOG(ERROR) << "Failed to persist message";
        }
    }

    m_service.m_buffer.swapBuffers();
}

void MqttConnectivityService::ConnectedState::run()
{
    while (m_service.m_run && m_service.isConnected() && !m_service.m_buffer.isEmpty())
    {
        const auto message = m_service.m_buffer.pop();
        if (!message)
            break;

        if (!m_service.publish(message))
        {
            m_service.m_persistence->push(message);
            LOG(ERROR) << "Failed to publish message";
            break;
        }
    }

    // publish persisted until new message arrives
    while (m_service.m_run && m_service.isConnected() && !m_service.m_persistence->empty() &&
           m_service.m_buffer.isEmpty())
    {
        const auto message = m_service.m_persistence->front();
        if (!message)
            break;

        if (m_service.publish(message))
        {
            m_service.m_persistence->pop();
        }
        else
        {
            LOG(ERROR) << "Failed to publish message";
        }
    }

    m_service.m_buffer.swapBuffers();
}
}    // namespace wolkabout
