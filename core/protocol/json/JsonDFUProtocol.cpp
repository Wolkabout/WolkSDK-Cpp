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

#include "protocol/json/JsonDFUProtocol.h"

#include "model/FirmwareUpdateAbort.h"
#include "model/FirmwareUpdateInstall.h"
#include "model/FirmwareUpdateStatus.h"
#include "model/FirmwareVersion.h"
#include "model/Message.h"
#include "protocol/json/Json.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonDFUProtocol::FIRMWARE_UPDATE_STATUS_TOPIC_ROOT = "d2p/firmware_update_status/";
const std::string JsonDFUProtocol::FIRMWARE_VERSION_TOPIC_ROOT = "d2p/firmware_version_update/";

const std::string JsonDFUProtocol::FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT = "p2d/firmware_update_install/";
const std::string JsonDFUProtocol::FIRMWARE_UPDATE_ABORT_TOPIC_ROOT = "p2d/firmware_update_abort/";

/*** FIRMWARE UPDATE STATUS ***/
static void to_json(json& j, const FirmwareUpdateStatus& p)
{
    const std::string status = [&]() -> std::string {
        switch (p.getStatus())
        {
        case FirmwareUpdateStatus::Status::INSTALLATION:
            return "INSTALLATION";
        case FirmwareUpdateStatus::Status::COMPLETED:
            return "COMPLETED";
        case FirmwareUpdateStatus::Status::ABORTED:
            return "ABORTED";
        case FirmwareUpdateStatus::Status::ERROR:
        default:
            return "ERROR";
        }
    }();

    j = json{{"status", status}};

    if (p.getErrorCode())
    {
        auto errorCode = p.getErrorCode().value();

        j.emplace("error", static_cast<int>(errorCode));
    }

    if (!p.getDeviceKeys().empty())
    {
        j["devices"] = p.getDeviceKeys();
    }
}
/*** FIRMWARE UPDATE STATUS ***/

/*** FIRMWARE UPDATE INSTALL ***/
static FirmwareUpdateInstall firmware_update_install_from_json(const json& j)
{
    const std::vector<std::string> deviceKeys = [&]() -> std::vector<std::string> {
        if (j.find("devices") != j.end())
        {
            return j.at("devices").get<std::vector<std::string>>();
        }

        return {};
    }();

    const std::string name = [&]() -> std::string {
        if (j.find("fileName") != j.end())
        {
            return j.at("fileName").get<std::string>();
        }

        return "";
    }();

    return FirmwareUpdateInstall(deviceKeys, name);
}
/*** FIRMWARE UPDATE INSTALL ***/

/*** FIRMWARE UPDATE ABORT ***/
static FirmwareUpdateAbort firmware_update_abort_from_json(const json& j)
{
    const std::vector<std::string> deviceKeys = [&]() -> std::vector<std::string> {
        if (j.find("devices") != j.end())
        {
            return j.at("devices").get<std::vector<std::string>>();
        }

        return {};
    }();

    return FirmwareUpdateAbort(deviceKeys);
}
/*** FIRMWARE UPDATE ABORT ***/

JsonDFUProtocol::JsonDFUProtocol(bool isGateway) : m_isGateway{isGateway} {}

std::vector<std::string> JsonDFUProtocol::getInboundChannels() const
{
    return {
      FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT + DEVICE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
      FIRMWARE_UPDATE_ABORT_TOPIC_ROOT + DEVICE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
      FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
      FIRMWARE_UPDATE_ABORT_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
    };
}

std::vector<std::string> JsonDFUProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {
      FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey,
      FIRMWARE_UPDATE_ABORT_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey,
      FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + DEVICE_PATH_PREFIX +
        CHANNEL_MULTI_LEVEL_WILDCARD,
      FIRMWARE_UPDATE_ABORT_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + DEVICE_PATH_PREFIX +
        CHANNEL_MULTI_LEVEL_WILDCARD,
    };
}

std::unique_ptr<Message> JsonDFUProtocol::makeMessage(const std::string& deviceKey,
                                                      const FirmwareUpdateStatus& firmwareUpdateStatus) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        std::string topic = FIRMWARE_UPDATE_STATUS_TOPIC_ROOT;

        if (m_isGateway)
        {
            // update status of gateway
            if (firmwareUpdateStatus.getDeviceKeys().size() == 1 &&
                firmwareUpdateStatus.getDeviceKeys()[0] == deviceKey)
            {
                topic += DEVICE_PATH_PREFIX + deviceKey;
            }
            // update status of single subdevice
            else if (firmwareUpdateStatus.getDeviceKeys().size() == 1 &&
                     firmwareUpdateStatus.getDeviceKeys()[0] != deviceKey)
            {
                topic += GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + DEVICE_PATH_PREFIX +
                         firmwareUpdateStatus.getDeviceKeys()[0];
            }
            // update status of multiple subdevices
            else
            {
                topic += DEVICE_PATH_PREFIX + deviceKey;
            }
        }
        else
        {
            topic += DEVICE_PATH_PREFIX + deviceKey;
        }

        const json jPayload(firmwareUpdateStatus);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Firmware update protocol: Unable to serialize firmware update status: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Firmware update protocol: Unable to serialize firmware update status";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDFUProtocol::makeMessage(const std::string& deviceKey,
                                                      const FirmwareVersion& version) const
{
    LOG(TRACE) << METHOD_INFO;

    std::string topic;
    // subdevice
    if (m_isGateway && deviceKey != version.getDeviceKey())
    {
        topic = FIRMWARE_VERSION_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + DEVICE_PATH_PREFIX +
                version.getDeviceKey();
    }
    // gateway or regular device
    else
    {
        topic = FIRMWARE_VERSION_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;
    }

    return std::unique_ptr<Message>(new Message(version.getVersion(), topic));
}

std::unique_ptr<FirmwareUpdateInstall> JsonDFUProtocol::makeFirmwareUpdateInstall(
  const wolkabout::Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FirmwareUpdateInstall>(new FirmwareUpdateInstall(firmware_update_install_from_json(j)));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Firmware update protocol: Unable to deserialize firmware install command: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Firmware update protocol: Unable to deserialize firmware install command";
        return nullptr;
    }
}

std::unique_ptr<FirmwareUpdateAbort> JsonDFUProtocol::makeFirmwareUpdateAbort(const wolkabout::Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), FIRMWARE_UPDATE_ABORT_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FirmwareUpdateAbort>(new FirmwareUpdateAbort(firmware_update_abort_from_json(j)));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "Firmware update protocol: Unable to deserialize firmware abort command: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "Firmware update protocol: Unable to deserialize firmware abort command";
        return nullptr;
    }
}

std::string JsonDFUProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
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
