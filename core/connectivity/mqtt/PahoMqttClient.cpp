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

#include "PahoMqttClient.h"

#include "MqttCallback.h"
#include "MqttClient.h"
#include "core/utilities/Logger.h"

#include <atomic>
#include <memory>
#include <mqtt/async_client.h>
#include <string>

namespace wolkabout
{
const unsigned short PahoMqttClient::MQTT_CONNECTION_COMPLETITION_TIMEOUT_MSEC = 2000;
const unsigned short PahoMqttClient::MQTT_ACTION_COMPLETITION_TIMEOUT_MSEC = 2000;
const unsigned short PahoMqttClient::MQTT_QOS = 2;

PahoMqttClient::PahoMqttClient(unsigned short keepAliveSec) : m_isConnected(false), m_keepAliveSec(keepAliveSec)
{
    m_callback.reset(new MqttCallback(
      [&] {
          LOG(DEBUG) << "Connected";
          m_isConnected = true;
      },
      [&] {
          LOG(DEBUG) << "Connection lost";
          m_isConnected = false;
          if (m_onConnectionLost)
          {
              m_onConnectionLost();
          }
      },
      [&](mqtt::const_message_ptr msg) {
          if (m_onMessageReceived)
          {
              m_onMessageReceived(msg->get_topic(), msg->get_payload_str());
          }
      },
      [&](mqtt::delivery_token_ptr tok) {}));
}

bool PahoMqttClient::connect(const std::string& username, const std::string& password, const std::string& host,
                             const std::string& clientId)
{
    if (m_isConnected)
    {
        LOG(DEBUG) << "Connecting aborted: already connected";
        return true;
    }

    m_isConnected = false;
    m_client.reset(new mqtt::async_client(host, clientId));
    m_client->set_callback(*m_callback);

    mqtt::connect_options connectOptions;
    connectOptions.set_user_name(username);
    connectOptions.set_password(password);
    connectOptions.set_clean_session(true);
    connectOptions.set_keep_alive_interval(m_keepAliveSec);

    mqtt::ssl_options sslOptions;
    sslOptions.set_enable_server_cert_auth(false);
    sslOptions.set_trust_store(getTrustStore());
    connectOptions.set_ssl(sslOptions);

    const auto lastWillTopic = getLastWillTopic();
    const auto lastWillMessage = getLastWillMessage();
    const auto lastWillRetain = getLastWillRetain();

    if (!lastWillTopic.empty())
    {
        mqtt::will_options willOptions;
        willOptions.set_payload(lastWillMessage);
        willOptions.set_qos(MQTT_QOS);
        willOptions.set_retained(lastWillRetain);
        willOptions.set_topic(lastWillTopic);
        connectOptions.set_will(willOptions);
    }

    try
    {
        LOG(DEBUG) << "Connecting to: " << host;
        mqtt::token_ptr token = m_client->connect(connectOptions);
        token->wait_for(std::chrono::milliseconds(MQTT_CONNECTION_COMPLETITION_TIMEOUT_MSEC));

        if (!token->is_complete() || !m_isConnected)
        {
            LOG(DEBUG) << "Connecting failed: token timeout";
            return false;
        }
    }
    catch (mqtt::exception& e)
    {
        LOG(DEBUG) << "Connecting failed: exception code " << e.get_reason_code();
        return false;
    }

    LOG(DEBUG) << "Connecting successful";
    return true;
}

void PahoMqttClient::disconnect()
{
    if (m_isConnected)
    {
        try
        {
            LOG(DEBUG) << "Disconnecting";
            m_isConnected = false;
            m_client->disconnect();
        }
        catch (mqtt::exception& e)
        {
            LOG(DEBUG) << "Disonnecting failed: exception code " << e.get_reason_code();
        }
    }
}

bool PahoMqttClient::isConnected()
{
    return m_isConnected;
}

bool PahoMqttClient::subscribe(const std::string& topic)
{
    if (!m_isConnected)
    {
        LOG(DEBUG) << "Subscribing aborted: not connected";
        return false;
    }

    try
    {
        LOG(DEBUG) << "Subscribing to: " << topic;

        mqtt::token_ptr token = m_client->subscribe(topic, MQTT_QOS);
        token->wait_for(std::chrono::milliseconds(MQTT_ACTION_COMPLETITION_TIMEOUT_MSEC));

        if (!token->is_complete())
        {
            LOG(DEBUG) << "Subscribing failed: token timeout";
            return false;
        }
    }
    catch (mqtt::exception& e)
    {
        LOG(DEBUG) << "Subscribing failed: exception code " << e.get_reason_code();
        return false;
    }

    LOG(TRACE) << "Subscribing successful";
    return true;
}

bool PahoMqttClient::publish(const std::string& topic, const std::string& message, bool retained)
{
    if (!m_isConnected)
    {
        LOG(DEBUG) << "Publishing aborted: not connected";
        return false;
    }

    std::lock_guard<std::mutex> guard{m_mutex};

    try
    {
        LOG(DEBUG) << "Sending message: " << message << ", to: " << topic;

        mqtt::message_ptr pubmsg = mqtt::make_message(topic, message.c_str(), strlen(message.c_str()));
        pubmsg->set_qos(MQTT_QOS);
        pubmsg->set_retained(retained);

        mqtt::token_ptr token = m_client->publish(pubmsg);
        token->wait_for(std::chrono::milliseconds(MQTT_ACTION_COMPLETITION_TIMEOUT_MSEC));

        if (!token->is_complete() || !m_isConnected)
        {
            LOG(DEBUG) << "Publishing failed: token timeout";
            return false;
        }
    }
    catch (mqtt::exception& e)
    {
        LOG(DEBUG) << "Publishing failed: exception code " << e.get_reason_code();
        return false;
    }

    LOG(TRACE) << "Publishing successful";
    return true;
}

PahoMqttClient::~PahoMqttClient() = default;
}    // namespace wolkabout
