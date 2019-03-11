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
#include "model/FileUploadAbort.h"
#include "model/FileUploadInitiate.h"
#include "model/FileUploadStatus.h"
#include "model/Message.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

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

const std::string JsonDownloadProtocol::FILE_UPLOAD_INITIATE_TOPIC_ROOT = "p2d/file_upload_initiate/";
const std::string JsonDownloadProtocol::FILE_UPLOAD_ABORT_TOPIC_ROOT = "p2d/file_upload_abort/";
const std::string JsonDownloadProtocol::FILE_UPLOAD_STATUS_TOPIC_ROOT = "d2p/file_upload_status/";

const std::string JsonDownloadProtocol::BINARY_REQUEST_TOPIC_ROOT = "d2p/file_binary_request/";
const std::string JsonDownloadProtocol::BINARY_RESPONSE_TOPIC_ROOT = "p2d/file_binary_response/";

const std::vector<std::string> JsonDownloadProtocol::INBOUND_CHANNELS = {
  FILE_UPLOAD_INITIATE_TOPIC_ROOT, FILE_UPLOAD_ABORT_TOPIC_ROOT, BINARY_RESPONSE_TOPIC_ROOT};

/*** FILE PACKET_REQUEST ***/
static void to_json(json& j, const FilePacketRequest& p)
{
    j = json{{"fileName", p.getFileName()}, {"chunkIndex", p.getChunkIndex()}, {"chunkSize", p.getChunkSize()}};
}
/*** FILE PACKET_REQUEST ***/

/*** FILE UPLOAD STATUS ***/
static void to_json(json& j, const FileUploadStatus& p)
{
    const std::string status = [&]() -> std::string {
        switch (p.getStatus())
        {
        case FileTransferStatus::FILE_TRANSFER:
            return "FILE_TRANSFER";
        case FileTransferStatus::FILE_READY:
            return "FILE_READY";
        case FileTransferStatus::ABORTED:
            return "ABORTED";
        case FileTransferStatus::ERROR:
            return "ERROR";
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
}
/*** FILE UPLOAD STATUS ***/

/*** FILE UPLOAD INITIATE ***/
static FileUploadInitiate file_upload_initiate_from_json(const json& j)
{
    const std::string name = [&]() -> std::string {
        if (j.find("fileName") != j.end())
        {
            return j.at("fileName").get<std::string>();
        }

        return "";
    }();

    const std::uint64_t size = [&]() -> std::uint64_t {
        if (j.find("fileSize") != j.end())
        {
            return j.at("fileSize").get<std::uint64_t>();
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

    return FileUploadInitiate(name, size, hash);
}
/*** FILE UPLOAD INITIATE ***/

/*** FILE UPLOAD ABORT ***/
static FileUploadAbort file_upload_abort_from_json(const json& j)
{
    const std::string name = [&]() -> std::string {
        if (j.find("fileName") != j.end())
        {
            return j.at("fileName").get<std::string>();
        }

        return "";
    }();

    return FileUploadAbort(name);
}
/*** FILE UPLOAD ABORT ***/

JsonDownloadProtocol::JsonDownloadProtocol(bool isGateway)
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

const std::string& JsonDownloadProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> JsonDownloadProtocol::getInboundChannels() const
{
    static std::vector<std::string> channels = [&] {
        std::vector<std::string> tempChannels;
        std::transform(
          INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(tempChannels),
          [&](const std::string& source) { return source + m_devicePrefix + CHANNEL_MULTI_LEVEL_WILDCARD; });

        return tempChannels;
    }();

    return channels;
}

std::vector<std::string> JsonDownloadProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + m_devicePrefix + deviceKey; });
    return channels;
}

bool JsonDownloadProtocol::isBinary(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), BINARY_RESPONSE_TOPIC_ROOT);
}

bool JsonDownloadProtocol::isUploadInitiate(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), FILE_UPLOAD_INITIATE_TOPIC_ROOT);
}

bool JsonDownloadProtocol::isUploadAbort(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), FILE_UPLOAD_ABORT_TOPIC_ROOT);
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

std::unique_ptr<FileUploadInitiate> JsonDownloadProtocol::makeFileUploadInitiate(const Message& message) const
{
    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FileUploadInitiate>(new FileUploadInitiate(file_upload_initiate_from_json(j)));
    }
    catch (...)
    {
        return nullptr;
    }
}

std::unique_ptr<FileUploadAbort> JsonDownloadProtocol::makeFileUploadAbort(const Message& message) const
{
    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FileUploadAbort>(new FileUploadAbort(file_upload_abort_from_json(j)));
    }
    catch (...)
    {
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDownloadProtocol::makeMessage(const std::string& deviceKey,
                                                           const FilePacketRequest& filePacketRequest) const
{
    const json jPayload(filePacketRequest);
    const std::string payload = jPayload.dump();
    const std::string topic = BINARY_REQUEST_TOPIC_ROOT + m_devicePrefix + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> JsonDownloadProtocol::makeMessage(const std::string& deviceKey,
                                                           const FileUploadStatus& fileUploadStatus) const
{
    const json jPayload(fileUploadStatus);
    const std::string payload = jPayload.dump();
    const std::string topic = FILE_UPLOAD_STATUS_TOPIC_ROOT + m_devicePrefix + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::string JsonDownloadProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string devicePathPrefix = CHANNEL_DELIMITER + m_devicePrefix;

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
