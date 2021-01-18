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

#ifndef ALARMTEMPLATE_H
#define ALARMTEMPLATE_H

#include <string>

namespace wolkabout
{
class AlarmTemplate
{
public:
    AlarmTemplate() = default;
    AlarmTemplate(std::string name, std::string reference, std::string description);

    const std::string& getName() const;
    AlarmTemplate& setName(const std::string& name);

    const std::string& getReference() const;
    AlarmTemplate& setReference(const std::string& reference);

    const std::string& getDescription() const;
    AlarmTemplate& setDescription(const std::string& description);

    bool operator==(const AlarmTemplate& rhs) const;
    bool operator!=(const AlarmTemplate& rhs) const;

private:
    std::string m_name;
    std::string m_reference;
    std::string m_description;
};
}    // namespace wolkabout

#endif    // ALARMTEMPLATE_H
