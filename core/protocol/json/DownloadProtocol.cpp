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

#include "protocol/json/DownloadProtocol.h"
#include "model/BinaryData.h"
#include "model/FilePacketRequest.h"
#include "model/Message.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string DownloadProtocol::NAME = "FileDownloadProtocol";

const std::string DownloadProtocol::CHANNEL_DELIMITER = "/";
const std::string DownloadProtocol::CHANNEL_WILDCARD = "#";

const std::string DownloadProtocol::FILE_HANDLING_STATUS_TOPIC_ROOT = "service/status/file/";

const std::string DownloadProtocol::BINARY_TOPIC_ROOT = "service/binary/";

const std::vector<std::string> DownloadProtocol::INBOUND_CHANNELS = {BINARY_TOPIC_ROOT};

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

const std::string& DownloadProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> DownloadProtocol::getInboundChannels() const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [](const std::string& source) { return source + CHANNEL_WILDCARD; });
    return channels;
}

std::vector<std::string> DownloadProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + deviceKey; });
    return channels;
}

bool DownloadProtocol::isBinary(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), BINARY_TOPIC_ROOT);
}

std::unique_ptr<BinaryData> DownloadProtocol::makeBinaryData(const Message& message) const
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

std::unique_ptr<Message> DownloadProtocol::makeMessage(const std::string& deviceKey,
                                                       const FilePacketRequest& filePacketRequest) const
{
    const json jPayload(filePacketRequest);
    const std::string payload = jPayload.dump();
    const std::string topic = FILE_HANDLING_STATUS_TOPIC_ROOT + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::string DownloadProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
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
}    // namespace wolkabout
