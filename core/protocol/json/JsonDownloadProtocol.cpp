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

#include "protocol/json/JsonDownloadProtocol.h"
#include "model/BinaryData.h"
#include "model/FileDelete.h"
#include "model/FileList.h"
#include "model/FilePacketRequest.h"
#include "model/FileUploadAbort.h"
#include "model/FileUploadInitiate.h"
#include "model/FileUploadStatus.h"
#include "model/Message.h"
#include "protocol/json/Json.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonDownloadProtocol::FILE_UPLOAD_INITIATE_TOPIC_ROOT = "p2d/file_upload_initiate/";
const std::string JsonDownloadProtocol::FILE_UPLOAD_ABORT_TOPIC_ROOT = "p2d/file_upload_abort/";
const std::string JsonDownloadProtocol::FILE_UPLOAD_STATUS_TOPIC_ROOT = "d2p/file_upload_status/";

const std::string JsonDownloadProtocol::BINARY_REQUEST_TOPIC_ROOT = "d2p/file_binary_request/";
const std::string JsonDownloadProtocol::BINARY_RESPONSE_TOPIC_ROOT = "p2d/file_binary_response/";

const std::string JsonDownloadProtocol::FILE_DELETE_TOPIC_ROOT = "p2d/file_delete/";
const std::string JsonDownloadProtocol::FILE_PURGE_TOPIC_ROOT = "p2d/file_purge/";

const std::string JsonDownloadProtocol::FILE_LIST_REQUEST_TOPIC_ROOT = "p2d/file_list_request/";
const std::string JsonDownloadProtocol::FILE_LIST_RESPONSE_TOPIC_ROOT = "d2p/file_list_response/";
const std::string JsonDownloadProtocol::FILE_LIST_UPDATE_TOPIC_ROOT = "d2p/file_list_update/";
const std::string JsonDownloadProtocol::FILE_LIST_CONFIRM_TOPIC_ROOT = "p2d/file_list_confirm/";

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

/*** FILE LIST ***/
static void to_json(json& j, const FileList& list)
{
    j = json::array();

    for (const auto& fileName : list.getFileNames())
    {
        json innerJ;
        innerJ["fileName"] = fileName;
        j.push_back(innerJ);
    }
}
/*** FILE LIST ***/

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

/*** FILE DELETE ***/
static FileDelete file_delete_from_json(const json& j)
{
    const std::string name = [&]() -> std::string {
        if (j.find("fileName") != j.end())
        {
            return j.at("fileName").get<std::string>();
        }

        return "";
    }();

    return FileDelete(name);
}
/*** FILE DELETE ***/

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
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), BINARY_RESPONSE_TOPIC_ROOT);
}

bool JsonDownloadProtocol::isUploadInitiate(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), FILE_UPLOAD_INITIATE_TOPIC_ROOT);
}

bool JsonDownloadProtocol::isUploadAbort(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), FILE_UPLOAD_ABORT_TOPIC_ROOT);
}

std::unique_ptr<BinaryData> JsonDownloadProtocol::makeBinaryData(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), BINARY_RESPONSE_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        return std::unique_ptr<BinaryData>(new BinaryData(ByteUtils::toByteArray(message.getContent())));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize binary data: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize binary data";
        return nullptr;
    }
}

std::unique_ptr<FileUploadInitiate> JsonDownloadProtocol::makeFileUploadInitiate(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), FILE_UPLOAD_INITIATE_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FileUploadInitiate>(new FileUploadInitiate(file_upload_initiate_from_json(j)));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file upload initiate: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file upload initiate";
        return nullptr;
    }
}

std::unique_ptr<FileUploadAbort> JsonDownloadProtocol::makeFileUploadAbort(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), FILE_UPLOAD_ABORT_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FileUploadAbort>(new FileUploadAbort(file_upload_abort_from_json(j)));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file upload abort: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file upload abort";
        return nullptr;
    }
}

std::unique_ptr<FileDelete> JsonDownloadProtocol::makeFileDelete(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), FILE_DELETE_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        return std::unique_ptr<FileDelete>(new FileDelete(file_delete_from_json(j)));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file delete command: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file delete command";
        return nullptr;
    }
}

bool JsonDownloadProtocol::isFilePurge(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), FILE_PURGE_TOPIC_ROOT);
}

bool JsonDownloadProtocol::isFileListRequest(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    return StringUtils::startsWith(message.getChannel(), FILE_LIST_REQUEST_TOPIC_ROOT);
}

std::unique_ptr<PlatformResult> JsonDownloadProtocol::makeFileListConfirm(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;

    if (!StringUtils::startsWith(message.getChannel(), FILE_LIST_CONFIRM_TOPIC_ROOT))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        PlatformResult result = j;

        return std::unique_ptr<PlatformResult>(new PlatformResult(result));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file list confirm: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to deserialize file list confirm";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDownloadProtocol::makeMessage(const std::string& deviceKey,
                                                           const FilePacketRequest& filePacketRequest) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const std::string topic = BINARY_REQUEST_TOPIC_ROOT + m_devicePrefix + deviceKey;

        const json jPayload(filePacketRequest);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file packet request: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file packet request";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDownloadProtocol::makeMessage(const std::string& deviceKey,
                                                           const FileUploadStatus& fileUploadStatus) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const std::string topic = FILE_UPLOAD_STATUS_TOPIC_ROOT + m_devicePrefix + deviceKey;

        const json jPayload(fileUploadStatus);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file upload status: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file upload status";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDownloadProtocol::makeFileListUpdateMessage(const std::string& deviceKey,
                                                                         const wolkabout::FileList& fileList) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const std::string topic = FILE_LIST_UPDATE_TOPIC_ROOT + m_devicePrefix + deviceKey;

        const json jPayload(fileList);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file list update: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file list update";
        return nullptr;
    }
}

std::unique_ptr<Message> JsonDownloadProtocol::makeFileListResponseMessage(const std::string& deviceKey,
                                                                           const wolkabout::FileList& fileList) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const std::string topic = FILE_LIST_RESPONSE_TOPIC_ROOT + m_devicePrefix + deviceKey;

        const json jPayload(fileList);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }
    catch (std::exception& e)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file list response: " << e.what();
        return nullptr;
    }
    catch (...)
    {
        LOG(DEBUG) << "File download protocol: Unable to serialize file list response";
        return nullptr;
    }
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
