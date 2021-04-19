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

#include "Logger.h"

#include <algorithm>
#include <iostream>
#include <spdlog/common.h>
#include <spdlog/details/file_helper.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

namespace
{
const unsigned BUFFER_LOG_SIZE = 5000;
}

namespace wolkabout
{
struct custom_filename_calculator
{
    // Create filename for the form basename.YYYY-MM-DD_hh-mm.ext
    static spdlog::filename_t calc_filename(const spdlog::filename_t& filename, const tm& now_tm)
    {
        spdlog::filename_t basename, ext;
        std::tie(basename, ext) = spdlog::details::file_helper::split_by_extension(filename);

        return fmt::format(SPDLOG_FILENAME_T("{}_{:04d}-{:02d}-{:02d}_{:02d}-{:02d}{}"), basename,
                           now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday, now_tm.tm_hour, now_tm.tm_min,
                           ext);
    }
};

void Logger::operator+=(Log& log)
{
    if (log.getLogLevel() == LogLevel::OFF)
        return;
    logEntry(log);
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::init(LogLevel level, Type type, const std::string& filePathWithExtension)
{
    static bool runOnce = false;
    if (runOnce)
    {
        return;
    }

    runOnce = true;

    if (type & Type::CONSOLE)
    {
        setupConsoleLogger();
    }

    if (type & Type::FILE)
    {
        setupFileLogger(filePathWithExtension);
    }

    if (type & Type::BUFFER)
    {
        setupBufferLogger();
    }

    setupLoggerParams(level);
}

void Logger::logEntry(const Log& log)
{
    if (m_fileLogger)
    {
        logMessage(log, *m_fileLogger);
    }

    if (m_consoleLogger)
    {
        logMessage(log, *m_consoleLogger);
    }

    if (m_bufferLogger)
    {
        logMessage(log, *m_bufferLogger);
    }
}
void Logger::setLevel(wolkabout::LogLevel level)
{
    switch (level)
    {
    case LogLevel::TRACE:
    {
        spdlog::set_level(spdlog::level::trace);
        break;
    }
    case LogLevel::DEBUG:
    {
        spdlog::set_level(spdlog::level::debug);
        break;
    }
    case LogLevel::INFO:
    {
        spdlog::set_level(spdlog::level::info);
        break;
    }
    case LogLevel::OFF:
    {
        spdlog::set_level(spdlog::level::off);
        break;
    }
    case LogLevel::ERROR:
    default:
    {
        spdlog::set_level(spdlog::level::err);
    }
    }
}

void Logger::logMessage(const Log& log, spdlog::logger& logger)
{
    switch (log.getLogLevel())
    {
    case LogLevel::ERROR:
    {
        return logger.error(log.getMessage());
    }
    case LogLevel::WARN:
    {
        return logger.warn(log.getMessage());
    }
    case LogLevel::INFO:
    {
        return logger.info(log.getMessage());
    }
    case LogLevel::DEBUG:
    {
        return logger.debug(log.getMessage());
    }
    case LogLevel::TRACE:
    {
        return logger.trace(log.getMessage());
    }
    }
}

void Logger::setupFileLogger(const std::string& filePathWithExtension)
{
    auto& instance = getInstance();

    try
    {
        instance.m_fileLogger = spdlog::synchronous_factory::template create<
          spdlog::sinks::daily_file_sink<std::mutex, custom_filename_calculator>>("fileLogger", filePathWithExtension,
                                                                                  0, 0);
    }
    catch (spdlog::spdlog_ex& e)
    {
        if (instance.m_consoleLogger)
        {
            Log log(LogLevel::ERROR);
            log << e.what();

            instance.logMessage(log, *instance.m_consoleLogger);
        }
        else
        {
            std::cout << e.what();
        }
    }
}

void Logger::setupConsoleLogger()
{
    auto& instance = getInstance();

    instance.m_consoleLogger = spdlog::stdout_logger_mt("consoleLogger");
}

void Logger::setupBufferLogger()
{
    auto& instance = getInstance();

    instance.m_bufferLogger = spdlog::synchronous_factory::template create<spdlog::sinks::ringbuffer_sink<std::mutex>>(
      "bufferLogger", BUFFER_LOG_SIZE);
    instance.m_bufferSink =
      dynamic_cast<spdlog::sinks::ringbuffer_sink_mt*>(instance.m_bufferLogger->sinks().at(0).get());
}

void Logger::setupLoggerParams(LogLevel level)
{
    spdlog::flush_every(std::chrono::seconds(15));
    spdlog::flush_on(spdlog::level::info);

    spdlog::set_pattern("[%D %H:%M:%S] [%L] %v");

    switch (level)
    {
    case LogLevel::TRACE:
    {
        spdlog::set_level(spdlog::level::trace);
        break;
    }
    case LogLevel::DEBUG:
    {
        spdlog::set_level(spdlog::level::debug);
        break;
    }
    case LogLevel::INFO:
    {
        spdlog::set_level(spdlog::level::info);
        break;
    }
    case LogLevel::OFF:
    {
        spdlog::set_level(spdlog::level::off);
        break;
    }
    case LogLevel::ERROR:
    default:
    {
        spdlog::set_level(spdlog::level::err);
    }
    }
}

Log::Log(LogLevel level) : m_level{level}, m_message{""} {}

LogLevel Log::getLogLevel() const
{
    return m_level;
}

std::string Log::getMessage() const
{
    return m_message.str();
}

std::vector<std::string> Logger::buffer()
{
    auto& instance = getInstance();

    if (!instance.m_bufferSink)
    {
        return {};
    }

    return instance.m_bufferSink->last_formatted();
}

wolkabout::LogLevel from_string(std::string level)
{
    std::transform(level.begin(), level.end(), level.begin(), ::toupper);

    if (level.compare("TRACE") == 0)
    {
        return wolkabout::LogLevel::TRACE;
    }
    else if (level.compare("DEBUG") == 0)
    {
        return wolkabout::LogLevel::DEBUG;
    }
    else if (level.compare("INFO") == 0)
    {
        return wolkabout::LogLevel::INFO;
    }
    else if (level.compare("WARN") == 0)
    {
        return wolkabout::LogLevel::WARN;
    }
    else if (level.compare("OFF") == 0)
    {
        return wolkabout::LogLevel::OFF;
    }
    else
    {
        return wolkabout::LogLevel::ERROR;
    }
}
}    // namespace wolkabout
