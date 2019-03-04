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

#include "model/AlarmTemplate.h"

#include <utility>

namespace wolkabout
{
AlarmTemplate::AlarmTemplate(std::string name, AlarmTemplate::AlarmSeverity severity, std::string reference,
                             std::string message, std::string description)
: m_name(std::move(name))
, m_severity(severity)
, m_reference(std::move(reference))
, m_message(std::move(message))
, m_description(std::move(description))
{
}

const std::string& AlarmTemplate::getName() const
{
    return m_name;
}

AlarmTemplate& AlarmTemplate::setName(const std::string& name)
{
    m_name = name;
    return *this;
}

AlarmTemplate::AlarmSeverity AlarmTemplate::getSeverity() const
{
    return m_severity;
}

AlarmTemplate& AlarmTemplate::setSeverity(AlarmTemplate::AlarmSeverity severity)
{
    m_severity = severity;
    return *this;
}

const std::string& AlarmTemplate::getReference() const
{
    return m_reference;
}

AlarmTemplate& AlarmTemplate::setReference(const std::string& reference)
{
    m_reference = reference;
    return *this;
}

const std::string& AlarmTemplate::getMessage() const
{
    return m_message;
}

AlarmTemplate& AlarmTemplate::setMessage(const std::string& message)
{
    m_message = message;
    return *this;
}

const std::string& AlarmTemplate::getDescription() const
{
    return m_description;
}

AlarmTemplate& AlarmTemplate::setDescription(const std::string& description)
{
    m_description = description;
    return *this;
}

bool AlarmTemplate::operator==(AlarmTemplate& rhs) const
{
    if (m_name != rhs.m_name || m_severity != rhs.m_severity || m_reference != rhs.m_reference ||
        m_message != rhs.m_message || m_description != rhs.m_description)
    {
        return false;
    }

    return true;
}

bool AlarmTemplate::operator!=(AlarmTemplate& rhs) const
{
    return !(*this == rhs);
}
}    // namespace wolkabout
