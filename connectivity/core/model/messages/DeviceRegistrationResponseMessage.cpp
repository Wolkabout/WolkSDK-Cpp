/**
 * Copyright 2023 Wolkabout Technology s.r.o.
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

#include "core/model/messages/DeviceRegistrationResponseMessage.h"

#include <utility>

namespace wolkabout
{
DeviceRegistrationResponseMessage::DeviceRegistrationResponseMessage(std::vector<std::string> success,
                                                                     std::vector<std::string> failure)
: m_success(std::move(success)), m_failed(std::move(failure))
{
}

const std::vector<std::string>& DeviceRegistrationResponseMessage::getSuccess() const
{
    return m_success;
}

const std::vector<std::string>& DeviceRegistrationResponseMessage::getFailed() const
{
    return m_failed;
}

MessageType DeviceRegistrationResponseMessage::getMessageType() const
{
    return MessageType::DEVICE_REGISTRATION_RESPONSE;
}
}    // namespace wolkabout
