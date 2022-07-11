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

#ifndef WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H

#include "core/model/Message.h"
#include "core/protocol/DataProtocol.h"

#include <string>

namespace wolkabout
{
/**
 * This is the current implementation of the Data Protocol section of the Wolkabout protocol.
 * The implementation uses `nlohmann::json`, the `<DIRECTION>/<DEVICE_KEY>/<MESSAGE_TYPE>` paradigm,
 * and only feeds, attributes and parameters.
 */
class WolkaboutDataProtocol : public DataProtocol
{
public:
    std::vector<std::string> getInboundChannels() const override;

    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    std::string getDeviceKey(const Message& message) const override;

    DeviceType getDeviceType(const Message& message) override;

    MessageType getMessageType(const Message& message) override;

    std::string getResponseChannelForMessage(MessageType type, const std::string& deviceKey) const override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 FeedRegistrationMessage feedRegistrationMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 FeedRemovalMessage feedRemovalMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 FeedValuesMessage feedValuesMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 PullFeedValuesMessage pullFeedValuesMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 AttributeRegistrationMessage attributeRegistrationMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 ParametersUpdateMessage parametersUpdateMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 ParametersPullMessage parametersPullMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 SynchronizeParametersMessage synchronizeParametersMessage) override;

    std::unique_ptr<Message> makeOutboundMessage(
      const std::string& deviceKey, DetailsSynchronizationRequestMessage detailsSynchronizationRequestMessage) override;

    std::unique_ptr<FeedValuesMessage> parseFeedValues(std::shared_ptr<Message> message) override;

    std::unique_ptr<ParametersUpdateMessage> parseParameters(std::shared_ptr<Message> message) override;

    std::unique_ptr<DetailsSynchronizationResponseMessage> parseDetails(std::shared_ptr<Message> message) override;

private:
    static std::string getFeedTopic(const std::string& deviceKey);

    static std::string getParametersTopic(const std::string& deviceKey);

    static std::string getDetailsSynchronizationTopic(const std::string& deviceKey);
};
}    // namespace wolkabout
#endif    // WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
