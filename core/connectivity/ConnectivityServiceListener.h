/**
 * Copyright 2022 Wolkabout Technology s.r.o.
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

#ifndef WOLKABOUTCORE_CONNECTIVITYSERVICELISTENER_H
#define WOLKABOUTCORE_CONNECTIVITYSERVICELISTENER_H

#include <string>
#include <vector>

namespace wolkabout
{
class ConnectivityServiceListener
{
public:
    virtual ~ConnectivityServiceListener() = default;

    virtual void messageReceived(const std::string& topic, const std::string& message) = 0;

    virtual void connectionLost() = 0;

    virtual std::vector<std::string> getChannels() const = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_CONNECTIVITYSERVICELISTENER_H
