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

#ifndef DFUPROTOCOL_H
#define DFUPROTOCOL_H

#include "protocol/FirmwareUpdateProtocol.h"

namespace wolkabout
{
class DFUProtocol : public FirmwareUpdateProtocol
{
public:
    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const FirmwareUpdateResponse& firmwareUpdateResponse) const override;

    std::unique_ptr<Message> makeFromFirmwareVersion(const std::string& deviceKey,
                                                     const std::string& firmwareVerion) const override;

    bool isFirmwareUpdateMessage(const Message& message) const override;

    std::unique_ptr<FirmwareUpdateCommand> makeFirmwareUpdateCommand(const Message& message) const override;

    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;

private:
    static const std::string FIRMWARE_UPDATE_STATUS_TOPIC_ROOT;
    static const std::string FIRMWARE_VERSION_TOPIC_ROOT;

    static const std::string FIRMWARE_UPDATE_COMMAND_TOPIC_ROOT;

    static const std::vector<std::string> INBOUND_CHANNELS;
};
}    // namespace wolkabout

#endif    // DFUPROTOCOL_H
