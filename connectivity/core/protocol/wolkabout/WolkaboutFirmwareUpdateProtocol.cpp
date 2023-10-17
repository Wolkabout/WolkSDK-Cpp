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

#include "core/protocol/wolkabout/WolkaboutFirmwareUpdateProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utility/Logger.h"

#include <nlohmann/json.hpp>

using nlohmann::json;
using namespace wolkabout::legacy;

namespace wolkabout
{
std::vector<std::string> WolkaboutFirmwareUpdateProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutFirmwareUpdateProtocol::getInboundChannelsForDevice(
  const std::string& deviceKey) const
{
    return {WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::FIRMWARE_UPDATE_INSTALL),
            WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::FIRMWARE_UPDATE_ABORT)};
}

MessageType WolkaboutFirmwareUpdateProtocol::getMessageType(const Message& message)
{
    return WolkaboutProtocol::getMessageType(message);
}

DeviceType WolkaboutFirmwareUpdateProtocol::getDeviceType(const Message& message)
{
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutFirmwareUpdateProtocol::getDeviceKey(const Message& message) const
{
    return WolkaboutProtocol::getDeviceKey(message);
}

std::string WolkaboutFirmwareUpdateProtocol::getResponseChannelForMessage(MessageType /* type */,
                                                                          const std::string& /* deviceKey */) const
{
    return {};
}

std::unique_ptr<Message> WolkaboutFirmwareUpdateProtocol::makeOutboundMessage(
  const std::string& deviceKey, const FirmwareUpdateStatusMessage& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'FirmwareUpdateStatus' message";

    // Verify that the message is valid
    auto statusString = toString(message.getStatus());
    if (statusString.empty())
    {
        LOG(ERROR) << errorPrefix << " -> The status value is not valid.";
        return nullptr;
    }
    auto errorString = toString(message.getError());
    if (message.getStatus() == FirmwareUpdateStatus::ERROR_UPDATE && errorString.empty())
    {
        LOG(ERROR) << errorPrefix << " -> Missing valid error value.";
        return nullptr;
    }

    // Make the topic for the message
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::FIRMWARE_UPDATE_STATUS);

    try
    {
        // Parse the message into a JSON
        auto payload = nlohmann::json({{"status", statusString}});
        if (message.getStatus() == FirmwareUpdateStatus::ERROR_UPDATE)
            payload["error"] = errorString;
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<FirmwareUpdateInstallMessage> WolkaboutFirmwareUpdateProtocol::parseFirmwareUpdateInstall(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FirmwareUpdateInstall' message";

    // Check that the message is a FirmwareUpdateInstall message.
    auto type = getMessageType(*message);
    if (type != MessageType::FIRMWARE_UPDATE_INSTALL)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FirmwareUpdateInstall' message.";
        return nullptr;
    }

    // Take the payload as the file name
    return std::unique_ptr<FirmwareUpdateInstallMessage>(
      new FirmwareUpdateInstallMessage(WolkaboutProtocol::removeQuotes(message->getContent())));
}

std::unique_ptr<FirmwareUpdateAbortMessage> WolkaboutFirmwareUpdateProtocol::parseFirmwareUpdateAbort(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'FirmwareUpdateAbort' message";

    // Check that the message is a FirmwareUpdateAbort message.
    auto type = getMessageType(*message);
    if (type != MessageType::FIRMWARE_UPDATE_ABORT)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'FirmwareUpdateAbort' message.";
        return nullptr;
    }

    // Take the payload as the file name
    return std::unique_ptr<FirmwareUpdateAbortMessage>(new FirmwareUpdateAbortMessage);
}
}    // namespace wolkabout
