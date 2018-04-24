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

#include "protocol/json/DFUProtocol.h"
#include "model/FilePacketRequest.h"
#include "model/FirmwareUpdateCommand.h"
#include "model/FirmwareUpdateResponse.h"
#include "model/Message.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string DFUProtocol::NAME = "DFUProtocol";

const std::string DFUProtocol::CHANNEL_DELIMITER = "/";
const std::string DFUProtocol::CHANNEL_WILDCARD = "#";

const std::string DFUProtocol::FIRMWARE_UPDATE_STATUS_TOPIC_ROOT = "service/status/firmware/";
const std::string DFUProtocol::FILE_HANDLING_STATUS_TOPIC_ROOT = "service/status/file/";
const std::string DFUProtocol::FIRMWARE_VERSION_TOPIC_ROOT = "firmware/version/";

const std::string DFUProtocol::FIRMWARE_UPDATE_COMMAND_TOPIC_ROOT = "service/commands/firmware/";
const std::string DFUProtocol::BINARY_TOPIC_ROOT = "service/binary/";

const std::vector<std::string> DFUProtocol::INBOUND_CHANNELS = {FIRMWARE_UPDATE_COMMAND_TOPIC_ROOT, BINARY_TOPIC_ROOT};

/*** FIRMWARE UPDATE RESPONSE ***/
void to_json(json& j, const FirmwareUpdateResponse& p)
{
    const std::string status = [&]() -> std::string {
        switch (p.getStatus())
        {
        case FirmwareUpdateResponse::Status::FILE_TRANSFER:
            return "FILE_TRANSFER";
        case FirmwareUpdateResponse::Status::FILE_READY:
            return "FILE_READY";
        case FirmwareUpdateResponse::Status::INSTALLATION:
            return "INSTALLATION";
        case FirmwareUpdateResponse::Status::COMPLETED:
            return "COMPLETED";
        case FirmwareUpdateResponse::Status::ABORTED:
            return "ABORTED";
        case FirmwareUpdateResponse::Status::ERROR:
            return "ERROR";
        default:
            return "ERROR";
        }
    }();

    j = json{{"status", status}};

    if (!p.getErrorCode().null())
    {
        auto errorCode = static_cast<FirmwareUpdateResponse::ErrorCode>(p.getErrorCode());

        j.emplace("error", static_cast<int>(errorCode));
    }
}

void to_json(json& j, const std::shared_ptr<FirmwareUpdateResponse>& p)
{
    to_json(j, *p);
}
/*** FIRMWARE UPDATE RESPONSE ***/

/*** FILE PACKET_REQUEST ***/
void to_json(json& j, const FilePacketRequest& p)
{
    j = json{{"fileName", p.getFileName()}, {"chunkIndex", p.getChunkIndex()}, {"chunkSize", p.getChunkSize()}};
}

void to_json(json& j, const std::shared_ptr<FilePacketRequest>& p)
{
    to_json(j, *p);
}
/*** FILE PACKET_REQUEST ***/

/*** FIRMWARE UPDATE COMMAND ***/
void from_json(const json& j, FirmwareUpdateCommand& p)
{
    const std::string typeStr = j.at("command").get<std::string>();

    FirmwareUpdateCommand::Type type;
    if (typeStr == "INSTALL")
    {
        type = FirmwareUpdateCommand::Type::INSTALL;
    }
    else if (typeStr == "ABORT")
    {
        type = FirmwareUpdateCommand::Type::ABORT;
    }
    else if (typeStr == "FILE_UPLOAD")
    {
        type = FirmwareUpdateCommand::Type::FILE_UPLOAD;
    }
    else if (typeStr == "URL_DOWNLOAD")
    {
        type = FirmwareUpdateCommand::Type::URL_DOWNLOAD;
    }
    else
    {
        type = FirmwareUpdateCommand::Type::UNKNOWN;
    }

    const bool autoInstall = [&]() -> bool {
        if (j.find("autoInstall") != j.end())
        {
            return j.at("autoInstall").get<bool>();
        }

        return false;
    }();

    if (type == FirmwareUpdateCommand::Type::FILE_UPLOAD)
    {
        const std::string name = [&]() -> std::string {
            if (j.find("fileName") != j.end())
            {
                return j.at("fileName").get<std::string>();
            }

            return "";
        }();

        const uint_fast64_t size = [&]() -> uint_fast64_t {
            if (j.find("fileSize") != j.end())
            {
                return j.at("fileSize").get<uint_fast64_t>();
            }

            return 0;
        }();

        const std::string hash = [&]() -> std::string {
            if (j.find("fileHash") != j.end())
            {
                return j.at("fileHash").get<std::string>();
            }

            return "";
        }();

        p = FirmwareUpdateCommand(type, name, size, hash, autoInstall);
        return;
    }
    else if (type == FirmwareUpdateCommand::Type::URL_DOWNLOAD)
    {
        const std::string url = [&]() -> std::string {
            if (j.find("fileUrl") != j.end())
            {
                return j.at("fileUrl").get<std::string>();
            }

            return "";
        }();

        p = FirmwareUpdateCommand(type, url, autoInstall);
        return;
    }
    else
    {
        p = FirmwareUpdateCommand(type);
    }
}

const std::string& DFUProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> DFUProtocol::getInboundChannels() const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [](const std::string& source) { return source + CHANNEL_WILDCARD; });
    return channels;
}

std::vector<std::string> DFUProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + deviceKey; });
    return channels;
}

std::unique_ptr<Message> DFUProtocol::makeMessage(const std::string& deviceKey,
                                                  const FirmwareUpdateResponse& firmwareUpdateResponse) const
{
    const json jPayload(firmwareUpdateResponse);
    const std::string payload = jPayload.dump();
    const std::string topic = FIRMWARE_UPDATE_STATUS_TOPIC_ROOT + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> DFUProtocol::makeMessage(const std::string& deviceKey,
                                                  const FilePacketRequest& filePacketRequest) const
{
    const json jPayload(filePacketRequest);
    const std::string payload = jPayload.dump();
    const std::string topic = FILE_HANDLING_STATUS_TOPIC_ROOT + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> DFUProtocol::makeFromFirmwareVersion(const std::string& deviceKey,
                                                              const std::string& firmwareVerion) const
{
    const std::string topic = FIRMWARE_VERSION_TOPIC_ROOT + deviceKey;
    return std::unique_ptr<Message>(new Message(firmwareVerion, topic));
}

std::unique_ptr<FirmwareUpdateCommand> DFUProtocol::makeFirmwareUpdateCommand(const Message& message) const
{
    try
    {
        const std::string content = message.getContent();

        if (StringUtils::startsWith(content, "{"))
        {
            json j = json::parse(content);

            return std::unique_ptr<FirmwareUpdateCommand>(new FirmwareUpdateCommand(j.get<FirmwareUpdateCommand>()));
        }
        else
        {
            FirmwareUpdateCommand::Type type;
            if (content == "INSTALL")
            {
                type = FirmwareUpdateCommand::Type::INSTALL;
            }
            else if (content == "ABORT")
            {
                type = FirmwareUpdateCommand::Type::ABORT;
            }
            else if (content == "FILE_UPLOAD")
            {
                type = FirmwareUpdateCommand::Type::FILE_UPLOAD;
            }
            else if (content == "URL_DOWNLOAD")
            {
                type = FirmwareUpdateCommand::Type::URL_DOWNLOAD;
            }
            else
            {
                type = FirmwareUpdateCommand::Type::UNKNOWN;
            }

            return std::unique_ptr<FirmwareUpdateCommand>(new FirmwareUpdateCommand(type));
        }
    }
    catch (...)
    {
        return nullptr;
    }
}

std::string DFUProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    // remove / from end of channel
    const std::string channel =
      StringUtils::endsWith(topic, CHANNEL_DELIMITER) ? StringUtils::removeSufix(topic, CHANNEL_DELIMITER) : topic;

    const auto position = channel.find_last_of(CHANNEL_DELIMITER);

    if (position != std::string::npos)
    {
        return channel.substr(position + CHANNEL_DELIMITER.size(), std::string::npos);
    }

    return "";
}
}
