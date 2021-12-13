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

#include "core/model/messages/SynchronizeParametersMessage.h"

#include <utility>

namespace wolkabout
{
SynchronizeParametersMessage::SynchronizeParametersMessage(std::vector<ParameterName> parameters)
: m_parameters(std::move(parameters))
{
}

const std::vector<ParameterName>& SynchronizeParametersMessage::getParameters() const
{
    return m_parameters;
}

MessageType SynchronizeParametersMessage::getMessageType() const
{
    return MessageType::SYNCHRONIZE_PARAMETERS;
}
}    // namespace wolkabout
