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

#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <functional>
#include <mutex>
#include <string>

namespace wolkabout
{
class MqttClient
{
public:
    using OnMessageReceivedCallback = std::function<void(std::string topic, std::string message)>;
    using OnConnectionLostCallback = std::function<void()>;

    MqttClient();

    virtual ~MqttClient() = default;

    virtual bool connect(const std::string& username, const std::string& password, const std::string& address,
                         const std::string& clientId) = 0;
    virtual void disconnect() = 0;

    virtual bool isConnected() = 0;

    virtual bool subscribe(const std::string& topic) = 0;

    virtual bool publish(const std::string& topic, const std::string& message, bool retained = false) = 0;

    void onMessageReceived(OnMessageReceivedCallback callback);

    void onConnectionLost(OnConnectionLostCallback callback);

    std::string getLastWillTopic() const;
    std::string getLastWillMessage() const;
    bool getLastWillRetain() const;
    void setLastWill(const std::string& topic, const std::string& message, bool retained = false);

    std::string getTrustStore() const;
    void setTrustStore(const std::string& trustStore);

protected:
    OnMessageReceivedCallback m_onMessageReceived;
    OnConnectionLostCallback m_onConnectionLost;
    mutable std::mutex m_variableLock;

private:
    std::string m_trustStore;
    std::string m_lastWillTopic;
    std::string m_lastWillMessage;
    bool m_lastWillRetain;
};
}    // namespace wolkabout

#endif
