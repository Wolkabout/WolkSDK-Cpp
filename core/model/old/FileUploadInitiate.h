/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#ifndef FILEUPLOADINITIATE_H
#define FILEUPLOADINITIATE_H

#include <cstdint>
#include <string>

namespace wolkabout
{
class FileUploadInitiate
{
public:
    FileUploadInitiate(std::string name, std::uint64_t size, std::string hash);

    const std::string& getName() const;
    std::uint64_t getSize() const;
    const std::string& getHash() const;

private:
    std::string m_name;
    std::uint64_t m_size;
    std::string m_hash;
};
}    // namespace wolkabout

#endif    // FILEUPLOADINITIATE_H
