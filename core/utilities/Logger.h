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

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

namespace spdlog
{
class logger;
namespace sinks
{
template <typename Mutex> class ringbuffer_sink;
}
}    // namespace spdlog

namespace wolkabout
{
class Log;

enum class LogLevel
{
    INFO = 0,
    WARN,
    ERROR,
    DEBUG,
    TRACE,
    OFF
};

/**
 * @brief Converts a log level from string.
 * @param level log level as string
 * @return corresponding log level (supported: ERROR, WARN, INFO, DEBUG, TRACE and
 * OFF. All other will be considered as ERROR)
 */
wolkabout::LogLevel from_string(std::string level);

/**
 * @brief The Logger class
 *
 * Abstract class that should be extended by specific logger. Logger is accessed
 * as a singleton. But this single instance must be set via #setInstance.
 */
class Logger
{
public:
    enum class Type
    {
        CONSOLE = 1,
        FILE = 2,
        BUFFER = 4
    };

    /**
     * @brief Logger destructor.
     */
    virtual ~Logger() = default;

    /**
     * @brief Prints a log message to a standard output.
     * @param log log to be printed
     */
    void operator+=(Log& log);

    void logEntry(const Log& log);

    /**
     * @brief Sets the log level of the logger.
     * @param level log level to be set
     */
    void setLevel(wolkabout::LogLevel level);

    /**
     * @brief Initialize logger.
     * @param level Indicates logging level
     * @param type Type of logger to use
     * @param filePathWithExtension File path when using file based logger
     */
    static void init(LogLevel level, Type type, const std::string& filePathWithExtension = "");

    /**
     * @brief Provides a logger singleton instance.
     * @return logger instance
     */
    static Logger& getInstance();

    std::vector<std::string> buffer();

private:
    Logger() = default;
    void logMessage(const Log& log, spdlog::logger& logger);

    static void setupFileLogger(const std::string& filePathWithExtension);
    static void setupConsoleLogger();
    static void setupBufferLogger();
    static void setupLoggerParams(LogLevel level);

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

/**
 * @brief The Log class
 *
 * This class holds a message that should be printed in the log file or console.
 */
class Log
{
public:
    /**
     * @brief Log constructor.
     * @param level log level that will be used for this log
     */
    explicit Log(wolkabout::LogLevel level);

    /**
     * @brief This is log appender for string value
     * @param value value to be appended
     * @return reference to appended log instance
     */
    template <typename T> Log& operator<<(T value);

    /**
     * @brief Provides a log level for this log.
     * @return log level
     */
    wolkabout::LogLevel getLogLevel() const;

    /**
     * @brief Provides a message for this log.
     * @return message
     */
    std::string getMessage() const;

private:
    const wolkabout::LogLevel m_level;
    std::stringstream m_message;
};

template <typename T> Log& Log::operator<<(T value)
{
    m_message << value;
    return *this;
}

#define METHOD_INFO __FILE__ << ":" << __func__ << ":" << __LINE__ << ": "

#define PREPEND_NAMED_SCOPE(logLevel) wolkabout::LogLevel::logLevel

#define LOG_(level) wolkabout::Logger::getInstance() += wolkabout::Log(level)

#define LOG(level) LOG_(PREPEND_NAMED_SCOPE(level))
}    // namespace wolkabout

#endif    // LOGGER_H
