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

#include "protocol/json/JsonRegistrationProtocol.h"
#include "model/DataType.h"
#include "model/GatewayUpdateRequest.h"
#include "model/GatewayUpdateResponse.h"
#include "model/SubdeviceRegistrationRequest.h"
#include "model/SubdeviceRegistrationResponse.h"
// #include "model/DeviceReregistrationResponse.h"
#include "model/Message.h"
#include "protocol/json/JsonDto.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <cassert>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonRegistrationProtocol::NAME = "JsonRegistrationProtocol";

const std::string JsonRegistrationProtocol::CHANNEL_DELIMITER = "/";
const std::string JsonRegistrationProtocol::CHANNEL_WILDCARD = "#";
const std::string JsonRegistrationProtocol::DEVICE_PATH_PREFIX = "d/";
const std::string JsonRegistrationProtocol::DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string JsonRegistrationProtocol::PLATFORM_TO_DEVICE_DIRECTION = "p2d/";

const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_REQUEST_TOPIC_ROOT =
  "d2p/register_subdevice_request/";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT =
  "p2d/register_subdevice_response/";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_REQUEST_TOPIC_ROOT = "d2p/update_gateway_request/";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT = "p2d/update_gateway_response/";
const std::string JsonRegistrationProtocol::DEVICE_REREGISTRATION_REQUEST_TOPIC_ROOT = "p2d/reregister_device/";
const std::string JsonRegistrationProtocol::DEVICE_REREGISTRATION_RESPONSE_TOPIC_ROOT = "d2p/reregister_device/";

const std::vector<std::string> JsonRegistrationProtocol::INBOUND_CHANNELS = {
  SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT + DEVICE_PATH_PREFIX, GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT,
  DEVICE_REREGISTRATION_REQUEST_TOPIC_ROOT + DEVICE_PATH_PREFIX};

const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_OK = "OK";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_ERROR_GATEWAY_NOT_FOUND = "ERROR_GATEWAY_NOT_FOUND";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_ERROR_NOT_A_GATEWAY = "ERROR_NOT_A_GATEWAY";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_ERROR_VALIDATION_ERROR = "ERROR_VALIDATION_ERROR";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_ERROR_INVALID_DTO = "ERROR_INVALID_DTO";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_ERROR_KEY_MISSING = "ERROR_KEY_MISSING";
const std::string JsonRegistrationProtocol::GATEWAY_UPDATE_RESPONSE_ERROR_UNKNOWN = "ERROR_UNKNOWN";

const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_OK = "OK";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_GATEWAY_NOT_FOUND =
  "ERROR_GATEWAY_NOT_FOUND";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_NOT_A_GATEWAY = "ERROR_NOT_A_GATEWAY";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_VALIDATION_ERROR =
  "ERROR_VALIDATION_ERROR";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_INVALID_DTO = "ERROR_INVALID_DTO";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_KEY_MISSING = "ERROR_KEY_MISSING";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_KEY_CONFLICT = "ERROR_KEY_CONFLICT";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN =
  "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_MAX_NUMBER_OF_DEVICES_EXCEEDED =
  "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED";
const std::string JsonRegistrationProtocol::SUBDEVICE_REGISTRATION_RESPONSE_ERROR_UNKNOWN = "ERROR_UNKNOWN";

const std::string& JsonRegistrationProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> JsonRegistrationProtocol::getInboundChannels() const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [](const std::string& source) { return source + CHANNEL_WILDCARD; });
    return channels;
}

std::vector<std::string> JsonRegistrationProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + deviceKey; });
    return channels;
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

bool JsonRegistrationProtocol::isSubdeviceRegistrationResponseMessage(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT);
}

bool JsonRegistrationProtocol::isGatewayUpdateResponseMessage(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT);
}

std::unique_ptr<Message> JsonRegistrationProtocol::makeMessage(const SubdeviceRegistrationRequest& request) const
{
    LOG(DEBUG) << METHOD_INFO;

    try
    {
        const json jsonPayload(request);
        std::string channel;

        channel = SUBDEVICE_REGISTRATION_REQUEST_TOPIC_ROOT + DEVICE_PATH_PREFIX + request.getSubdeviceKey();

        return std::unique_ptr<Message>(new Message(jsonPayload.dump(), channel));
    }
    catch (...)
    {
        return nullptr;
    }
}

std::unique_ptr<SubdeviceRegistrationResponse> JsonRegistrationProtocol::makeSubdeviceRegistrationResponse(
  const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const json j = json::parse(message.getContent());

        const std::string typeStr = j.at("result").get<std::string>();

        const SubdeviceRegistrationResponse::Result result = [&] {
            if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_OK)
            {
                return SubdeviceRegistrationResponse::Result::OK;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_GATEWAY_NOT_FOUND)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_GATEWAY_NOT_FOUND;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_KEY_CONFLICT)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_KEY_CONFLICT;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_MAX_NUMBER_OF_DEVICES_EXCEEDED)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_VALIDATION_ERROR)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_VALIDATION_ERROR;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_INVALID_DTO)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_INVALID_DTO;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_KEY_MISSING)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_KEY_MISSING;
            }
            else if (typeStr == SUBDEVICE_REGISTRATION_RESPONSE_ERROR_UNKNOWN)
            {
                return SubdeviceRegistrationResponse::Result::ERROR_UNKNOWN;
            }

            assert(false);
            throw std::logic_error("");
        }();

        return std::unique_ptr<SubdeviceRegistrationResponse>(new SubdeviceRegistrationResponse(
          j["payload"].at("deviceKey").get<std::string>(), result, j.at("description").get<std::string>()));
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Device registration protocol: Unable to deserialize device "
                      "registration response: "
                   << e.what();
        return nullptr;
    }
}

std::unique_ptr<GatewayUpdateResponse> JsonRegistrationProtocol::makeGatewayUpdateResponse(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const json j = json::parse(message.getContent());

        const std::string typeStr = j.at("result").get<std::string>();

        const GatewayUpdateResponse::Result result = [&] {
            if (typeStr == GATEWAY_UPDATE_RESPONSE_OK)
            {
                return GatewayUpdateResponse::Result::OK;
            }
            else if (typeStr == GATEWAY_UPDATE_RESPONSE_ERROR_GATEWAY_NOT_FOUND)
            {
                return GatewayUpdateResponse::Result::ERROR_GATEWAY_NOT_FOUND;
            }
            else if (typeStr == GATEWAY_UPDATE_RESPONSE_ERROR_NOT_A_GATEWAY)
            {
                return GatewayUpdateResponse::Result::ERROR_NOT_A_GATEWAY;
            }
            else if (typeStr == GATEWAY_UPDATE_RESPONSE_ERROR_VALIDATION_ERROR)
            {
                return GatewayUpdateResponse::Result::ERROR_VALIDATION_ERROR;
            }
            else if (typeStr == GATEWAY_UPDATE_RESPONSE_ERROR_INVALID_DTO)
            {
                return GatewayUpdateResponse::Result::ERROR_INVALID_DTO;
            }
            else if (typeStr == GATEWAY_UPDATE_RESPONSE_ERROR_KEY_MISSING)
            {
                return GatewayUpdateResponse::Result::ERROR_KEY_MISSING;
            }
            else if (typeStr == GATEWAY_UPDATE_RESPONSE_ERROR_UNKNOWN)
            {
                return GatewayUpdateResponse::Result::ERROR_UNKNOWN;
            }

            assert(false);
            throw std::logic_error("");
        }();

        return std::unique_ptr<GatewayUpdateResponse>(
          new GatewayUpdateResponse(result, j.at("description").get<std::string>()));
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Device registration protocol: Unable to deserialize device "
                      "registration response: "
                   << e.what();
        return nullptr;
    }
}

std::unique_ptr<Message> JsonRegistrationProtocol::makeMessage(const GatewayUpdateRequest& request) const
{
    LOG(DEBUG) << METHOD_INFO;

    try
    {
        const json jsonPayload(request);
        std::string channel;

        channel = GATEWAY_UPDATE_REQUEST_TOPIC_ROOT;

        return std::unique_ptr<Message>(new Message(jsonPayload.dump(), channel));
    }
    catch (...)
    {
        return nullptr;
    }
}
}    // namespace wolkabout
