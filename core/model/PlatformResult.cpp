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

#include "model/PlatformResult.h"

namespace wolkabout
{
std::string to_string(PlatformResult result)
{
    switch (result)
    {
    case PlatformResult::OK:
    {
        return "OK";
    }
    case PlatformResult::ERROR_GATEWAY_NOT_FOUND:
    {
        return "ERROR_GATEWAY_NOT_FOUND";
    }
    case PlatformResult::ERROR_KEY_MISSING:
    {
        return "ERROR_KEY_MISSING";
    }
    case PlatformResult::ERROR_NOT_A_GATEWAY:
    {
        return "ERROR_NOT_A_GATEWAY";
    }
    case PlatformResult::ERROR_DEVICE_NOT_FOUND:
    {
        return "ERROR_DEVICE_NOT_FOUND";
    }
    case PlatformResult::ERROR_INVALID_DEVICE:
    {
        return "ERROR_INVALID_DEVICE";
    }
    }

    return "";
}
}    // namespace wolkabout