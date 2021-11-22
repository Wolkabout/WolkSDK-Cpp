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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Attribute.h"
#include "Feed.h"
#include "core/Types.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class Message
{
public:
    Message(std::string content, std::string channel);

    virtual ~Message() = default;

    const std::string& getContent() const;

    const std::string& getChannel() const;

private:
    std::string m_content;
    std::string m_channel;
};

class MessageObject
{
public:
    virtual ~MessageObject() = default;

    virtual MessageType getMessageType() = 0;
};

class FeedValuesMessage : public MessageObject
{
public:
    explicit FeedValuesMessage(const std::vector<Reading>& readings);

    explicit FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings);

    MessageType getMessageType() override;

    const std::map<std::uint64_t, std::vector<Reading>>& getReadings() const;

private:
    std::map<std::uint64_t, std::vector<Reading>> m_readings;
};

class PullFeedValuesMessage : public MessageObject
{
public:
    MessageType getMessageType() override;
};

class FeedRegistrationMessage : public MessageObject
{
public:
    explicit FeedRegistrationMessage(std::vector<Feed> feeds);

    MessageType getMessageType() override;

    const std::vector<Feed>& getFeeds() const;

private:
    std::vector<Feed> m_feeds;
};

class FeedRemovalMessage : public MessageObject
{
public:
    explicit FeedRemovalMessage(std::vector<std::string> feedRefsToRemove);

    MessageType getMessageType() override;

    const std::vector<std::string>& getReferences() const;

private:
    std::vector<std::string> m_references;
};

class AttributeRegistrationMessage : public MessageObject
{
public:
    explicit AttributeRegistrationMessage(std::vector<Attribute> attributes);

    explicit AttributeRegistrationMessage(const std::vector<std::shared_ptr<Attribute>>& attributes);

    MessageType getMessageType() override;

    const std::vector<Attribute>& getAttributes() const;

private:
    std::vector<Attribute> m_attributes;
};

class ParametersUpdateMessage : public MessageObject
{
public:
    explicit ParametersUpdateMessage(std::vector<Parameters> parameterList);

    MessageType getMessageType() override;

    const std::vector<Parameters>& getParameters() const;

private:
    std::vector<Parameters> m_parameterList;
};

class ParametersPullMessage : public MessageObject
{
public:
    MessageType getMessageType() override;
};

class FileUploadInitiateMessage : public MessageObject
{
public:
    explicit FileUploadInitiateMessage(std::string name, std::uint64_t size, std::string hash);

    const std::string& getName() const;

    std::uint64_t getSize() const;

    const std::string& getHash() const;

    MessageType getMessageType() override;

private:
    std::string m_name;
    std::uint64_t m_size;
    std::string m_hash;
};

class FileUploadStatusMessage : public MessageObject
{
public:
    explicit FileUploadStatusMessage(std::string name, FileUploadStatus status,
                                     FileUploadError error = FileUploadError::NONE);

    const std::string& getName() const;

    FileUploadStatus getStatus() const;

    FileUploadError getError() const;

    MessageType getMessageType() override;

private:
    std::string m_name;
    FileUploadStatus m_status;
    FileUploadError m_error;
};

class FileUploadAbortMessage : public MessageObject
{
public:
    explicit FileUploadAbortMessage(std::string name);

    const std::string& getName() const;

    MessageType getMessageType() override;

private:
    std::string m_name;
};

class FileBinaryRequestMessage : public MessageObject
{
public:
    explicit FileBinaryRequestMessage(std::string name, std::uint64_t chunkIndex);

    const std::string& getName() const;

    std::uint64_t getChunkIndex() const;

    MessageType getMessageType() override;

private:
    std::string m_name;
    std::uint64_t m_chunkIndex;
};

class FileBinaryResponseMessage : public MessageObject
{
public:
    explicit FileBinaryResponseMessage(const std::string& payload);

    const std::string& getPreviousHash() const;

    const std::vector<std::uint8_t>& getData() const;

    const std::string& getCurrentHash() const;

    MessageType getMessageType() override;

private:
    std::string m_previousHash;
    std::vector<std::uint8_t> m_data;
    std::string m_currentHash;
};

class FileUrlDownloadInitMessage : public MessageObject
{
public:
    explicit FileUrlDownloadInitMessage(std::string path);

    const std::string& getPath() const;

    MessageType getMessageType() override;

private:
    std::string m_path;
};

class FileUrlDownloadAbortMessage : public MessageObject
{
public:
    explicit FileUrlDownloadAbortMessage(std::string path);

    const std::string& getPath() const;

    MessageType getMessageType() override;

private:
    std::string m_path;
};

class FileUrlDownloadStatusMessage : public MessageObject
{
public:
    explicit FileUrlDownloadStatusMessage(std::string fileUrl, std::string fileName, FileUploadStatus status,
                                          FileUploadError error = FileUploadError::NONE);

    const std::string& getFileUrl() const;

    const std::string& getFileName() const;

    FileUploadStatus getStatus() const;

    FileUploadError getError() const;

    MessageType getMessageType() override;

private:
    std::string m_fileUrl;
    std::string m_fileName;
    FileUploadStatus m_status;
    FileUploadError m_error;
};

class FileListRequestMessage : public MessageObject
{
public:
    MessageType getMessageType() override;
};

class FileListResponseMessage : public MessageObject
{
public:
    explicit FileListResponseMessage(std::vector<FileInformation> files);

    const std::vector<FileInformation>& getFiles() const;

    MessageType getMessageType() override;

private:
    std::vector<FileInformation> m_files;
};

class FileDeleteMessage : public MessageObject
{
public:
    explicit FileDeleteMessage(std::vector<std::string> files);

    const std::vector<std::string>& getFiles() const;

    MessageType getMessageType() override;

private:
    std::vector<std::string> m_files;
};

class FilePurgeMessage : public MessageObject
{
public:
    MessageType getMessageType() override;
};
}    // namespace wolkabout

#endif
