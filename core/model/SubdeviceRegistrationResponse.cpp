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

#include "model/SubdeviceRegistrationResponse.h"

#include <string>
#include <utility>

namespace wolkabout
{
SubdeviceRegistrationResponse::SubdeviceRegistrationResponse(SubdeviceRegistrationResponse::Result result,
                                                             std::string description)
: m_result(std::move(result)), m_description(std::move(description))
{
}

const SubdeviceRegistrationResponse::Result& SubdeviceRegistrationResponse::getResult() const
{
    return m_result;
}

const std::string& SubdeviceRegistrationResponse::getDescription() const
{
    return m_description;
}
}    // namespace wolkabout
