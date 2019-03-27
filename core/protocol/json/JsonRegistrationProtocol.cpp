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

#include "protocol/json/JsonRegistrationProtocol.h"
#include "model/GatewayUpdateRequest.h"
#include "model/GatewayUpdateResponse.h"
#include "model/Message.h"
#include "model/SubdeviceDeletionRequest.h"
#include "model/SubdeviceRegistrationRequest.h"
#include "model/SubdeviceRegistrationResponse.h"
#include "protocol/json/Json.h"
#include "protocol/json/JsonDto.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <cassert>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_REQUEST_TOPIC_ROOT =
  "d2p/register_subdevice_request/";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT =
  "p2d/register_subdevice_response/";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_REQUEST_TOPIC_ROOT = "d2p/update_gateway_request/";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT = "p2d/update_gateway_response/";
const std::string JsonRegistrationProtocol::SUBDEVICE_DELETION_REQUEST_TOPIC_ROOT = "d2p/delete_subdevice_request/";
const std::string JsonRegistrationProtocol::SUBDEVICE_DELETION_RESPONSE_TOPIC_ROOT = "p2d/delete_subdevice_response/";

std::vector<std::string> JsonRegistrationProtocol::getInboundChannels() const
{
    return {SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
            GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
            SUBDEVICE_DELETION_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD};
}

std::vector<std::string> JsonRegistrationProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey,
            GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey,
            SUBDEVICE_DELETION_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey};
}

std::string JsonRegistrationProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
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

bool JsonRegistrationProtocol::isSubdeviceRegistrationResponse(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT);
}

bool JsonRegistrationProtocol::isGatewayUpdateResponse(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT);
}

bool JsonRegistrationProtocol::isSubdeviceDeletionResponse(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), SUBDEVICE_DELETION_RESPONSE_TOPIC_ROOT);
}

std::unique_ptr<Message> JsonRegistrationProtocol::makeMessage(const std::string& deviceKey,
                                                               const SubdeviceRegistrationRequest& request) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const json jsonPayload(request);
        const std::string channel = SUBDEVICE_REGISTRATION_REQUEST_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey;

        const auto content = jsonPayload.dump();

        return std::unique_ptr<Message>(new Message(content, channel));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to serialize subdevice registration request: "
                   << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to serialize subdevice registration request";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonRegistrationProtocol::makeMessage(const std::string& deviceKey,
                                                               const GatewayUpdateRequest& request) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const json jsonPayload(request);
        const auto channel = GATEWAY_UPDATE_REQUEST_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey;

        const auto content = jsonPayload.dump();

        return std::unique_ptr<Message>(new Message(content, channel));
    }
    catch (const std::exception& e)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to serialize gateway update request: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to serialize gateway update request";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonRegistrationProtocol::makeMessage(const std::string& deviceKey,
                                                               const SubdeviceDeletionRequest& request) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string channel = SUBDEVICE_DELETION_REQUEST_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey +
                                CHANNEL_DELIMITER + DEVICE_PATH_PREFIX + request.getDeviceKey();

    return std::unique_ptr<Message>(new Message("", channel));
}

std::unique_ptr<SubdeviceRegistrationResponse> JsonRegistrationProtocol::makeSubdeviceRegistrationResponse(
  const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        const json jsonResponse = json::parse(message.getContent());
        SubdeviceRegistrationResponse response = subdevice_registration_response_from_json(jsonResponse);

        return std::unique_ptr<SubdeviceRegistrationResponse>(new SubdeviceRegistrationResponse(response));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to deserialize subdevice registration response: "
                   << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to deserialize subdevice registration response";
        return nullptr;
    }
}

std::unique_ptr<GatewayUpdateResponse> JsonRegistrationProtocol::makeGatewayUpdateResponse(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        const json jsonResponse = json::parse(message.getContent());
        GatewayUpdateResponse response = gateway_update_response_from_json(jsonResponse);

        return std::unique_ptr<GatewayUpdateResponse>(new GatewayUpdateResponse(response));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to deserialize subdevice registration response: "
                   << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Subdevice registration protocol: Unable to deserialize subdevice registration response";
        return nullptr;
    }
}

std::string JsonRegistrationProtocol::getResponseChannel(const std::string& deviceKey, const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (StringUtils::startsWith(message.getChannel(), SUBDEVICE_REGISTRATION_REQUEST_TOPIC_ROOT))
    {
        return SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey;
    }
    else if (StringUtils::startsWith(message.getChannel(), SUBDEVICE_DELETION_REQUEST_TOPIC_ROOT))
    {
        return SUBDEVICE_DELETION_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey;
    }
    else if (StringUtils::startsWith(message.getChannel(), GATEWAY_UPDATE_REQUEST_TOPIC_ROOT))
    {
        return GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey;
    }

    return "";
}
}    // namespace wolkabout
