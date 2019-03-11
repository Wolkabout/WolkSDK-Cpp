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

#include "protocol/json/JsonStatusProtocol.h"
#include "Json.h"
#include "model/DeviceStatus.h"
#include "model/DeviceStatusResponse.h"
#include "model/DeviceStatusUpdate.h"
#include "model/DeviceStatusConfirm.h"
#include "model/Message.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

using nlohmann::json;

namespace wolkabout
{
const std::string JsonStatusProtocol::NAME = "JsonStatusProtocol";

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
const std::string JsonStatusProtocol::STATUS_RESPONSE_STATUS_SERVICE = "SERVICE";
const std::string JsonStatusProtocol::STATUS_RESPONSE_STATUS_OFFLINE = "OFFLINE";

void to_json(json& j, const DeviceStatusResponse& p)
{
    const std::string status = [&]() -> std::string {
        switch (p.getStatus())
        {
        case DeviceStatus::CONNECTED:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_CONNECTED;
        }
        case DeviceStatus::SLEEP:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_SLEEP;
        }
        case DeviceStatus::SERVICE:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_SERVICE;
        }
        case DeviceStatus::OFFLINE:
        default:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_OFFLINE;
        }
        }
    }();

    j = json{{JsonStatusProtocol::STATUS_RESPONSE_STATE_FIELD, status}};
}

void to_json(json& j, const DeviceStatusUpdate& p)
{
    const std::string status = [&]() -> std::string {
        switch (p.getStatus())
        {
        case DeviceStatus::CONNECTED:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_CONNECTED;
        }
        case DeviceStatus::SLEEP:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_SLEEP;
        }
        case DeviceStatus::SERVICE:
        {
            return JsonStatusProtocol::STATUS_RESPONSE_STATUS_SERVICE;
        }
        case DeviceStatus::OFFLINE:
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
    auto resultStr = [&]() -> std::string {
        switch (dto.getResult())
        {
            case PlatformResult::OK:
                return "OK";
                break;

            default:
                throw std::invalid_argument("Unhandled result");
        }
    }();

    // clang-format off
    j = {
            {"result", resultStr}
    };
    // clang-format on
}
/*** DEVICE STATUS CONFIRM DTO ***/

const std::string& JsonStatusProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> JsonStatusProtocol::getInboundChannels() const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [](const std::string& source) { return source + CHANNEL_MULTI_LEVEL_WILDCARD; });
    return channels;
}

std::vector<std::string> JsonStatusProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + deviceKey; });
    return channels;
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
    return StringUtils::startsWith(message.getChannel(), PONG_TOPIC_ROOT);
}

std::unique_ptr<Message> JsonStatusProtocol::makeMessage(const std::string& deviceKey,
                                                         const DeviceStatusResponse& response) const
{
    LOG(TRACE) << METHOD_INFO;

    const json jPayload(response);
    const std::string topic = DEVICE_STATUS_RESPONSE_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

    const std::string payload = jPayload.dump();

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> JsonStatusProtocol::makeMessage(const std::string& deviceKey,
                                                         const DeviceStatusUpdate& response) const
{
    LOG(TRACE) << METHOD_INFO;

    const json jPayload(response);
    const std::string topic = DEVICE_STATUS_UPDATE_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

    const std::string payload = jPayload.dump();

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> JsonStatusProtocol::makeLastWillMessage(const std::string& deviceKey) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string topic = LAST_WILL_TOPIC + CHANNEL_DELIMITER + deviceKey;
    const std::string payload = "";

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> JsonStatusProtocol::makeLastWillMessage(const std::vector<std::string>& deviceKeys) const
{
    LOG(TRACE) << METHOD_INFO;

    if (deviceKeys.size() == 1)
    {
        return makeLastWillMessage(deviceKeys.front());
    }
    else
    {
        const std::string topic = LAST_WILL_TOPIC;
        const json jPayload(deviceKeys);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
}

std::unique_ptr<Message> JsonStatusProtocol::makeFromPingRequest(const std::string& deviceKey) const
{
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
