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

#ifndef WOLKABOUTCORE_ATTRIBUTEREGISTRATIONMESSAGE_H
#define WOLKABOUTCORE_ATTRIBUTEREGISTRATIONMESSAGE_H

#include "core/model/Attribute.h"
#include "core/model/messages/MessageModel.h"

#include <memory>
#include <vector>

namespace wolkabout
{
class AttributeRegistrationMessage : public MessageModel
{
public:
    explicit AttributeRegistrationMessage(std::vector<Attribute> attributes);

    MessageType getMessageType() const override;

    const std::vector<Attribute>& getAttributes() const;

private:
    std::vector<Attribute> m_attributes;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_ATTRIBUTEREGISTRATIONMESSAGE_H
