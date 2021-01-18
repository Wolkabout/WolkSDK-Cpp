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

#ifndef JSONPROTOCOL_H
#define JSONPROTOCOL_H

#include "protocol/DataProtocol.h"

#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class JsonProtocol : public DataProtocol
{
public:
    JsonProtocol(bool isGateway = false);

    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    bool isActuatorSetMessage(const Message& message) const override;
    bool isConfigurationSetMessage(const Message& message) const override;

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

    std::string extractReferenceFromChannel(const std::string& topic) const override;
    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;

private:
    bool m_isGateway;

    std::string joinMultiValues(const std::vector<std::string>& values, const std::string& delimiter) const;
    std::vector<std::string> parseMultiValues(const std::string& values, const std::string& delimiter) const;

    static const std::string SENSOR_READING_TOPIC_ROOT;
    static const std::string EVENT_TOPIC_ROOT;
    static const std::string ACTUATION_STATUS_TOPIC_ROOT;
    static const std::string CONFIGURATION_STATUS_TOPIC_ROOT;

    static const std::string ACTUATION_SET_TOPIC_ROOT;
    static const std::string CONFIGURATION_SET_TOPIC_ROOT;

    static const std::string MULTIVALUE_READING_DELIMITER;
};
}    // namespace wolkabout

#endif
