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

#include "core/utilities/ByteUtils.h"

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

namespace wolkabout
{
ByteArray ByteUtils::toByteArray(const std::string& data)
{
    ByteArray array{};

    for (const auto& byte : data)
    {
        array.push_back(static_cast<Byte>(byte));
    }

    return array;
}

std::string ByteUtils::toString(const ByteArray& data)
{
    auto stream = std::stringstream{};

    for (const auto& byte : data)
    {
        stream << byte;
    }

    return stream.str();
}

std::string ByteUtils::toHexString(const ByteArray& data)
{
    auto stream = std::stringstream{};

    for (const auto& byte : data)
    {
        stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<std::int32_t>(byte);
    }

    return stream.str();
}

ByteArray ByteUtils::hashSHA256(const ByteArray& value)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256(&value[0], value.size(), reinterpret_cast<unsigned char*>(&digest));

    ByteArray ret{};

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ret.push_back(digest[i]);
    }

    return ret;
}

ByteArray ByteUtils::hashMDA5(const ByteArray& value)
{
    std::uint8_t hashCStr[MD5_DIGEST_LENGTH];
    auto context = MD5_CTX();
    MD5_Init(&context);
    MD5_Update(&context, value.data(), value.size());
    MD5_Final(hashCStr, &context);
    auto hash = ByteArray{};
    for (auto i = std::uint32_t{0}; i < MD5_DIGEST_LENGTH; ++i)
        hash.emplace_back(hashCStr[i]);
    return hash;
}
}    // namespace wolkabout
