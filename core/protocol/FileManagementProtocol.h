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
 * This is an interface defining a FileManagementProtocol, used to accomplish the FileManagement functionality of the
 * platform.
 * This protocol defines the serializers/deserializers for a bunch of messages that are necessary to accomplish that
 * functionality.
 */
class FileManagementProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a FileUploadStatusMessage.
     *
     * @param deviceKey The device key for which the FileUploadStatusMessage is regarding.
     * @param message The message containing information about a currently ongoing file upload process.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const FileUploadStatusMessage& message) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a FileBinaryRequestMessage.
     *
     * @param deviceKey The device key for which the FileBinaryRequestMessage is regarding.
     * @param message The message containing a request from a device for a chunk of a file that is currently being
     * transferred.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const FileBinaryRequestMessage& message) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a FileUrlDownloadStatusMessage.
     *
     * @param deviceKey The device key for which the FileUrlDownloadStatusMessage is regarding.
     * @param message The message containing information about a currently ongoing file download process.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const FileUrlDownloadStatusMessage& message) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a FileListResponseMessage.
     *
     * @param deviceKey The device key for which the FileListResponseMessage is regarding.
     * @param message The message containing information about a current list of files the device currently holds.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const FileListResponseMessage& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileUploadInitiateMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileUploadInitiateMessage.
     * @return A parsed FileUploadInitiateMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileUploadInitiateMessage> parseFileUploadInit(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileUploadAbortMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileUploadAbortMessage.
     * @return A parsed FileUploadAbortMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileUploadAbortMessage> parseFileUploadAbort(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileBinaryResponseMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileBinaryResponseMessage.
     * @return A parsed FileBinaryResponseMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileBinaryResponseMessage> parseFileBinaryResponse(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileUrlDownloadInitMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileUrlDownloadInitMessage.
     * @return A parsed FileUrlDownloadInitMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileUrlDownloadInitMessage> parseFileUrlDownloadInit(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileUrlDownloadAbortMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileUrlDownloadAbortMessage.
     * @return A parsed FileUrlDownloadAbortMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileUrlDownloadAbortMessage> parseFileUrlDownloadAbort(
      std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileListRequestMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileListRequestMessage.
     * @return A parsed FileListRequestMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileListRequestMessage> parseFileListRequest(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FileDeleteMessage.
     *
     * @param message The received MQTT message that is potentially a valid FileDeleteMessage.
     * @return A parsed FileDeleteMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FileDeleteMessage> parseFileDelete(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FilePurgeMessage.
     *
     * @param message The received MQTT message that is potentially a valid FilePurgeMessage.
     * @return A parsed FilePurgeMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FilePurgeMessage> parseFilePurge(std::shared_ptr<Message> message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILEMANAGEMENTPROTOCOL_H
