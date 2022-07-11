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

#ifndef WOLKABOUTCORE_PARAMETERSUPDATEMESSAGE_H
#define WOLKABOUTCORE_PARAMETERSUPDATEMESSAGE_H

#include "core/model/messages/MessageModel.h"

namespace wolkabout
{
class ParametersUpdateMessage : public MessageModel
{
public:
    explicit ParametersUpdateMessage(std::vector<Parameter> parameterList);

    MessageType getMessageType() const override;

    const std::vector<Parameter>& getParameters() const;

private:
    std::vector<Parameter> m_parameterList;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_PARAMETERSUPDATEMESSAGE_H
