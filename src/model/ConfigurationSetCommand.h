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
#ifndef CONFIGURATIONSETCOMMAND_H
#define CONFIGURATIONSETCOMMAND_H

#include "model/ConfigurationItem.h"

#include <string>
#include <vector>

namespace wolkabout
{
class ConfigurationSetCommand
{
public:
    ConfigurationSetCommand(std::vector<ConfigurationItem> values);

    const std::vector<ConfigurationItem>& getValues() const;

private:
    std::vector<ConfigurationItem> m_values;
};
}    // namespace wolkabout

#endif    // CONFIGURATIONSETCOMMAND_H
