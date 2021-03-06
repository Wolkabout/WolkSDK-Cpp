/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class Protocol
{
public:
    virtual ~Protocol() = default;

    /**
     * @brief Get generic inbound channels
     * @return
     */
    virtual std::vector<std::string> getInboundChannels() const = 0;

    /**
     * @brief Get inbound channels for provided device key
     * @param deviceKey
     * @return
     */
    virtual std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const = 0;

    virtual std::string extractDeviceKeyFromChannel(const std::string& topic) const = 0;
};
}    // namespace wolkabout

#endif
