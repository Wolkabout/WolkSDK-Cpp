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

#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "utilities/Logger.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <thread>

namespace wolkabout
{
class ConsoleLogger : public Logger
{
public:
    ConsoleLogger();
    ~ConsoleLogger();

    void logEntry(Log& log) override;
    void setLogLevel(wolkabout::LogLevel level) override;

    void flushAt(LogLevel level);
    void flushEvery(std::chrono::seconds period);

private:
    void flush();

    static std::string getFormattedDateTime();

    std::atomic<LogLevel> m_level{LogLevel::ERROR};
    std::atomic<LogLevel> m_flushAtLevel{LogLevel::INFO};
    std::chrono::seconds m_flushPeriod{5};

    std::atomic_bool m_run;
    std::condition_variable m_condition;
    std::mutex m_mutex;
    std::thread m_flusher;
};
}    // namespace wolkabout

#endif
