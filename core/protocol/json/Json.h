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

#ifndef JSON_H
#define JSON_H

namespace wolkabout
{
const std::string CHANNEL_DELIMITER = "/";
const std::string CHANNEL_MULTI_LEVEL_WILDCARD = "#";
const std::string CHANNEL_SINGLE_LEVEL_WILDCARD = "+";

const std::string GATEWAY_PATH_PREFIX = "g/";
const std::string DEVICE_PATH_PREFIX = "d/";
const std::string REFERENCE_PATH_PREFIX = "r/";
const std::string DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string PLATFORM_TO_DEVICE_DIRECTION = "p2d/";
}    // namespace wolkabout

#endif    // JSON_H
