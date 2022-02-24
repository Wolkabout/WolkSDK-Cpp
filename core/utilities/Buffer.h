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

#ifndef BUFFER_H
#define BUFFER_H

#include "core/utilities/Logger.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace wolkabout
{
template <class T> class Buffer
{
public:
    Buffer() = default;
    ~Buffer();

    void stop();

    void push(T item);
    void push_rvalue(T&& item);

    T pop();

    bool isEmpty() const;

    void swapBuffers();

    void notify();

private:
    std::queue<T> m_pushQueue;
    std::queue<T> m_popQueue;

    mutable std::mutex m_pushLock;
    mutable std::mutex m_popLock;
    std::condition_variable m_condition;
    std::atomic_bool m_exitCondition{false};
};

template <class T> Buffer<T>::~Buffer()
{
    stop();
}

template <class T> void Buffer<T>::stop()
{
    {
        std::lock_guard<std::mutex> lg{m_pushLock};
        m_exitCondition = true;
    }
    m_condition.notify_one();
}

template <class T> void Buffer<T>::push(T item)
{
    {
        std::lock_guard<std::mutex> guard{m_pushLock};
        m_pushQueue.push(std::move(item));
    }

    m_condition.notify_one();
}

template <class T> void Buffer<T>::push_rvalue(T&& item)
{
    {
        std::lock_guard<std::mutex> guard{m_pushLock};
        m_pushQueue.push(std::move(item));
    }

    m_condition.notify_one();
}

template <class T> T Buffer<T>::pop()
{
    std::lock_guard<std::mutex> guard{m_popLock};

    if (m_popQueue.empty())
    {
        LOG(ERROR) << "Popping from empty buffer";
        return {};
    }

    T item = m_popQueue.front();
    m_popQueue.pop();

    return item;
}

template <class T> void Buffer<T>::swapBuffers()
{
    std::unique_lock<std::mutex> pushGuard(m_pushLock);

    if (m_pushQueue.empty())
        m_condition.wait(pushGuard, [&] { return !m_pushQueue.empty() || m_exitCondition; });

    std::lock_guard<std::mutex> popGuard{m_popLock};
    std::swap(m_pushQueue, m_popQueue);
}

template <class T> bool Buffer<T>::isEmpty() const
{
    std::lock_guard<std::mutex> guard{m_popLock};

    return m_popQueue.empty();
}

template <class T> void Buffer<T>::notify()
{
    m_condition.notify_one();
}
}    // namespace wolkabout

#endif
