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

#ifndef MQTTCONNECTIVITYSERVICE_H
#define MQTTCONNECTIVITYSERVICE_H

#include "core/connectivity/ConnectivityService.h"
#include "core/connectivity/OutboundMessageHandler.h"
#include "core/connectivity/mqtt/MqttClient.h"
#include "core/utilities/Buffer.h"

#include <atomic>
#include <memory>
#include <string>
#include <thread>

namespace wolkabout
{
class MessagePersistence;

class MqttConnectivityService : public ConnectivityService, public OutboundMessageHandler
{
public:
    MqttConnectivityService(std::shared_ptr<MqttClient> mqttClient, std::string key, std::string password,
                            std::string host, std::string trustStore, std::string clientId,
                            std::shared_ptr<MessagePersistence> messagePersistence = nullptr);

    ~MqttConnectivityService() override;

    bool connect() override;
    void disconnect() override;
    bool reconnect() override;

    bool isConnected() override;

    bool publish(std::shared_ptr<Message> outboundMessages) override;

    void addMessage(std::shared_ptr<Message> message) override;

private:
    class State
    {
    public:
        State(MqttConnectivityService& service);
        virtual ~State() = default;
        virtual void run() = 0;

    protected:
        MqttConnectivityService& m_service;
    };

    class DisconnectedState : public State
    {
    public:
        using State::State;
        void run() override;
    };

    class ConnectedState : public State
    {
    public:
        using State::State;
        void run() override;
    };

    void changeToState(State* state);

    void run();

    // ConnectivityService fields
    std::shared_ptr<MqttClient> m_mqttClient;

    const std::string m_key;
    const std::string m_password;
    const std::string m_host;
    const std::string m_trustStore;
    const std::string m_clientId;

    std::string m_lastWillChannel;
    std::string m_lastWillPayload;
    bool m_lastWillRetain;

    // OutboundMessageHandler fields
    std::shared_ptr<MessagePersistence> m_persistence;

    ConnectedState m_connectedState;
    DisconnectedState m_disconnectedState;
    std::atomic<State*> m_currentState;

    Buffer<std::shared_ptr<Message>> m_buffer;

    std::atomic_bool m_run;
    std::unique_ptr<std::thread> m_worker;
};
}    // namespace wolkabout

#endif
