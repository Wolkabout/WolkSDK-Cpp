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

#ifndef WOLKABOUTCORE_WOLKABOUTPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTPROTOCOL_H

#include "core/Types.h"
#include "core/model/MqttMessage.h"

namespace wolkabout
{
const std::string CHANNEL_DELIMITER = "/";
const std::string CHANNEL_MULTI_LEVEL_WILDCARD = "#";
const std::string CHANNEL_SINGLE_LEVEL_WILDCARD = "+";

const std::string DEVICE_TO_PLATFORM_DIRECTION = "d2p";
const std::string PLATFORM_TO_DEVICE_DIRECTION = "p2d";

const std::string TIMESTAMP_KEY = "timestamp";

class WolkaboutProtocol
{
public:
    static MessageType getMessageType(const std::shared_ptr<MqttMessage>& message);

    static std::string extractDeviceKeyFromChannel(const std::string& topic);
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_WOLKABOUTPROTOCOL_H
