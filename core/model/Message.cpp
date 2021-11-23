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

#include "core/model/Message.h"
#include "core/utilities/ByteUtils.h"

#include <string>
#include <utility>

namespace wolkabout
{
Message::Message(std::string content, std::string channel)
: m_content(std::move(content)), m_channel(std::move(channel))
{
}

const std::string& Message::getContent() const
{
    return m_content;
}

const std::string& Message::getChannel() const
{
    return m_channel;
}

FeedValuesMessage::FeedValuesMessage(const std::vector<Reading>& readings)
{
    for (const auto& reading : readings)
    {
        m_readings[reading.getTimestamp()].emplace_back(reading);
    }
}

FeedValuesMessage::FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings)
{
    for (const auto& reading : readings)
    {
        m_readings[reading->getTimestamp()].emplace_back(*reading);
    }
}

MessageType FeedValuesMessage::getMessageType()
{
    return MessageType::FEED_VALUES;
}

const std::map<std::uint64_t, std::vector<Reading>>& FeedValuesMessage::getReadings() const
{
    return m_readings;
}

MessageType PullFeedValuesMessage::getMessageType()
{
    return MessageType::PULL_FEED_VALUES;
}

FeedRegistrationMessage::FeedRegistrationMessage(std::vector<Feed> feeds) : m_feeds(std::move(feeds)) {}

const std::vector<Feed>& FeedRegistrationMessage::getFeeds() const
{
    return m_feeds;
}

MessageType FeedRegistrationMessage::getMessageType()
{
    return MessageType::FEED_REGISTRATION;
}

FeedRemovalMessage::FeedRemovalMessage(std::vector<std::string> feedRefsToRemove)
: m_references(std::move(feedRefsToRemove))
{
}

MessageType FeedRemovalMessage::getMessageType()
{
    return MessageType::FEED_REMOVAL;
}

const std::vector<std::string>& FeedRemovalMessage::getReferences() const
{
    return m_references;
}

AttributeRegistrationMessage::AttributeRegistrationMessage(std::vector<Attribute> attributes)
: m_attributes(std::move(attributes))
{
}

AttributeRegistrationMessage::AttributeRegistrationMessage(const std::vector<std::shared_ptr<Attribute>>& attributes)
{
    for (const auto& attribute : attributes)
    {
        m_attributes.push_back(*attribute);
    }
}

MessageType AttributeRegistrationMessage::getMessageType()
{
    return MessageType::ATTRIBUTE_REGISTRATION;
}

const std::vector<Attribute>& AttributeRegistrationMessage::getAttributes() const
{
    return m_attributes;
}

ParametersUpdateMessage::ParametersUpdateMessage(std::vector<Parameters> parameterList)
: m_parameterList(std::move(parameterList))
{
}

MessageType ParametersUpdateMessage::getMessageType()
{
    return MessageType::PARAMETER_SYNC;
}

const std::vector<Parameters>& ParametersUpdateMessage::getParameters() const
{
    return m_parameterList;
}

MessageType ParametersPullMessage::getMessageType()
{
    return MessageType::PULL_PARAMETERS;
}

FileUploadInitiateMessage::FileUploadInitiateMessage(std::string name, std::uint64_t size, std::string hash)
: m_name(std::move(name)), m_size(size), m_hash(std::move(hash))
{
}

const std::string& FileUploadInitiateMessage::getName() const
{
    return m_name;
}

uint64_t FileUploadInitiateMessage::getSize() const
{
    return m_size;
}

const std::string& FileUploadInitiateMessage::getHash() const
{
    return m_hash;
}

MessageType FileUploadInitiateMessage::getMessageType()
{
    return MessageType::FILE_URL_DOWNLOAD_INIT;
}

FileUploadStatusMessage::FileUploadStatusMessage(std::string name, FileUploadStatus status, FileUploadError error)
: m_name(std::move(name)), m_status(status), m_error(error)
{
}

const std::string& FileUploadStatusMessage::getName() const
{
    return m_name;
}

FileUploadStatus FileUploadStatusMessage::getStatus() const
{
    return m_status;
}

FileUploadError FileUploadStatusMessage::getError() const
{
    return m_error;
}

MessageType FileUploadStatusMessage::getMessageType()
{
    return MessageType::FILE_UPLOAD_STATUS;
}

FileUploadAbortMessage::FileUploadAbortMessage(std::string name) : m_name(std::move(name)) {}

const std::string& FileUploadAbortMessage::getName() const
{
    return m_name;
}

MessageType FileUploadAbortMessage::getMessageType()
{
    return MessageType::FILE_UPLOAD_ABORT;
}

FileBinaryRequestMessage::FileBinaryRequestMessage(std::string name, std::uint64_t chunkIndex)
: m_name(std::move(name)), m_chunkIndex(chunkIndex)
{
}

const std::string& FileBinaryRequestMessage::getName() const
{
    return m_name;
}

std::uint64_t FileBinaryRequestMessage::getChunkIndex() const
{
    return m_chunkIndex;
}

MessageType FileBinaryRequestMessage::getMessageType()
{
    return MessageType::FILE_BINARY_REQUEST;
}

FileBinaryResponseMessage::FileBinaryResponseMessage(const std::string& payload)
{
    if (payload.length() > 64)
    {
        m_previousHash = payload.substr(0, 64);
        m_data = ByteUtils::toByteArray(payload.substr(64, payload.length() - 128));
        m_currentHash = payload.substr(payload.length() - 64);
    }
}

const std::string& FileBinaryResponseMessage::getPreviousHash() const
{
    return m_previousHash;
}

const std::vector<std::uint8_t>& FileBinaryResponseMessage::getData() const
{
    return m_data;
}

const std::string& FileBinaryResponseMessage::getCurrentHash() const
{
    return m_currentHash;
}

MessageType FileBinaryResponseMessage::getMessageType()
{
    return MessageType::FILE_BINARY_RESPONSE;
}

FileUrlDownloadInitMessage::FileUrlDownloadInitMessage(std::string path) : m_path(std::move(path)) {}

const std::string& FileUrlDownloadInitMessage::getPath() const
{
    return m_path;
}

MessageType FileUrlDownloadInitMessage::getMessageType()
{
    return MessageType::FILE_URL_DOWNLOAD_INIT;
}

FileUrlDownloadAbortMessage::FileUrlDownloadAbortMessage(std::string path) : m_path(std::move(path)) {}

const std::string& FileUrlDownloadAbortMessage::getPath() const
{
    return m_path;
}

MessageType FileUrlDownloadAbortMessage::getMessageType()
{
    return MessageType::FILE_URL_DOWNLOAD_ABORT;
}

FileUrlDownloadStatusMessage::FileUrlDownloadStatusMessage(std::string fileUrl, std::string fileName,
                                                           FileUploadStatus status, FileUploadError error)
: m_fileUrl(std::move(fileUrl)), m_fileName(std::move(fileName)), m_status(status), m_error(error)
{
}

const std::string& FileUrlDownloadStatusMessage::getFileUrl() const
{
    return m_fileUrl;
}

const std::string& FileUrlDownloadStatusMessage::getFileName() const
{
    return m_fileName;
}

FileUploadStatus FileUrlDownloadStatusMessage::getStatus() const
{
    return m_status;
}

FileUploadError FileUrlDownloadStatusMessage::getError() const
{
    return m_error;
}

MessageType FileUrlDownloadStatusMessage::getMessageType()
{
    return MessageType::FILE_URL_DOWNLOAD_STATUS;
}

MessageType FileListRequestMessage::getMessageType()
{
    return MessageType::FILE_LIST_REQUEST;
}

FileListResponseMessage::FileListResponseMessage(std::vector<FileInformation> files) : m_files(std::move(files)) {}

const std::vector<FileInformation>& FileListResponseMessage::getFiles() const
{
    return m_files;
}

MessageType FileListResponseMessage::getMessageType()
{
    return MessageType::FILE_LIST_RESPONSE;
}

FileDeleteMessage::FileDeleteMessage(std::vector<std::string> files) : m_files(std::move(files)) {}

const std::vector<std::string>& FileDeleteMessage::getFiles() const
{
    return m_files;
}

MessageType FileDeleteMessage::getMessageType()
{
    return MessageType::FILE_DELETE;
}

MessageType FilePurgeMessage::getMessageType()
{
    return MessageType::FILE_PURGE;
}

FirmwareUpdateInstallMessage::FirmwareUpdateInstallMessage(std::string file) : m_file(std::move(file)) {}

const std::string& FirmwareUpdateInstallMessage::getFile() const
{
    return m_file;
}

MessageType FirmwareUpdateInstallMessage::getMessageType()
{
    return MessageType::FIRMWARE_UPDATE_INSTALL;
}

FirmwareUpdateStatusMessage::FirmwareUpdateStatusMessage(FirmwareUpdateStatus status, FirmwareUpdateError error)
: m_status(status), m_error(error)
{
}

FirmwareUpdateStatus FirmwareUpdateStatusMessage::getStatus() const
{
    return m_status;
}

FirmwareUpdateError FirmwareUpdateStatusMessage::getError() const
{
    return m_error;
}

MessageType FirmwareUpdateStatusMessage::getMessageType()
{
    return MessageType::FIRMWARE_UPDATE_STATUS;
}

MessageType FirmwareUpdateAbortMessage::getMessageType()
{
    return MessageType::FIRMWARE_UPDATE_ABORT;
}
}    // namespace wolkabout
