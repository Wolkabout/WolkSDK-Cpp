#ifndef WOLKABOUTCORE_DEVICE_H
#define WOLKABOUTCORE_DEVICE_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Attribute.h"
#include "Feed.h"
#include "core/Types.h"

namespace wolkabout
{

class DeviceTemplate;

class Device
{
private:
    std::string m_name;
    std::string m_key;
    std::string m_password;
    OutboundDataMode m_type;

    std::vector<Feed> m_feeds;
    std::vector<Attribute> m_attributes;
    std::map<ParameterName, std::string> m_parameters;

public:
    Device(std::string key, std::string password, OutboundDataMode type, std::shared_ptr<DeviceTemplate> deviceTemplate = nullptr);
    Device(std::string name, std::string key, std::string password, OutboundDataMode type, std::shared_ptr<DeviceTemplate> deviceTemplate = nullptr);
    const std::string& getName() const;
    void setName(const std::string& name);
    const std::string& getKey() const;
    void setKey(const std::string& key);
    const std::string& getPassword() const;
    void setPassword(const std::string& password);
    const std::vector<Feed>& getFeeds() const;
    void setFeeds(const std::vector<Feed>& feeds);
    const std::vector<Attribute>& getAttributes() const;
    void setAttributes(const std::vector<Attribute>& attributes);
    const std::map<ParameterName, std::string>& getParameters() const;
    void setParameters(const std::map<ParameterName, std::string>& parameters);
    OutboundDataMode getType() const;
    void setType(OutboundDataMode type);
};
} // !namespace wolkabout
#endif    // WOLKABOUTCORE_DEVICE_H
