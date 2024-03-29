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

#ifndef FILESYSTEMMESSAGEPERSISTENCE_H
#define FILESYSTEMMESSAGEPERSISTENCE_H

#include "core/persistence/MessagePersistence.h"

#include <list>
#include <mutex>
#include <queue>

namespace wolkabout
{
class MessagePersister;

/**
 * @brief The FileSystemMessagePersistence class
 * Persists messages on file system
 */
class FileSystemMessagePersistence : public MessagePersistence
{
public:
    FileSystemMessagePersistence(std::string persistPath, PersistenceMethod method);
    ~FileSystemMessagePersistence() override;

    bool push(std::shared_ptr<Message> message) override;
    void pop() override;
    std::shared_ptr<Message> front() override;
    bool empty() const override;

protected:
    void initialize();

    void saveReading(const std::string& fileName);
    void deleteFirstReading();
    void deleteLastReading();

    std::string firstReading();
    std::string lastReading();
    std::string readingPath(const std::string& readingFileName) const;
    static bool matchFileNumber(const std::string& fileName, unsigned long& number);

    std::string saveToDisk(const std::shared_ptr<Message>& message);

    std::unique_ptr<MessagePersister> m_persister;

    mutable std::recursive_mutex m_mutex;

    const std::string m_persistPath;
    PersistenceMethod m_method;
    std::list<std::string> m_readingFiles;
    unsigned long m_messageNum;
};
}    // namespace wolkabout

#endif
