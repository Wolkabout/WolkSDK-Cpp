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

#ifndef JSONDOWNLOADPROTOCOL_H
#define JSONDOWNLOADPROTOCOL_H

#include "model/PlatformResult.h"
#include "protocol/Protocol.h"

#include <memory>

namespace wolkabout
{
class BinaryData;
class FileDelete;
class FileList;
class FilePacketRequest;
class FilePurge;
class FileUploadAbort;
class FileUploadInitiate;
class FileUploadStatus;
class FileUrlDownloadAbort;
class FileUrlDownloadInitiate;
class FileUrlDownloadStatus;
class Message;

class JsonDownloadProtocol : public Protocol
{
public:
    explicit JsonDownloadProtocol(bool isGateway = false);

    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;

    bool isBinary(const Message& message) const;

    bool isUploadInitiate(const Message& message) const;

    bool isUploadAbort(const Message& message) const;

    std::unique_ptr<FileUploadInitiate> makeFileUploadInitiate(const Message& message) const;

    std::unique_ptr<FileUploadAbort> makeFileUploadAbort(const Message& message) const;

    std::unique_ptr<FileDelete> makeFileDelete(const Message& message) const;

    bool isFilePurge(const Message& message) const;

    bool isFileListRequest(const Message& message) const;

    std::unique_ptr<PlatformResult> makeFileListConfirm(const Message& message) const;

    std::unique_ptr<BinaryData> makeBinaryData(const Message& message) const;

    std::unique_ptr<FileUrlDownloadInitiate> makeFileUrlDownloadInitiate(const Message& message) const;

    std::unique_ptr<FileUrlDownloadAbort> makeFileUrlDownloadAbort(const Message& message) const;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const FilePacketRequest& filePacketRequest) const;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey, const FileUploadStatus& fileUploadStatus) const;

    std::unique_ptr<Message> makeFileListUpdateMessage(const std::string& deviceKey, const FileList& fileList) const;

    std::unique_ptr<Message> makeFileListResponseMessage(const std::string& deviceKey, const FileList& fileList) const;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const FileUrlDownloadStatus& fileUrlDownloadStatus) const;

private:
    std::string m_devicePrefix;

    static const std::string FILE_UPLOAD_INITIATE_TOPIC_ROOT;
    static const std::string FILE_UPLOAD_ABORT_TOPIC_ROOT;
    static const std::string FILE_UPLOAD_STATUS_TOPIC_ROOT;

    static const std::string BINARY_REQUEST_TOPIC_ROOT;
    static const std::string BINARY_RESPONSE_TOPIC_ROOT;

    static const std::string FILE_DELETE_TOPIC_ROOT;
    static const std::string FILE_PURGE_TOPIC_ROOT;

    static const std::string FILE_LIST_REQUEST_TOPIC_ROOT;
    static const std::string FILE_LIST_RESPONSE_TOPIC_ROOT;
    static const std::string FILE_LIST_UPDATE_TOPIC_ROOT;
    static const std::string FILE_LIST_CONFIRM_TOPIC_ROOT;

    static const std::string FILE_URL_DOWNLOAD_INITIATE_TOPIC_ROOT;
    static const std::string FILE_URL_DOWNLOAD_ABORT_TOPIC_ROOT;
    static const std::string FILE_URL_DOWNLOAD_STATUS_TOPIC_ROOT;

    static const std::vector<std::string> INBOUND_CHANNELS;
};
}    // namespace wolkabout

#endif    // JSONDOWNLOADPROTOCOL_H
