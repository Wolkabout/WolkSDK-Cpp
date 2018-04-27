#ifndef DATAPROTOCOL_H
#define DATAPROTOCOL_H

#include "protocol/Protocol.h"

#include <map>
#include <memory>
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

    virtual std::unique_ptr<ConfigurationSetCommand> makeConfigurationSetCommand(
      const Message& message, const std::map<std::string, std::string>& delimiters = {}) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const std::vector<std::shared_ptr<SensorReading>>& sensorReadings,
                                                 const std::string& delimiter = "") const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const std::vector<std::shared_ptr<Alarm>>& alarms) const = 0;

    virtual std::unique_ptr<Message> makeMessage(
      const std::string& deviceKey, const std::vector<std::shared_ptr<ActuatorStatus>>& actuatorStatuses) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const std::vector<ConfigurationItem>& configuration,
                                                 const std::map<std::string, std::string>& delimiters = {}) const = 0;

    inline Type getType() const override final { return Protocol::Type::DATA; }
};
}    // namespace wolkabout

#endif    // DATAPROTOCOL_H
