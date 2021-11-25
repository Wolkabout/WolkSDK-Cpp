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

#ifndef WOLKABOUTCORE_SYNCHRONIZEPARAMETERSMESSAGE_H
#define WOLKABOUTCORE_SYNCHRONIZEPARAMETERSMESSAGE_H

#include "core/model/messages/MessageObject.h"

namespace wolkabout
{
class SynchronizeParametersMessage : public MessageObject
{
public:
    explicit SynchronizeParametersMessage(std::vector<ParameterName> parameters);

    const std::vector<ParameterName>& getParameters() const;

    MessageType getMessageType() override;

private:
    std::vector<ParameterName> m_parameters;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_SYNCHRONIZEPARAMETERSMESSAGE_H
