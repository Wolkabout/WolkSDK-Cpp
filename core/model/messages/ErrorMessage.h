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

#ifndef WOLKABOUTCORE_ERRORMESSAGE_H
#define WOLKABOUTCORE_ERRORMESSAGE_H

#include "core/model/messages/MessageModel.h"

#include <chrono>
#include <string>

namespace wolkabout
{
class ErrorMessage : public MessageModel
{
public:
    explicit ErrorMessage(std::string deviceKey, std::string errorMessage,
                          std::chrono::system_clock::time_point arrivalTime = {});

    const std::string& getDeviceKey() const;

    const std::string& getMessage() const;

    const std::chrono::system_clock::time_point& getArrivalTime() const;

    MessageType getMessageType() const override;

private:
    std::string m_deviceKey;
    std::string m_message;
    std::chrono::system_clock::time_point m_arrivalTime;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_ERRORMESSAGE_H
