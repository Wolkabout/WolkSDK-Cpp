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

    std::vector<Feed> m_feeds;
    std::vector<Attribute> m_attributes;
    std::map<ParameterName, std::string> m_parameters;

public:
    Device(std::string key, std::string password, std::shared_ptr<DeviceTemplate> deviceTemplate = nullptr);
    Device(std::string name, std::string key, std::string password, std::shared_ptr<DeviceTemplate> deviceTemplate = nullptr);
};
} // !namespace wolkabout
#endif    // WOLKABOUTCORE_DEVICE_H
