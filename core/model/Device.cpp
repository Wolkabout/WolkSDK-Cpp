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

#include "core/model/Device.h"

namespace wolkabout
{
Device::Device(std::string key, std::string password, OutboundDataMode type, std::string name)
: m_name(std::move(name)), m_key(std::move(key)), m_password(std::move(password)), m_type(type)
{
    if (m_type < OutboundDataMode::PUSH || m_type > OutboundDataMode::PULL)
        throw std::runtime_error("Failed to create Device with an invalid OutboundDataMode value.");
}

const std::string& Device::getName() const
{
    return m_name;
}

void Device::setName(const std::string& name)
{
    m_name = name;
}

const std::string& Device::getKey() const
{
    return m_key;
}

const std::string& Device::getPassword() const
{
    return m_password;
}

const std::vector<Feed>& Device::getFeeds() const
{
    return m_feeds;
}

void Device::setFeeds(const std::vector<Feed>& feeds)
{
    m_feeds = feeds;
}

const std::vector<Attribute>& Device::getAttributes() const
{
    return m_attributes;
}

void Device::setAttributes(const std::vector<Attribute>& attributes)
{
    m_attributes = attributes;
}

const std::map<ParameterName, std::string>& Device::getParameters() const
{
    return m_parameters;
}

void Device::setParameters(const std::map<ParameterName, std::string>& parameters)
{
    m_parameters = parameters;
}

OutboundDataMode Device::getType() const
{
    return m_type;
}
}    // namespace wolkabout
