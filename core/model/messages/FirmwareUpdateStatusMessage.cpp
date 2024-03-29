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

#include "core/model/messages/FirmwareUpdateStatusMessage.h"

namespace wolkabout
{
FirmwareUpdateStatusMessage::FirmwareUpdateStatusMessage(FirmwareUpdateStatus status, FirmwareUpdateError error)
: m_status(status), m_error(error)
{
}

FirmwareUpdateStatus FirmwareUpdateStatusMessage::getStatus() const
{
    return m_status;
}

FirmwareUpdateError FirmwareUpdateStatusMessage::getError() const
{
    return m_error;
}

MessageType FirmwareUpdateStatusMessage::getMessageType() const
{
    return MessageType::FIRMWARE_UPDATE_STATUS;
}
}    // namespace wolkabout
