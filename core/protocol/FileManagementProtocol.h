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

#ifndef WOLKABOUTCORE_FILEMANAGEMENTPROTOCOL_H
#define WOLKABOUTCORE_FILEMANAGEMENTPROTOCOL_H

#include "core/model/messages/FileBinaryRequestMessage.h"
#include "core/model/messages/FileBinaryResponseMessage.h"
#include "core/model/messages/FileDeleteMessage.h"
#include "core/model/messages/FileListRequestMessage.h"
#include "core/model/messages/FileListResponseMessage.h"
#include "core/model/messages/FilePurgeMessage.h"
#include "core/model/messages/FileUploadAbortMessage.h"
#include "core/model/messages/FileUploadInitiateMessage.h"
#include "core/model/messages/FileUploadStatusMessage.h"
#include "core/model/messages/FileUrlDownloadAbortMessage.h"
#include "core/model/messages/FileUrlDownloadInitMessage.h"
#include "core/model/messages/FileUrlDownloadStatusMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This is an abstract class definition for a FileManagement protocol implementation.
 * This protocol is able to convert inbound and outbound messages regarding the FileManagement functionality.
 */
class FileManagementProtocol : public Protocol
{
public:
    virtual std::unique_ptr<MqttMessage> makeOutboundMessage(const std::string& deviceKey,
                                                             const FileUploadStatusMessage& message) = 0;

    virtual std::unique_ptr<MqttMessage> makeOutboundMessage(const std::string& deviceKey,
                                                             const FileBinaryRequestMessage& message) = 0;

    virtual std::unique_ptr<MqttMessage> makeOutboundMessage(const std::string& deviceKey,
                                                             const FileUrlDownloadStatusMessage& message) = 0;

    virtual std::unique_ptr<MqttMessage> makeOutboundMessage(const std::string& deviceKey,
                                                             const FileListResponseMessage& message) = 0;

    virtual std::shared_ptr<FileUploadInitiateMessage> parseFileUploadInit(std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FileUploadAbortMessage> parseFileUploadAbort(std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FileBinaryResponseMessage> parseFileBinaryResponse(
      std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FileUrlDownloadInitMessage> parseFileUrlDownloadInit(
      std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FileUrlDownloadAbortMessage> parseFileUrlDownloadAbort(
      std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FileListRequestMessage> parseFileListRequest(std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FileDeleteMessage> parseFileDelete(std::shared_ptr<MqttMessage> message) = 0;

    virtual std::shared_ptr<FilePurgeMessage> parseFilePurge(std::shared_ptr<MqttMessage> message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILEMANAGEMENTPROTOCOL_H
