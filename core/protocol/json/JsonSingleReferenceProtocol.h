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
#ifndef JSONSINGLEREFERENCEPROTOCOL_H
#define JSONSINGLEREFERENCEPROTOCOL_H

#include "protocol/DataProtocol.h"

namespace wolkabout
{
class JsonSingleReferenceProtocol : public DataProtocol
{
public:
    const std::string& getName() const override;
    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    bool isActuatorSetMessage(const Message& message) const override;
    bool isActuatorGetMessage(const Message& message) const override;
    bool isConfigurationSetMessage(const Message& message) const override;
    bool isConfigurationGetMessage(const Message& message) const override;

    std::unique_ptr<ActuatorGetCommand> makeActuatorGetCommand(const Message& message) const override;
    std::unique_ptr<ActuatorSetCommand> makeActuatorSetCommand(const Message& message) const override;
    std::unique_ptr<ConfigurationSetCommand> makeConfigurationSetCommand(const Message& message) const override;

    std::unique_ptr<Message> makeMessage(
      const std::string& deviceKey, const std::vector<std::shared_ptr<SensorReading>>& sensorReadings) const override;
    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const std::vector<std::shared_ptr<Alarm>>& alarms) const override;
    std::unique_ptr<Message> makeMessage(
      const std::string& deviceKey,
      const std::vector<std::shared_ptr<ActuatorStatus>>& actuatorStatuses) const override;
    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const std::vector<ConfigurationItem>& configuration) const override;

    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;
    std::string extractReferenceFromChannel(const std::string& topic) const override;

private:
    std::string joinMultiValues(const std::vector<std::string>& values, const std::string& delimiter) const;
    std::vector<std::string> parseMultiValues(const std::string& values, const std::string& delimiter) const;

    static const std::string NAME;

    static const std::string CHANNEL_DELIMITER;
    static const std::string CHANNEL_WILDCARD;

    static const std::string SENSOR_READING_TOPIC_ROOT;
    static const std::string ALARMS_TOPIC_ROOT;
    static const std::string ACTUATION_STATUS_TOPIC_ROOT;
    static const std::string CONFIGURATION_RESPONSE_TOPIC_ROOT;

    static const std::string ACTUATION_COMMAND_TOPIC_ROOT;
    static const std::string CONFIGURATION_COMMAND_TOPIC_ROOT;

    static const std::vector<std::string> INBOUND_CHANNELS;

    static const std::string MULTIVALUE_READING_DELIMITER;
};
}    // namespace wolkabout

#endif    // JSONSINGLEREFERENCEPROTOCOL_H
