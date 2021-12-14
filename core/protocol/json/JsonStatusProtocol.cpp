/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#include "JsonStatusProtocol.h"

#include "Json.h"
#include "JsonDto.h"
#include "core/model/DeviceStatus.h"
#include "core/model/DeviceStatusConfirm.h"
#include "core/model/Message.h"
#include "core/utilities/Logger.h"
#include "core/utilities/StringUtils.h"
#include "core/utilities/json.hpp"

using nlohmann::json;

namespace wolkabout
{
const std::string JsonStatusProtocol::LAST_WILL_TOPIC = "lastwill";
const std::string JsonStatusProtocol::DEVICE_STATUS_REQUEST_TOPIC_ROOT = "p2d/subdevice_status_request/";
const std::string JsonStatusProtocol::DEVICE_STATUS_RESPONSE_TOPIC_ROOT = "d2p/subdevice_status_response/";
const std::string JsonStatusProtocol::DEVICE_STATUS_UPDATE_TOPIC_ROOT = "d2p/subdevice_status_update/";
const std::string JsonStatusProtocol::DEVICE_STATUS_CONFIRM_TOPIC_ROOT = "p2d/subdevice_status_confirm/";
const std::string JsonStatusProtocol::PONG_TOPIC_ROOT = "pong/";
const std::string JsonStatusProtocol::PING_TOPIC_ROOT = "ping/";

const std::vector<std::string> JsonStatusProtocol::INBOUND_CHANNELS = {
  DEVICE_STATUS_REQUEST_TOPIC_ROOT + DEVICE_PATH_PREFIX, PONG_TOPIC_ROOT};

const std::string JsonStatusProtocol::STATUS_RESPONSE_STATE_FIELD = "state";
const std::string JsonStatusProtocol::STATUS_RESPONSE_STATUS_CONNECTED = "CONNECTED";
const std::string JsonStatusProtocol::STATUS_RESPONSE_STATUS_SLEEP = "SLEEP";
const std::string JsonStatusProtocol::STATUS_RESPONSE_STATUS_SERVICE = "SERVICE_MODE";
const std::string JsonStatusProtocol::STATUS_RESPONSE_STATUS_OFFLINE = "OFFLINE";

void to_json(json& j, const DeviceStatus& p)
{
    const std::string status = [&]() -> std::string
    {
        switch (p.getStatus())
        {
        case DeviceStatus::Status::CONNECTED:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_CONNECTED;
        }
        case DeviceStatus::Status::SLEEP:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_SLEEP;
        }
        case DeviceStatus::Status::SERVICE:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_SERVICE;
        }
        case DeviceStatus::Status::OFFLINE:
        default:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_OFFLINE;
        }
        }
    }();

    j = json{{JsonStatusProtocol::STATUS_RESPONSE_STATE_FIELD, status}};
}

/*** DEVICE STATUS CONFIRM DTO ***/
void to_json(json& j, const DeviceStatusConfirm& dto)
{
    // clang-format off
    j = {
        {"result", dto.getResult().getMessage()}
    };
    // clang-format on
}
/*** DEVICE STATUS CONFIRM DTO ***/

JsonStatusProtocol::JsonStatusProtocol(bool isGateway) : m_isGateway{isGateway}
{
    if (isGateway)
    {
        m_devicePrefix = GATEWAY_PATH_PREFIX;
    }
    else
    {
        m_devicePrefix = DEVICE_PATH_PREFIX;
    }
}

std::vector<std::string> JsonStatusProtocol::getInboundChannels() const
{
    return {DEVICE_STATUS_REQUEST_TOPIC_ROOT + m_devicePrefix + CHANNEL_MULTI_LEVEL_WILDCARD,
            DEVICE_STATUS_CONFIRM_TOPIC_ROOT + m_devicePrefix + CHANNEL_MULTI_LEVEL_WILDCARD, PONG_TOPIC_ROOT};
}

std::vector<std::string> JsonStatusProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {
      DEVICE_STATUS_REQUEST_TOPIC_ROOT + m_devicePrefix + deviceKey + CHANNEL_DELIMITER + CHANNEL_MULTI_LEVEL_WILDCARD,
      DEVICE_STATUS_CONFIRM_TOPIC_ROOT + m_devicePrefix + deviceKey + CHANNEL_DELIMITER + CHANNEL_MULTI_LEVEL_WILDCARD,
      PONG_TOPIC_ROOT + deviceKey};
}

bool JsonStatusProtocol::isStatusRequestMessage(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), DEVICE_STATUS_REQUEST_TOPIC_ROOT);
}

bool JsonStatusProtocol::isStatusConfirmMessage(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), DEVICE_STATUS_CONFIRM_TOPIC_ROOT);
}

bool JsonStatusProtocol::isPongMessage(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), PONG_TOPIC_ROOT);
}

std::unique_ptr<Message> JsonStatusProtocol::makeStatusResponseMessage(const std::string& deviceKey,
                                                                       const wolkabout::DeviceStatus& response) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        std::string topic;
        if (m_isGateway && deviceKey != response.getDeviceKey())
        {
            topic = DEVICE_STATUS_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                    DEVICE_PATH_PREFIX + response.getDeviceKey();
        }
        else
        {
            topic = DEVICE_STATUS_RESPONSE_TOPIC_ROOT + m_devicePrefix + deviceKey;
        }

        const json jPayload(response);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Status protocol: Unable to serialize device status response: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Status protocol: Unable to serialize device status response";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonStatusProtocol::makeStatusUpdateMessage(const std::string& deviceKey,
                                                                     const wolkabout::DeviceStatus& response) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        std::string topic;
        if (m_isGateway && deviceKey != response.getDeviceKey())
        {
            topic = DEVICE_STATUS_UPDATE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                    DEVICE_PATH_PREFIX + response.getDeviceKey();
        }
        else
        {
            topic = DEVICE_STATUS_UPDATE_TOPIC_ROOT + m_devicePrefix + deviceKey;
        }

        const json jPayload(response);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Status protocol: Unable to serialize device status update: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Status protocol: Unable to serialize device status update";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonStatusProtocol::makeLastWillMessage(const std::string& deviceKey) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string topic = LAST_WILL_TOPIC + CHANNEL_DELIMITER + deviceKey;

    return std::unique_ptr<Message>(new Message("", topic));
}

std::unique_ptr<Message> JsonStatusProtocol::makeLastWillMessage(const std::vector<std::string>& deviceKeys) const
{
    LOG(TRACE) << METHOD_INFO;

    if (deviceKeys.size() == 1)
    {
        return makeLastWillMessage(deviceKeys.front());
    }

    try
    {
        const std::string topic = LAST_WILL_TOPIC;
        const json jPayload(deviceKeys);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Status protocol: Unable to serialize last will message: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Status protocol: Unable to serialize last will message";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonStatusProtocol::makeFromPingRequest(const std::string& deviceKey) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string topic = PING_TOPIC_ROOT + deviceKey;

    return std::unique_ptr<Message>(new Message("", topic));
}

std::string JsonStatusProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string devicePathPrefix = CHANNEL_DELIMITER + DEVICE_PATH_PREFIX;

    const auto deviceKeyStartPosition = topic.find(devicePathPrefix);
    if (deviceKeyStartPosition != std::string::npos)
    {
        const auto keyEndPosition = topic.find(CHANNEL_DELIMITER, deviceKeyStartPosition + devicePathPrefix.size());

        const auto pos = deviceKeyStartPosition + devicePathPrefix.size();

        return topic.substr(pos, keyEndPosition - pos);
    }

    return "";
}
}    // namespace wolkabout
