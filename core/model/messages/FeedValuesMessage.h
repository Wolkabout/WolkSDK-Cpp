/**
 * Copyright 2021 Wolkabout s.r.o.
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

#ifndef WOLKABOUTCORE_FEEDVALUESMESSAGE_H
#define WOLKABOUTCORE_FEEDVALUESMESSAGE_H

#include "core/model/Reading.h"
#include "core/model/messages/MessageObject.h"

#include <map>
#include <memory>
#include <vector>

namespace wolkabout
{
class FeedValuesMessage : public MessageObject
{
public:
    explicit FeedValuesMessage(const std::vector<Reading>& readings);

    explicit FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings);

    MessageType getMessageType() override;

    const std::map<std::uint64_t, std::vector<Reading>>& getReadings() const;

private:
    std::map<std::uint64_t, std::vector<Reading>> m_readings;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FEEDVALUESMESSAGE_H
