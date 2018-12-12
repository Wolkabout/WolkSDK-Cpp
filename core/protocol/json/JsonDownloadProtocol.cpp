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

#include "protocol/json/JsonDownloadProtocol.h"
#include "model/BinaryData.h"
#include "model/FilePacketRequest.h"
#include "model/Message.h"
#include "utilities/json.hpp"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"

#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonDownloadProtocol::NAME = "FileDownloadProtocol";

const std::string JsonDownloadProtocol::CHANNEL_DELIMITER = "/";
const std::string JsonDownloadProtocol::CHANNEL_MULTI_LEVEL_WILDCARD = "#";
const std::string JsonDownloadProtocol::CHANNEL_SINGLE_LEVEL_WILDCARD = "+";

const std::string JsonDownloadProtocol::GATEWAY_PATH_PREFIX = "g/";
const std::string JsonDownloadProtocol::DEVICE_PATH_PREFIX = "d/";
const std::string JsonDownloadProtocol::DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string JsonDownloadProtocol::PLATFORM_TO_DEVICE_DIRECTION = "p2d/";

const std::string JsonDownloadProtocol::FILE_HANDLING_STATUS_TOPIC_ROOT = "service/status/file/";

const std::string JsonDownloadProtocol::BINARY_TOPIC_ROOT = "service/binary/";

const std::vector<std::string> JsonDownloadProtocol::INBOUND_CHANNELS = {BINARY_TOPIC_ROOT};

/*** FILE PACKET_REQUEST ***/
static void to_json(json& j, const FilePacketRequest& p)
{
    j = json{{"fileName", p.getFileName()}, {"chunkIndex", p.getChunkIndex()}, {"chunkSize", p.getChunkSize()}};
}

static void to_json(json& j, const std::shared_ptr<FilePacketRequest>& p)
{
    to_json(j, *p);
}
/*** FILE PACKET_REQUEST ***/

const std::string& JsonDownloadProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> JsonDownloadProtocol::getInboundChannels() const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [](const std::string& source) { return source + DEVICE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD; });
    return channels;
}

std::vector<std::string> JsonDownloadProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + DEVICE_PATH_PREFIX + deviceKey; });
    return channels;
}

bool JsonDownloadProtocol::isBinary(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), BINARY_TOPIC_ROOT);
}

std::unique_ptr<BinaryData> JsonDownloadProtocol::makeBinaryData(const Message& message) const
{
    try
    {
        return std::unique_ptr<BinaryData>(new BinaryData(ByteUtils::toByteArray(message.getContent())));
    }
    catch (const std::invalid_argument&)
    {
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDownloadProtocol::makeMessage(const std::string& deviceKey,
                                                       const FilePacketRequest& filePacketRequest) const
{
    const json jPayload(filePacketRequest);
    const std::string payload = jPayload.dump();
    const std::string topic = FILE_HANDLING_STATUS_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::string JsonDownloadProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
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
}
