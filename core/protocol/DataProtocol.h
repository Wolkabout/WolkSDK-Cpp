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

#ifndef DATAPROTOCOL_H
#define DATAPROTOCOL_H

#include "protocol/Protocol.h"

#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class Message;
class ActuatorSetCommand;
class ActuatorGetCommand;
class ActuatorStatus;
class Alarm;
class ConfigurationSetCommand;
class ConfigurationItem;
class SensorReading;

class DataProtocol : public Protocol
{
public:
    virtual std::string extractReferenceFromChannel(const std::string& topic) const = 0;

    virtual bool isActuatorSetMessage(const Message& message) const = 0;
    virtual bool isActuatorGetMessage(const Message& message) const = 0;

    virtual bool isConfigurationSetMessage(const Message& message) const = 0;
    virtual bool isConfigurationGetMessage(const Message& message) const = 0;

    virtual std::unique_ptr<ActuatorGetCommand> makeActuatorGetCommand(const Message& message) const = 0;
    virtual std::unique_ptr<ActuatorSetCommand> makeActuatorSetCommand(const Message& message) const = 0;

    virtual std::unique_ptr<ConfigurationSetCommand> makeConfigurationSetCommand(const Message& message) const = 0;

    virtual std::unique_ptr<Message> makeMessage(
      const std::string& deviceKey, const std::vector<std::shared_ptr<SensorReading>>& sensorReadings) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const std::vector<std::shared_ptr<Alarm>>& alarms) const = 0;

    virtual std::unique_ptr<Message> makeMessage(
      const std::string& deviceKey, const std::vector<std::shared_ptr<ActuatorStatus>>& actuatorStatuses) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const std::vector<ConfigurationItem>& configuration) const = 0;
};
}    // namespace wolkabout

#endif    // DATAPROTOCOL_H
