/**
 * Copyright 2021 Wolkabout s.r.o.
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

#include "core/model/messages/PlatformStatusMessage.h"

#include "core/utilities/Logger.h"

#include <algorithm>

namespace wolkabout
{
std::string toString(ConnectivityStatus value)
{
    LOG(TRACE) << METHOD_INFO;

    switch (value)
    {
    case ConnectivityStatus::CONNECTED:
        return "CONNECTED";
    case ConnectivityStatus::OFFLINE:
        return "OFFLINE";
    default:
        return "";
    }
}

ConnectivityStatus connectivityStatusFromString(const std::string& value)
{
    LOG(TRACE) << METHOD_INFO;

    // Make an uppercase copy of the string
    auto upperCase = std::string(value);
    std::transform(upperCase.cbegin(), upperCase.cend(), upperCase.begin(), ::toupper);

    // Check the value of the string now
    if (upperCase == "CONNECTED")
        return ConnectivityStatus::CONNECTED;
    else if (upperCase == "OFFLINE")
        return ConnectivityStatus::OFFLINE;
    return ConnectivityStatus::NONE;
}

PlatformStatusMessage::PlatformStatusMessage(ConnectivityStatus status) : m_status(status) {}

ConnectivityStatus PlatformStatusMessage::getStatus() const
{
    return m_status;
}

MessageType PlatformStatusMessage::getMessageType() const
{
    return MessageType::SYNCHRONIZE_PARAMETERS;
}
}    // namespace wolkabout
