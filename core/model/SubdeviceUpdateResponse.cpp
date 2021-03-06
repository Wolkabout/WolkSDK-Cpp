/*
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

#include "SubdeviceUpdateResponse.h"

#include <utility>

namespace wolkabout
{
SubdeviceUpdateResponse::SubdeviceUpdateResponse(std::string subdeviceKey, PlatformResult result)
: m_subdeviceKey{std::move(subdeviceKey)}, m_result{std::move(result)}
{
}

const PlatformResult& SubdeviceUpdateResponse::getResult() const
{
    return m_result;
}

const std::string& SubdeviceUpdateResponse::getSubdeviceKey() const
{
    return m_subdeviceKey;
}
}    // namespace wolkabout
