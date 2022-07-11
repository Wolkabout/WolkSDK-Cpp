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

#ifndef WOLKABOUTCORE_DEVICE_H
#define WOLKABOUTCORE_DEVICE_H

#include "core/Types.h"
#include "core/model/Attribute.h"
#include "core/model/Feed.h"

#include <map>
#include <vector>

namespace wolkabout
{
/**
 * This class represents a device model that should be used to communicate with the platform.
 * This gives context, allows the connection to be established, and gives context to the data being sent out.
 */
class Device
{
public:
    /**
     * Default constructor for this model class.
     *
     * @param key The device key, handed out by the platform when the device was created.
     * @param password The device password, also handed out by the platform.
     * @param type The type of the device, set by the user when the device was created.
     * @param name The name of the device set on the platform. Optional.
     */
    Device(std::string key, std::string password, OutboundDataMode type, std::string name = "");

    /**
     * Default getter for the name of the device.
     *
     * @return The name of the device as a string.
     */
    const std::string& getName() const;

    /**
     * Default setter for the name of the device.
     *
     * @param name The new name of the device as a string.
     */
    void setName(const std::string& name);

    /**
     * Default getter for the device key.
     *
     * @return The device key as a string.
     */
    const std::string& getKey() const;

    /**
     * Default getter for the device password.
     *
     * @return The device password as a string.
     */
    const std::string& getPassword() const;

    /**
     * Default getter for the outbound data mode of the device.
     *
     * @return The mode as an enum value.
     */
    OutboundDataMode getType() const;

    /**
     * Default getter for the list of feeds of the device.
     *
     * @return The list of feeds as a vector of Feed objects.
     */
    const std::vector<Feed>& getFeeds() const;

    /**
     * Default setter for the list of feeds of the device.
     * This will completely override the list of currently set device feeds.
     *
     * @param feeds The new list of feeds as a vector of Feed objects.
     */
    void setFeeds(const std::vector<Feed>& feeds);

    /**
     * Default getter for the list of attributes of the device.
     *
     * @return The list of attributes as a vector of Attribute objects.
     */
    const std::vector<Attribute>& getAttributes() const;

    /**
     * Default setter for the list of attributes of the device.
     * This will completely override the list of currently set device attributes.
     *
     * @param attributes The new list of attributes as a vector of Attribute objects.
     */
    void setAttributes(const std::vector<Attribute>& attributes);

    /**
     * Default getter for the list of parameters of the device.
     *
     * @return The map of parameters and their values, with ParameterName enum values as keys.
     */
    const std::map<ParameterName, std::string>& getParameters() const;

    /**
     * Default setter for the list of parameters of the device.
     * This will completely override the list of currently set device parameters.
     *
     * @param parameters The new map of parameters and their values, with ParameterName enum values as keys.
     */
    void setParameters(const std::map<ParameterName, std::string>& parameters);

private:
    std::string m_name;
    std::string m_key;
    std::string m_password;
    OutboundDataMode m_type;

    std::vector<Feed> m_feeds;
    std::vector<Attribute> m_attributes;
    std::map<ParameterName, std::string> m_parameters;
};
}    // namespace wolkabout
#endif    // WOLKABOUTCORE_DEVICE_H
