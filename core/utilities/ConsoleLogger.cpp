/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#include "utilities/ConsoleLogger.h"

#include <iostream>

namespace wolkabout
{
ConsoleLogger::ConsoleLogger() : m_flusher{[this] { flush(); }} {}

ConsoleLogger::~ConsoleLogger()
{
    m_run = false;
    m_condition.notify_one();

    if (m_flusher.joinable())
    {
        m_flusher.join();
    }
}

void ConsoleLogger::logEntry(Log& log)
{
    if (static_cast<int>(log.getLogLevel()) >= static_cast<int>(m_level.load()))
    {
        switch (log.getLogLevel())
        {
        case LogLevel::TRACE:
        {
            std::cout << getFormattedDateTime() << "[T]" << log.getMessage();
            break;
        }
        case LogLevel::DEBUG:
        {
            std::cout << getFormattedDateTime() << "[D]" << log.getMessage();
            break;
        }
        case LogLevel::INFO:
        {
            std::cout << getFormattedDateTime() << "[I]" << log.getMessage();
            break;
        }
        case LogLevel::WARN:
        {
            std::cout << getFormattedDateTime() << "[W]" << log.getMessage();
            break;
        }
        case LogLevel::ERROR:
        {
            std::cout << getFormattedDateTime() << "[E]" << log.getMessage();
            break;
        }
        }

        if (static_cast<int>(log.getLogLevel()) >= static_cast<int>(m_flushAtLevel.load()))
        {
            std::cout << std::endl;
        }
        else
        {
            std::cout << "\n";
        }
    }
}

void ConsoleLogger::setLogLevel(wolkabout::LogLevel level)
{
    m_level = level;
}

void ConsoleLogger::flushAt(LogLevel level)
{
    m_flushAtLevel = level;
}

void ConsoleLogger::flushEvery(std::chrono::seconds period)
{
    if (period.count() == 0)
    {
        m_run = false;
    }

    m_condition.notify_one();

    std::lock_guard<std::mutex> guard{m_mutex};
    m_flushEvery = period;
}

void ConsoleLogger::flush()
{
    m_run = true;
    while (m_run)
    {
        std::unique_lock<std::mutex> lock{m_mutex};

        m_condition.wait_for(lock, m_flushEvery);

        std::cout << std::flush;
    }
}

std::string ConsoleLogger::getFormattedDateTime()
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char _retval[20];
    strftime(_retval, sizeof(_retval), "%Y-%m-%d %H:%M:%S", timeinfo);

    return _retval;
}
}    // namespace wolkabout
