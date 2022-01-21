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

#ifndef CIRCULARFILESYSTEMMESSAGEPERSISTENCE_H
#define CIRCULARFILESYSTEMMESSAGEPERSISTENCE_H

#include "core/persistence/filesystem/FileSystemMessagePersistence.h"

namespace wolkabout
{
/**
 * @brief The CircularFileSystemMessagePersistence class
 * Specialization of FileSystemMessagePersistence for limited storage
 */
class CircularFileSystemMessagePersistence : public FileSystemMessagePersistence
{
public:
    explicit CircularFileSystemMessagePersistence(const std::string& persistPath, PersistenceMethod method,
                                                  unsigned sizeLimitBytes = 0);

    bool push(std::shared_ptr<Message> message) override;
    void pop() override;

    void setSizeLimit(unsigned bytes);

private:
    void loadFileSize();
    void checkSizeAndNormalize();

    unsigned m_sizeLimitBytes;
    unsigned long long m_totalFileSize = 0;
};
}    // namespace wolkabout

#endif    // CIRCULARFILESYSTEMMESSAGEPERSISTENCE_H
