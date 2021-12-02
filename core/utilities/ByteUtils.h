/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

#ifndef BYTEUTILS_H
#define BYTEUTILS_H

#include <cstdint>
#include <string>
#include <vector>

namespace wolkabout
{
using Byte = std::uint8_t;
using ByteArray = std::vector<Byte>;

class ByteUtils
{
public:
    ByteUtils() = delete;

    static ByteArray toByteArray(const std::string& data);

    static std::string toString(const ByteArray& data);

    static std::string toHexString(const ByteArray& data);

    static ByteArray hashSHA256(const ByteArray& value);

    static ByteArray hashMDA5(const ByteArray& value);
};
}    // namespace wolkabout

#endif    // BYTE_H
