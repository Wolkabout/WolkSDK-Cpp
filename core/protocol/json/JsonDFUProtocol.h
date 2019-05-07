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

#ifndef JSONDFUPROTOCOL_H
#define JSONDFUPROTOCOL_H

#include "protocol/FirmwareUpdateProtocol.h"

namespace wolkabout
{
class FirmwareUpdateAbort;
class FirmwareUpdateInstall;
class FirmwareUpdateStatus;
class FirmwareVersion;

class JsonDFUProtocol : public Protocol
{
public:
    explicit JsonDFUProtocol(bool isGateway = false);

    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;
    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const FirmwareUpdateStatus& firmwareUpdateStatus) const;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey, const FirmwareVersion& firmwareVersion) const;

    std::unique_ptr<FirmwareUpdateInstall> makeFirmwareUpdateInstall(const Message& message) const;
    std::unique_ptr<FirmwareUpdateAbort> makeFirmwareUpdateAbort(const Message& message) const;

private:
    bool m_isGateway;
    std::string m_devicePrefix;

    static const std::string FIRMWARE_UPDATE_STATUS_TOPIC_ROOT;
    static const std::string FIRMWARE_VERSION_TOPIC_ROOT;

    static const std::string FIRMWARE_UPDATE_INSTALL_TOPIC_ROOT;
    static const std::string FIRMWARE_UPDATE_ABORT_TOPIC_ROOT;
};
}    // namespace wolkabout

#endif    // JSONDFUPROTOCOL_H
