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

#include "model/FileUploadInitiate.h"

#include <utility>

namespace wolkabout
{
FileUploadInitiate::FileUploadInitiate(std::string name, std::uint64_t size, std::string hash)
: m_name{std::move(name)}, m_size{size}, m_hash{std::move(hash)}
{
}

const std::string& FileUploadInitiate::getName() const
{
    return m_name;
}

std::uint64_t FileUploadInitiate::getSize() const
{
    return m_size;
}

const std::string& FileUploadInitiate::getHash() const
{
    return m_hash;
}

}    // namespace wolkabout
