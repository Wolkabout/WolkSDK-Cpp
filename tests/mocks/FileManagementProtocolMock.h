/**
 * Copyright 2021 Wolkabout s.r.o.
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

#ifndef WOLKABOUTCONNECTOR_FILEMANAGEMENTPROTOCOLMOCK_H
#define WOLKABOUTCONNECTOR_FILEMANAGEMENTPROTOCOLMOCK_H

#include "core/protocol/FileManagementProtocol.h"

#include <gmock/gmock.h>

using namespace wolkabout;

class FileManagementProtocolMock : public FileManagementProtocol
{
public:
    // Protocol methods
    MOCK_METHOD(std::vector<std::string>, getInboundChannels, (), (const));
    MOCK_METHOD(std::vector<std::string>, getInboundChannelsForDevice, (const std::string&), (const));
    MOCK_METHOD(MessageType, getMessageType, (const Message&));
    MOCK_METHOD(DeviceType, getDeviceType, (const Message&));
    MOCK_METHOD(std::string, getDeviceKey, (const Message&), (const));
    // FileManagementProtocol methods
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, const FileUploadStatusMessage&));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, const FileBinaryRequestMessage&));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage,
                (const std::string&, const FileUrlDownloadStatusMessage&));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, const FileListResponseMessage&));
    MOCK_METHOD(std::unique_ptr<FileUploadInitiateMessage>, parseFileUploadInit, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FileUploadAbortMessage>, parseFileUploadAbort, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FileBinaryResponseMessage>, parseFileBinaryResponse, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FileUrlDownloadInitMessage>, parseFileUrlDownloadInit, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FileUrlDownloadAbortMessage>, parseFileUrlDownloadAbort, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FileListRequestMessage>, parseFileListRequest, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FileDeleteMessage>, parseFileDelete, (std::shared_ptr<Message>));
    MOCK_METHOD(std::unique_ptr<FilePurgeMessage>, parseFilePurge, (std::shared_ptr<Message>));
};

#endif    // WOLKABOUTCONNECTOR_FILEMANAGEMENTPROTOCOLMOCK_H
