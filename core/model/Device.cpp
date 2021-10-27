#include "Device.h"

namespace wolkabout
{
const std::string& wolkabout::Device::getName() const
{
    return m_name;
}
void wolkabout::Device::setName(const std::string& mName)
{
    m_name = mName;
}
const std::string& wolkabout::Device::getKey() const
{
    return m_key;
}
void wolkabout::Device::setKey(const std::string& mKey)
{
    m_key = mKey;
}
const std::string& wolkabout::Device::getPassword() const
{
    return m_password;
}
void wolkabout::Device::setPassword(const std::string& mPassword)
{
    m_password = mPassword;
}
const std::vector<Feed>& wolkabout::Device::getFeeds() const
{
    return m_feeds;
}
void wolkabout::Device::setFeeds(const std::vector<Feed>& mFeeds)
{
    m_feeds = mFeeds;
}
const std::vector<Attribute>& wolkabout::Device::getAttributes() const
{
    return m_attributes;
}
void wolkabout::Device::setAttributes(const std::vector<Attribute>& mAttributes)
{
    m_attributes = mAttributes;
}
const std::map<ParameterName, std::string>& wolkabout::Device::getParameters() const
{
    return m_parameters;
}
void wolkabout::Device::setParameters(const std::map<ParameterName, std::string>& mParameters)
{
    m_parameters = mParameters;
}
wolkabout::Device::Device(std::string key, std::string password, std::shared_ptr<DeviceTemplate> deviceTemplate)
: m_key(std::move(key)), m_password(std::move(password)), m_name("")
{
    // TODO implement reading from device template file
}
wolkabout::Device::Device(std::string name, std::string key, std::string password,
                          std::shared_ptr<DeviceTemplate> deviceTemplate)
: m_name(std::move(name)), m_key(std::move(key)), m_password(std::move(password))
{
    // TODO implement reading from device template file
}
} // !namespace wolkabout
