/**
 * Copyright 2022 Wolkabout Technology s.r.o.
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

#ifndef WOLKABOUTCONNECTOR_WOLKABOUTFILEMANAGEMENTPROTOCOL_H
#define WOLKABOUTCONNECTOR_WOLKABOUTFILEMANAGEMENTPROTOCOL_H

#include "core/protocol/FileManagementProtocol.h"

namespace wolkabout
{
class WolkaboutFileManagementProtocol : public FileManagementProtocol
{
public:
    std::vector<std::string> getInboundChannels() const override;

    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    std::string getDeviceKey(const Message& message) const override;

    DeviceType getDeviceType(const Message& message) override;

    MessageType getMessageType(const Message& message) override;

    std::string getResponseChannelForMessage(MessageType type, const std::string& deviceKey) const override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const FileUploadStatusMessage& message) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const FileBinaryRequestMessage& message) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const FileUrlDownloadStatusMessage& message) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const FileListResponseMessage& message) override;

    std::unique_ptr<FileUploadInitiateMessage> parseFileUploadInit(std::shared_ptr<Message> message) override;

    std::unique_ptr<FileUploadAbortMessage> parseFileUploadAbort(std::shared_ptr<Message> message) override;

    std::unique_ptr<FileBinaryResponseMessage> parseFileBinaryResponse(std::shared_ptr<Message> message) override;

    std::unique_ptr<FileUrlDownloadInitMessage> parseFileUrlDownloadInit(std::shared_ptr<Message> message) override;

    std::unique_ptr<FileUrlDownloadAbortMessage> parseFileUrlDownloadAbort(std::shared_ptr<Message> message) override;

    std::unique_ptr<FileListRequestMessage> parseFileListRequest(std::shared_ptr<Message> message) override;

    std::unique_ptr<FileDeleteMessage> parseFileDelete(std::shared_ptr<Message> message) override;

    std::unique_ptr<FilePurgeMessage> parseFilePurge(std::shared_ptr<Message> message) override;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCONNECTOR_WOLKABOUTFILEMANAGEMENTPROTOCOL_H
