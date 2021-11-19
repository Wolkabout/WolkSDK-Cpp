/*
 * Copyright 2021 Adriateh d.o.o.
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

#include "core/protocol/wolkabout/WolkaboutFileManagementProtocol.h"
#include "core/protocol/wolkabout/WolkaboutProtocol.h"

namespace wolkabout
{
std::vector<std::string> WolkaboutFileManagementProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutFileManagementProtocol::getInboundChannelsForDevice(
  const std::string& deviceKey) const
{
    return {PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER + "file_upload_initiate",
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER + "file_upload_abort",
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER + "file_binary_response"};
}

std::string WolkaboutFileManagementProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    return {};
}
}    // namespace wolkabout
