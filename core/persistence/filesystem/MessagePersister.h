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

#ifndef MESSAGEPERSISTER_H
#define MESSAGEPERSISTER_H

#include "core/model/Message.h"

#include <memory>

namespace wolkabout
{
class MessagePersister
{
public:
    virtual ~MessagePersister() = default;

    virtual std::string save(const Message& message) const;
    virtual std::unique_ptr<Message> load(const std::string& text) const;
};
}    // namespace wolkabout

#endif    // MESSAGEPERSISTER_H
