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

#include "SensorReading.h"

#include <string>
#include <utility>

namespace wolkabout
{
SensorReading::SensorReading() : Reading({""}, "") {}

SensorReading::SensorReading(std::string value, std::string reference, unsigned long long int rtc, std::string key)
: Reading({std::move(value)}, std::move(reference), rtc, key)
{
}

SensorReading::SensorReading(std::vector<std::string> values, std::string reference, unsigned long long int rtc,
                             std::string key)
: Reading(std::move(values), std::move(reference), rtc, key)
{
}

void SensorReading::acceptVisit(ReadingVisitor& visitor)
{
    visitor.visit(*this);
}
}    // namespace wolkabout
