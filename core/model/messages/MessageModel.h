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

#ifndef WOLKABOUTCORE_MESSAGEMODEL_H
#define WOLKABOUTCORE_MESSAGEMODEL_H

#include "core/Types.h"

namespace wolkabout
{
/**
 * This is an interface for all messages that are found in this protocol.
 * This way messages can announce their types before being downcast into a specific type.
 * The type is usually determined by a protocol parser, and the messages is later passed through to an message handler.
 */
class MessageModel
{
public:
    virtual ~MessageModel() = default;

    virtual MessageType getMessageType() const = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_MESSAGEMODEL_H
