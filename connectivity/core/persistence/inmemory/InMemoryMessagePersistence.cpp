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

#include "core/persistence/inmemory/InMemoryMessagePersistence.h"

#include "core/model/Message.h"

namespace wolkabout
{
bool InMemoryMessagePersistence::push(std::shared_ptr<Message> message)
{
    std::lock_guard<std::mutex> lg{m_lock};
    m_queue.push(message);
    return true;
}

void InMemoryMessagePersistence::pop()
{
    std::lock_guard<std::mutex> lg{m_lock};
    m_queue.pop();
}

std::shared_ptr<Message> InMemoryMessagePersistence::front()
{
    std::lock_guard<std::mutex> lg{m_lock};
    return m_queue.front();
}

bool InMemoryMessagePersistence::empty() const
{
    std::lock_guard<std::mutex> lg{m_lock};
    return m_queue.empty();
}
}    // namespace wolkabout
