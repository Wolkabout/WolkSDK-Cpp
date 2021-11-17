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

#ifndef WOLKABOUTCORE_DEVICE_H
#define WOLKABOUTCORE_DEVICE_H

#include "core/Types.h"
#include "core/model/Attribute.h"
#include "core/model/Feed.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class DeviceTemplate;

class Device
{
public:
    Device(std::string key, std::string password, OutboundDataMode type,
           std::shared_ptr<DeviceTemplate> deviceTemplate = nullptr);

    Device(std::string name, std::string key, std::string password, OutboundDataMode type,
           std::shared_ptr<DeviceTemplate> deviceTemplate = nullptr);

    const std::string& getName() const;

    void setName(const std::string& name);

    const std::string& getKey() const;

    void setKey(const std::string& key);

    const std::string& getPassword() const;

    void setPassword(const std::string& password);

    const std::vector<Feed>& getFeeds() const;

    void setFeeds(const std::vector<Feed>& feeds);

    const std::vector<Attribute>& getAttributes() const;

    void setAttributes(const std::vector<Attribute>& attributes);

    const std::map<ParameterName, std::string>& getParameters() const;

    void setParameters(const std::map<ParameterName, std::string>& parameters);

    OutboundDataMode getType() const;

    void setType(OutboundDataMode type);

private:
    std::string m_name;
    std::string m_key;
    std::string m_password;
    OutboundDataMode m_type;

    std::vector<Feed> m_feeds;
    std::vector<Attribute> m_attributes;
    std::map<ParameterName, std::string> m_parameters;
};
}    // namespace wolkabout
#endif    // WOLKABOUTCORE_DEVICE_H
