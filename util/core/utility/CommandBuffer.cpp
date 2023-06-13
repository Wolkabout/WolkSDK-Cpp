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

#include "core/utility/CommandBuffer.h"

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace wolkabout::legacy
{
CommandBuffer::CommandBuffer()
: m_isRunning(true), m_worker(std::unique_ptr<std::thread>(new std::thread(&CommandBuffer::run, this)))
{
}

CommandBuffer::~CommandBuffer()
{
    stop();
}

void CommandBuffer::pushCommand(const std::shared_ptr<Command>& command)
{
    std::unique_lock<std::mutex> unique_lock(m_lock);

    m_pushCommandQueue.push(command);

    notify();
}

void CommandBuffer::stop()
{
    {
        std::lock_guard<std::mutex> lg{m_lock};
        m_isRunning = false;
    }
    notify();

    if (m_worker && m_worker->joinable())
    {
        m_worker->join();
    }
}

std::shared_ptr<CommandBuffer::Command> CommandBuffer::popCommand()
{
    if (m_popCommandQueue.empty())
    {
        return nullptr;
    }

    std::shared_ptr<Command> command = m_popCommandQueue.front();
    m_popCommandQueue.pop();
    return command;
}

bool CommandBuffer::empty() const
{
    std::unique_lock<std::mutex> unique_lock(m_lock);

    return m_pushCommandQueue.empty();
}

void CommandBuffer::switchBuffers()
{
    std::unique_lock<std::mutex> unique_lock(m_lock);

    if (m_pushCommandQueue.empty())
        m_condition.wait(unique_lock, [&] { return !m_pushCommandQueue.empty() || !m_isRunning; });

    std::swap(m_pushCommandQueue, m_popCommandQueue);
}

void CommandBuffer::notify()
{
    m_condition.notify_one();
}

void CommandBuffer::processCommands()
{
    switchBuffers();

    if (!m_isRunning)
        return;

    std::shared_ptr<std::function<void()>> command;
    while ((command = popCommand()) != nullptr)
    {
        command->operator()();
    }
}

void CommandBuffer::run()
{
    while (m_isRunning)
    {
        processCommands();
    }
}
}    // namespace wolkabout::legacy
