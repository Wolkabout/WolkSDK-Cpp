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

#include "protocol/FileDownloadProtocol.h"

namespace wolkabout
{
class JsonDownloadProtocol : public FileDownloadProtocol
{
public:
    explicit JsonDownloadProtocol(bool isGateway = false);

    const std::string& getName() const override;
    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    bool isBinary(const Message& message) const override;

    std::unique_ptr<BinaryData> makeBinaryData(const Message& message) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const FilePacketRequest& filePacketRequest) const override;

    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;

    bool isUploadInitiate(const Message& message) const override;

    bool isUploadAbort(const Message& message) const override;

    std::unique_ptr<FileUploadInitiate> makeFileUploadInitiate(const Message& message) const override;

    std::unique_ptr<FileUploadAbort> makeFileUploadAbort(const Message& message) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const FileUploadStatus& fileUploadStatus) const override;

private:
    std::string m_devicePrefix;

    static const std::string NAME;

    static const std::string CHANNEL_DELIMITER;
    static const std::string CHANNEL_MULTI_LEVEL_WILDCARD;
    static const std::string CHANNEL_SINGLE_LEVEL_WILDCARD;

    static const std::string GATEWAY_PATH_PREFIX;
    static const std::string DEVICE_PATH_PREFIX;
    static const std::string DEVICE_TO_PLATFORM_DIRECTION;
    static const std::string PLATFORM_TO_DEVICE_DIRECTION;

    static const std::string FILE_UPLOAD_INITIATE_TOPIC_ROOT;
    static const std::string FILE_UPLOAD_ABORT_TOPIC_ROOT;
    static const std::string FILE_UPLOAD_STATUS_TOPIC_ROOT;

    static const std::string BINARY_REQUEST_TOPIC_ROOT;
    static const std::string BINARY_RESPONSE_TOPIC_ROOT;

    static const std::vector<std::string> INBOUND_CHANNELS;
};
}    // namespace wolkabout

#endif    // JSONDOWNLOADPROTOCOL_H
