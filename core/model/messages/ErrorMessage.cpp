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

#include "core/model/messages/ErrorMessage.h"

#include <utility>

namespace wolkabout
{
ErrorMessage::ErrorMessage(std::string deviceKey, std::string errorMessage,
                           std::chrono::system_clock::time_point arrivalTime)
: m_deviceKey(std::move(deviceKey))
, m_message(std::move(errorMessage))
, m_arrivalTime(arrivalTime.time_since_epoch().count() != 0 ? arrivalTime : std::chrono::system_clock::now())
{
}

const std::string& ErrorMessage::getDeviceKey() const
{
    return m_deviceKey;
}

const std::string& ErrorMessage::getMessage() const
{
    return m_message;
}

MessageType ErrorMessage::getMessageType() const
{
    return MessageType::ERROR;
}

const std::chrono::system_clock::time_point& ErrorMessage::getArrivalTime() const
{
    return m_arrivalTime;
}
}    // namespace wolkabout
