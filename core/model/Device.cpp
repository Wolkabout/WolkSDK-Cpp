#include "Device.h"

namespace wolkabout
{
const std::string& wolkabout::Device::getName() const
{
    return m_name;
}
void wolkabout::Device::setName(const std::string& name)
{
    m_name = name;
}
const std::string& wolkabout::Device::getKey() const
{
    return m_key;
}
void wolkabout::Device::setKey(const std::string& key)
{
    m_key = key;
}
const std::string& wolkabout::Device::getPassword() const
{
    return m_password;
}
void wolkabout::Device::setPassword(const std::string& password)
{
    m_password = password;
}
const std::vector<Feed>& wolkabout::Device::getFeeds() const
{
    return m_feeds;
}
void wolkabout::Device::setFeeds(const std::vector<Feed>& feeds)
{
    m_feeds = feeds;
}
const std::vector<Attribute>& wolkabout::Device::getAttributes() const
{
    return m_attributes;
}
void wolkabout::Device::setAttributes(const std::vector<Attribute>& attributes)
{
    m_attributes = attributes;
}
const std::map<ParameterName, std::string>& wolkabout::Device::getParameters() const
{
    return m_parameters;
}
void wolkabout::Device::setParameters(const std::map<ParameterName, std::string>& parameters)
{
    m_parameters = parameters;
}
OutboundDataMode Device::getType() const
{
    return m_type;
}
void Device::setType(OutboundDataMode type)
{
    m_type = type;
}
wolkabout::Device::Device(std::string key, std::string password, OutboundDataMode type, std::shared_ptr<DeviceTemplate> deviceTemplate)
: m_name("")
, m_key(std::move(key))
, m_password(std::move(password))
, m_type(type)
{
    // TODO implement reading from device template file
}
wolkabout::Device::Device(std::string name, std::string key, std::string password,
                          OutboundDataMode type, std::shared_ptr<DeviceTemplate> deviceTemplate)
: m_name(std::move(name)), m_key(std::move(key)), m_password(std::move(password)), m_type(type)
{
    // TODO implement reading from device template file
}
} // !namespace wolkabout
