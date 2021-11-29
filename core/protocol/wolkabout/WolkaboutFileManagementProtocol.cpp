/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

#include "core/protocol/wolkabout/WolkaboutFileManagementProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

using nlohmann::json;

namespace wolkabout
{
std::vector<std::string> WolkaboutFileManagementProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutFileManagementProtocol::getInboundChannelsForDevice(
  const std::string& deviceKey) const
{
    return {PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_UPLOAD_INIT),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_UPLOAD_ABORT),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_BINARY_RESPONSE),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_URL_DOWNLOAD_INIT),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_URL_DOWNLOAD_ABORT),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_LIST_REQUEST),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_DELETE),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FILE_PURGE)};
}

std::string WolkaboutFileManagementProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::extractDeviceKeyFromChannel(topic);
}

MessageType WolkaboutFileManagementProtocol::getMessageType(std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getMessageType(message);
}

std::unique_ptr<Message> WolkaboutFileManagementProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                              const FileUploadStatusMessage& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'FileUploadStatus' message";

    // Verify that the message is valid
    if (message.getName().empty())
    {
        LOG(ERROR) << errorPrefix << " -> The name is empty.";
        return nullptr;
    }
    auto statusString = toString(message.getStatus());
    if (statusString.empty())
    {
        LOG(ERROR) << errorPrefix << " -> The status value is not valid.";
        return nullptr;
    }
    auto errorString = toString(message.getError());
    if (message.getStatus() == FileUploadStatus::ERROR && errorString.empty())
    {
        LOG(ERROR) << errorPrefix << " -> Missing valid error value.";
        return nullptr;
    }

    // Make the topic for the message
    const auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                       toString(MessageType::FILE_UPLOAD_STATUS);

    // Parse the message into a JSON
    auto payload = json{{"name", message.getName()}, {"status", statusString}};
    if (message.getStatus() == FileUploadStatus::ERROR)
        payload["error"] = errorString;
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<Message> WolkaboutFileManagementProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                              const FileBinaryRequestMessage& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'FileBinaryRequest' message";

    // Verify that the message is valid
    if (message.getName().empty())
    {
        LOG(ERROR) << errorPrefix << " -> The name is empty.";
        return nullptr;
    }

    // Make the topic for the message
    const auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                       toString(MessageType::FILE_BINARY_REQUEST);

    // Parse the message into a JSON
    auto payload = json{{"name", message.getName()}, {"chunkIndex", message.getChunkIndex()}};
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<Message> WolkaboutFileManagementProtocol::makeOutboundMessage(
  const std::string& deviceKey, const FileUrlDownloadStatusMessage& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'FileUrlDownloadStatus' message";

    // Verify that the message is valid
    if (message.getFileName().empty())
    {
        LOG(ERROR) << errorPrefix << " -> The file name is empty.";
        return nullptr;
    }
    if (message.getFileUrl().empty())
    {
        LOG(ERROR) << errorPrefix << " -> The file url is empty.";
        return nullptr;
    }
    const auto statusString = toString(message.getStatus());
    if (statusString.empty())
    {
        LOG(ERROR) << errorPrefix << " -> The status value is not valid.";
        return nullptr;
    }
    const auto errorString = toString(message.getError());
    if (message.getStatus() == FileUploadStatus::ERROR && errorString.empty())
    {
        LOG(ERROR) << errorPrefix << " -> Missing valid error value.";
        return nullptr;
    }

    // Make the topic for the message
    const auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                       toString(MessageType::FILE_URL_DOWNLOAD_STATUS);

    // Create the JSON payload
    auto payload =
      json{{"fileName", message.getFileName()}, {"fileUrl", message.getFileUrl()}, {"status", statusString}};
    if (message.getStatus() == FileUploadStatus::ERROR)
        payload["error"] = errorString;
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<Message> WolkaboutFileManagementProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                              const FileListResponseMessage& message)
{
    LOG(TRACE) << METHOD_INFO;

    // Make the topic for the message
    const auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                       toString(MessageType::FILE_LIST_REQUEST);

    // Create the JSON payload
    auto payload = json::array();
    for (const auto& file : message.getFiles())
        payload.push_back(json{{"name", file.name}, {"size", file.size}, {"hash", file.hash}});
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<FileUploadInitiateMessage> WolkaboutFileManagementProtocol::parseFileUploadInit(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileUploadInitiate' message";

    // Check that the message is a FileUploadInitiate message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_UPLOAD_INIT)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileUploadInitiate' message.";
        return nullptr;
    }

    // Load the JSON contents of the message
    auto payload = json::parse(message->getContent());

    // Check that the payload is an object, and contains required fields
    if (!payload.is_object())
    {
        LOG(ERROR) << errorPrefix << " -> The payload is not a valid JSON object.";
        return nullptr;
    }

    // Required fields
    auto nameIt = payload.find("name");
    if (nameIt == payload.end() || !nameIt->is_string())
    {
        LOG(ERROR) << errorPrefix << " -> The payload is missing a valid 'name' value.";
        return nullptr;
    }
    auto sizeIt = payload.find("size");
    if (sizeIt == payload.end() || !sizeIt->is_number_unsigned())
    {
        LOG(ERROR) << errorPrefix << " -> The payload is missing a valid 'size' value.";
        return nullptr;
    }
    auto hashIt = payload.find("hash");
    if (hashIt == payload.end() || !hashIt->is_string())
    {
        LOG(ERROR) << errorPrefix << " -> The payload is missing a valid 'hash' value.";
        return nullptr;
    }

    // Make the message
    return std::unique_ptr<FileUploadInitiateMessage>(new FileUploadInitiateMessage(
      nameIt->get<std::string>(), sizeIt->get<std::uint64_t>(), hashIt->get<std::string>()));
}

std::unique_ptr<FileUploadAbortMessage> WolkaboutFileManagementProtocol::parseFileUploadAbort(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileUploadAbort' message";

    // Check that the message is a FileUploadInitiate message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_UPLOAD_ABORT)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileUploadAbort' message.";
        return nullptr;
    }

    // Take the payload and return it as the file name
    return std::unique_ptr<FileUploadAbortMessage>(new FileUploadAbortMessage(message->getContent()));
}

std::unique_ptr<FileBinaryResponseMessage> WolkaboutFileManagementProtocol::parseFileBinaryResponse(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileBinaryResponse' message";

    // Check that the message is a FileBinaryResponse message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_BINARY_RESPONSE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileBinaryResponse' message.";
        return nullptr;
    }

    // Return the entire payload in the response
    return std::unique_ptr<FileBinaryResponseMessage>(new FileBinaryResponseMessage(message->getContent()));
}
std::unique_ptr<FileUrlDownloadInitMessage> WolkaboutFileManagementProtocol::parseFileUrlDownloadInit(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileUrlDownloadInit' message";

    // Check that the message is a FileUrlDownloadInit message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_URL_DOWNLOAD_INIT)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileUrlDownloadInit' message.";
        return nullptr;
    }

    // Take the payload as the file path
    return std::unique_ptr<FileUrlDownloadInitMessage>(new FileUrlDownloadInitMessage(message->getContent()));
}

std::unique_ptr<FileUrlDownloadAbortMessage> WolkaboutFileManagementProtocol::parseFileUrlDownloadAbort(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileUrlDownloadAbort' message";

    // Check that the message is a FileUrlDownloadAbort message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_URL_DOWNLOAD_ABORT)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileUrlDownloadAbort' message.";
        return nullptr;
    }

    // Take the payload as the file path
    return std::unique_ptr<FileUrlDownloadAbortMessage>(new FileUrlDownloadAbortMessage(message->getContent()));
}

std::unique_ptr<FileListRequestMessage> WolkaboutFileManagementProtocol::parseFileListRequest(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileListRequest' message";

    // Check that the message is a FileListRequest message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_LIST_REQUEST)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileListRequest' message.";
        return nullptr;
    }

    // Take the payload as the file path
    return std::unique_ptr<FileListRequestMessage>(new FileListRequestMessage);
}

std::unique_ptr<FileDeleteMessage> WolkaboutFileManagementProtocol::parseFileDelete(std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FileDelete' message";

    // Check that the message is a FileDelete message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_DELETE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FileDelete' message.";
        return nullptr;
    }

    // Load the JSON contents of the message
    auto payload = json::parse(message->getContent());

    // Check that the payload is an array of strings
    if (!payload.is_array())
    {
        LOG(ERROR) << errorPrefix << " -> The payload is not a valid JSON array.";
        return nullptr;
    }

    // Start checking every value of the array
    auto files = std::vector<std::string>{};
    for (const auto& file : payload.items())
    {
        // Check the type of the value
        if (!file.value().is_string())
        {
            LOG(ERROR) << errorPrefix << " -> The payload contains array values that are not strings.";
            return nullptr;
        }

        // Place the file name into the vector
        files.emplace_back(file.value().get<std::string>());
    }

    // Make the message
    return std::unique_ptr<FileDeleteMessage>(new FileDeleteMessage(files));
}

std::unique_ptr<FilePurgeMessage> WolkaboutFileManagementProtocol::parseFilePurge(std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FilePurge' message";

    // Check that the message is a FilePurge message.
    auto type = getMessageType(message);
    if (type != MessageType::FILE_PURGE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FilePurge' message.";
        return nullptr;
    }

    // Take the payload as the file path
    return std::unique_ptr<FilePurgeMessage>(new FilePurgeMessage);
}
}    // namespace wolkabout
