/**
 * Copyright 2023 Wolkabout Technology s.r.o.
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

#include "core/protocol/wolkabout/WolkaboutProtocol.h"

#include <nlohmann/json-schema.hpp>

using nlohmann::json;
using nlohmann::json_schema::json_validator;

namespace wolkabout
{
const json feed_values_schema = R"(
{
	"$schema": "https://json-schema.org/draft/2019-09/schema",
    "type": "array",
    "items": {
      "type": "object",
      "properties": {
        "timestamp": {
          "type": "number"
        }
      },
      "required": [
        "timestamp"
      ]
    }
}
)"_json;

const json parameters_schema = R"(
{
	"$schema": "https://json-schema.org/draft/2019-09/schema",
    "type": "object",
    "propertyNames": {
        "pattern": "CONNECTIVITY_TYPE|OUTBOUND_DATA_MODE|OUTBOUND_DATA_RETENTION_TIME|MAXIMUM_MESSAGE_SIZE|FILE_TRANSFER_PLATFORM_ENABLED|FILE_TRANSFER_URL_ENABLED|FIRMWARE_UPDATE_ENABLED|FIRMWARE_UPDATE_CHECK_TIME|FIRMWARE_UPDATE_REPOSITORY|FIRMWARE_VERSION|GATEWAY|GATEWAY_PARENT|EXTERNAL_ID|UNKNOWN"
    }
}
)"_json;

const json details_synchronization_schema = R"(
{
	"$schema": "https://json-schema.org/draft/2019-09/schema",
    "type": "object",
	"properties": {
    	"feeds": {
          "type": "array",
          "items": {
            "type": "string",
            "pattern": "^(?!\\s*$).+"
          }
        },
        "attributes": {
          "type": "array",
          "items": {
            "type": "string",
            "pattern": "^(?!\\s*$).+"
          }
        }
    },
    "required": [
      "feeds", "attributes"
    ]
}
)"_json;

const json file_upload_initiate_schema = R"(
{
	"$schema": "https://json-schema.org/draft/2019-09/schema",
    "type": "object",
    "properties": {
      "name": {
        "type": "string"
      },
      "size": {
        "type": "number"
      },
      "hash": {
        "type": "string",
        "pattern": "[0-9A-Fa-f]{32}"
      }
    },
    "required": [
      "name", "size", "hash"
    ]
}
)"_json;

const json string_array_schema = R"(
{
	"$schema": "https://json-schema.org/draft/2019-09/schema",
    "type": "array",
    "items": {
        "type": "string",
        "pattern": "^(?!\\s*$).+"
    }
}
)"_json;
const json file_delete_schema = string_array_schema;

const json children_synchronization_schema = string_array_schema;

const json registered_devices_schema = R"(
{
	"$schema": "https://json-schema.org/draft/2019-09/schema",
    "type": "object",
    "properties": {
      "timestampFrom": {
        "type": "number"
      },
      "deviceType": {
        "type": ["string", "null"]
      },
      "externalId": {
        "type": ["string", "null"]
      },
      "matchingDevices": {
        "type": "array",
        "items": {
          "type": "object",
          "properties": {
            "deviceKey": {
              "type": "string",
              "pattern": "^(?!\\s*$).+"
            },
            "externalId": {
              "type": ["string", "null"]
            },
            "deviceType": {
              "type": ["string", "null"]
            }
          },
          "required": [
            "deviceKey"
          ]
        }
      }
    },
    "required": [
      "timestampFrom", "matchingDevices"
    ]
}
)"_json;

bool WolkaboutProtocol::checkThatObjectContainsKeys(const nlohmann::json& j, const std::vector<std::string>& keys)
{
    return std::all_of(keys.cbegin(), keys.cend(), [&](const std::string& key) { return j.find(key) != j.cend(); });
}

std::string WolkaboutProtocol::removeQuotes(std::string value)
{
    // Make place for the iterator in the string
    auto index = value.find(ESCAPED_QUOTES);
    while (index != std::string::npos)
    {
        value.replace(index, ESCAPED_QUOTES.size(), "");
        index = value.find(ESCAPED_QUOTES);
    }
    return value;
}

MessageType WolkaboutProtocol::getMessageType(const Message& message)
{
    // Take the topic, and extract its last part
    const auto& topic = message.getChannel();
    const auto section = topic.substr(topic.rfind(CHANNEL_DELIMITER) + 1);
    return messageTypeFromString(section);
}

DeviceType WolkaboutProtocol::getDeviceType(const Message& message)
{
    // Take the topic, and extract its first part
    const auto& topic = message.getChannel();
    const auto section = topic.substr(0, topic.find(CHANNEL_DELIMITER));
    if (section == GATEWAY_TO_PLATFORM_DIRECTION || section == PLATFORM_TO_GATEWAY_DIRECTION)
        return DeviceType::GATEWAY;
    else if (section == DEVICE_TO_PLATFORM_DIRECTION || section == PLATFORM_TO_DEVICE_DIRECTION)
        return DeviceType::STANDALONE;
    return DeviceType::NONE;
}

std::string WolkaboutProtocol::getDeviceKey(const Message& message)
{
    // Substring between the two channel delimiters
    const auto& topic = message.getChannel();
    const auto firstDivider = topic.find(CHANNEL_DELIMITER);
    const auto lastDivider = topic.rfind(CHANNEL_DELIMITER);
    return topic.substr(firstDivider + 1, lastDivider - firstDivider - 1);
}

bool WolkaboutProtocol::validateJSONPayload(const Message& message)
{
    // Create the validator, and set the proper scheme
    static auto validator = json_validator{};
    switch (getMessageType(message))
    {
    case MessageType::FEED_VALUES:
        validator.set_root_schema(feed_values_schema);
        break;
    case MessageType::PARAMETER_SYNC:
    case MessageType::SYNCHRONIZE_PARAMETERS:
        validator.set_root_schema(parameters_schema);
        break;
    case MessageType::DETAILS_SYNCHRONIZATION_RESPONSE:
        validator.set_root_schema(details_synchronization_schema);
        break;
    case MessageType::FILE_UPLOAD_INIT:
        validator.set_root_schema(file_upload_initiate_schema);
        break;
    case MessageType::FILE_DELETE:
        validator.set_root_schema(file_delete_schema);
        break;
    case MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE:
        validator.set_root_schema(children_synchronization_schema);
        break;
    case MessageType::REGISTERED_DEVICES_RESPONSE:
        validator.set_root_schema(registered_devices_schema);
        break;
    default:
        return false;
    }

    // Validate the message
    validator.validate(json::parse(message.getContent()));
    return true;
}

std::string WolkaboutProtocol::CHANNEL_DELIMITER = "/";
std::string WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION = "d2p";
std::string WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION = "p2d";
std::string WolkaboutProtocol::GATEWAY_TO_PLATFORM_DIRECTION = "g2p";
std::string WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION = "p2g";
std::string WolkaboutProtocol::TIMESTAMP_KEY = "timestamp";
std::string WolkaboutProtocol::ESCAPED_QUOTES = "\"";
std::string WolkaboutProtocol::WILDCARD_SINGLE_LEVEL = "+";
}    // namespace wolkabout
