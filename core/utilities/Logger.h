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

#ifndef WOLKABOUTCORE_LOGGER_H
#define WOLKABOUTCORE_LOGGER_H

#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace spdlog
{
class logger;
namespace sinks
{
template <typename T> class ringbuffer_sink;
}
}    // namespace spdlog

namespace wolkabout::legacy
{
class Log;

enum class LogLevel
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    OFF
};

/**
 * @brief Converts a log level from string.
 * @param level log level as string
 * @return corresponding log level (supported: ERROR, WARN, INFO, DEBUG, TRACE and
 * OFF. All other will be considered as ERROR)
 */
wolkabout::legacy::LogLevel from_string(std::string level);

class Logger
{
public:
    enum class Type
    {
        CONSOLE = 1,
        FILE = 2,
        BUFFER = 4
    };

    virtual ~Logger() = default;

    static void set(std::unique_ptr<Logger> logger);

    static Logger& getInstance();

    static void init(LogLevel level, Type type, const std::string& filePathWithExtension = "");

    void operator+=(const Log& log);

    virtual void logEntry(const Log& log);

    std::vector<std::string> buffer();

private:
    void logMessage(const Log& log, spdlog::logger& logger);

    static void setupFileLogger(const std::string& filePathWithExtension);
    static void setupConsoleLogger();
    static void setupBufferLogger();
    static void setupLoggerParams(LogLevel level);

    static std::unique_ptr<Logger> m_instance;

    std::shared_ptr<spdlog::logger> m_fileLogger = nullptr;
    std::shared_ptr<spdlog::logger> m_consoleLogger = nullptr;
    std::shared_ptr<spdlog::logger> m_bufferLogger = nullptr;
    spdlog::sinks::ringbuffer_sink<std::mutex>* m_bufferSink = nullptr;
};

inline constexpr Logger::Type operator|(Logger::Type a, Logger::Type b)
{
    return static_cast<Logger::Type>(static_cast<int>(a) | static_cast<int>(b));
}

inline constexpr bool operator&(Logger::Type a, Logger::Type b)
{
    return static_cast<int>(a) & static_cast<int>(b);
}

class Log
{
public:
    Log(LogLevel level);

    template <typename T> Log& operator<<(T value);

    LogLevel getLogLevel() const;

    std::string getMessage() const;

private:
    const LogLevel m_level;
    std::stringstream m_message;
};

template <typename T> Log& Log::operator<<(T value)
{
    m_message << value;
    return *this;
}

class LOG
{
public:
	explicit LOG(wolkabout::legacy::LogLevel level, bool doLog = true);
    virtual ~LOG();

    template <typename T> LOG& operator<<(T value)
    {
        m_log << value;
        return *this;
    }

private:
	wolkabout::legacy::Log m_log;
    bool m_doLog;
};

class LOG_WHEN_TRUE : public LOG
{
public:
    using LOG::LOG;
	LOG_WHEN_TRUE(wolkabout::legacy::LogLevel) = delete;
};

constexpr auto TRACE = wolkabout::legacy::LogLevel::TRACE;
constexpr auto DEBUG = wolkabout::legacy::LogLevel::DEBUG;
constexpr auto INFO = wolkabout::legacy::LogLevel::INFO;
constexpr auto WARN = wolkabout::legacy::LogLevel::WARN;
constexpr auto ERROR = wolkabout::legacy::LogLevel::ERROR;

#ifndef METHOD_INFO
#define METHOD_INFO __PRETTY_FUNCTION__
#endif
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_LOGGER_H
