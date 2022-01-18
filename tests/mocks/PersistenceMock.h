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

#ifndef WOLKABOUTCONNECTOR_PERSISTENCEMOCK_H
#define WOLKABOUTCONNECTOR_PERSISTENCEMOCK_H

#include "core/persistence/Persistence.h"

#include <gmock/gmock.h>

using namespace wolkabout;

class PersistenceMock : public Persistence
{
public:
    MOCK_METHOD(bool, putReading, (const std::string&, const Reading&));
    MOCK_METHOD(std::vector<std::shared_ptr<Reading>>, getReadings, (const std::string&, std::uint_fast64_t));
    MOCK_METHOD(void, removeReadings, (const std::string&, std::uint_fast64_t));
    MOCK_METHOD(std::vector<std::string>, getReadingsKeys, ());
    MOCK_METHOD(bool, putAttribute, (const std::string&, std::shared_ptr<Attribute>));
    MOCK_METHOD((std::map<std::string, std::shared_ptr<Attribute>>), getAttributes, ());
    MOCK_METHOD(std::shared_ptr<Attribute>, getAttributeUnderKey, (const std::string&));
    MOCK_METHOD(void, removeAttributes, ());
    MOCK_METHOD(void, removeAttributes, (const std::string&));
    MOCK_METHOD(std::vector<std::string>, getAttributeKeys, ());
    MOCK_METHOD(bool, putParameter, (const std::string&, Parameter));
    MOCK_METHOD((std::map<std::string, Parameter>), getParameters, ());
    MOCK_METHOD(Parameter, getParameterForKey, (const std::string&));
    MOCK_METHOD(void, removeParameters, ());
    MOCK_METHOD(void, removeParameters, (const std::string&));
    MOCK_METHOD(std::vector<std::string>, getParameterKeys, ());
    MOCK_METHOD(bool, isEmpty, ());
};

#endif    // WOLKABOUTCONNECTOR_PERSISTENCEMOCK_H
