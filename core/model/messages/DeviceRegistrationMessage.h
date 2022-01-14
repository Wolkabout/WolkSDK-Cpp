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

#ifndef WOLKABOUTCORE_DEVICEREGISTRATIONMESSAGE_H
#define WOLKABOUTCORE_DEVICEREGISTRATIONMESSAGE_H

#include "core/model/Attribute.h"
#include "core/model/Feed.h"
#include "core/model/messages/MessageModel.h"

#include <map>

namespace wolkabout
{
// This is a collection of information required to register a device.
struct DeviceRegistrationData
{
    std::string name;
    std::string key;
    std::string guid;
    std::map<ParameterName, std::string> parameters;
    std::map<std::string, Feed> feeds;
    std::map<std::string, Attribute> attributes;
};

class DeviceRegistrationMessage : public MessageModel
{
public:
    explicit DeviceRegistrationMessage(std::vector<DeviceRegistrationData> devices);

    const std::vector<DeviceRegistrationData>& getDevices() const;

    MessageType getMessageType() const override;

private:
    std::vector<DeviceRegistrationData> m_devices;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_DEVICEREGISTRATIONMESSAGE_H
