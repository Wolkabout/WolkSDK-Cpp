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

#ifndef WOLKABOUTCORE_REGISTEREDDEVICESREQUESTMESSAGE_H
#define WOLKABOUTCORE_REGISTEREDDEVICESREQUESTMESSAGE_H

#include "core/model/messages/MessageModel.h"

#include <chrono>

namespace wolkabout
{
class RegisteredDevicesRequestMessage : public MessageModel
{
public:
    explicit RegisteredDevicesRequestMessage(std::chrono::milliseconds timestampFrom = std::chrono::milliseconds{0},
                                             std::string deviceType = {}, std::string externalId = {});

    std::chrono::milliseconds getTimestampFrom() const;

    const std::string& getDeviceType() const;

    const std::string& getExternalId() const;

    MessageType getMessageType() const override;

private:
    std::chrono::milliseconds m_timestampFrom;
    std::string m_deviceType;
    std::string m_externalId;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_REGISTEREDDEVICESREQUESTMESSAGE_H
