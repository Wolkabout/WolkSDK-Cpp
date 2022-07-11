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

#ifndef DATAPROTOCOL_H
#define DATAPROTOCOL_H

#include "core/model/messages/AttributeRegistrationMessage.h"
#include "core/model/messages/DetailsSynchronizationRequestMessage.h"
#include "core/model/messages/DetailsSynchronizationResponseMessage.h"
#include "core/model/messages/FeedRegistrationMessage.h"
#include "core/model/messages/FeedRemovalMessage.h"
#include "core/model/messages/FeedValuesMessage.h"
#include "core/model/messages/ParametersPullMessage.h"
#include "core/model/messages/ParametersUpdateMessage.h"
#include "core/model/messages/PullFeedValuesMessage.h"
#include "core/model/messages/SynchronizeParametersMessage.h"
#include "core/protocol/Protocol.h"

#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
/**
 * This is an interface defining a DataProtocol, used to transfer basic device data.
 * This protocol defines the serializers/deserializers for a bunch of messages that are necessary to accomplish
 * the basic device data communication.
 */
class DataProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a FeedRegistrationMessage.
     *
     * @param deviceKey The device key for which the FeedRegistrationMessage is regarding.
     * @param feedRegistrationMessage The message containing information about feeds that need to be registered.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         FeedRegistrationMessage feedRegistrationMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a FeedRemovalMessage.
     *
     * @param deviceKey The device key for which the FeedRemovalMessage is regarding.
     * @param feedRemovalMessage The message containing information about feeds that need to be removed.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         FeedRemovalMessage feedRemovalMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a FeedValuesMessage.
     *
     * @param deviceKey The device key for which the FeedValuesMessage is regarding.
     * @param feedValuesMessage The message containing information about new feed values the device is sending.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         FeedValuesMessage feedValuesMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a PullFeedValuesMessage.
     *
     * @param deviceKey The device key for which the PullFeedValuesMessage is regarding.
     * @param pullFeedValuesMessage The request message for new feed values from the platform.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         PullFeedValuesMessage pullFeedValuesMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a AttributeRegistrationMessage.
     *
     * @param deviceKey The device key for which the AttributeRegistrationMessage is regarding.
     * @param attributeRegistrationMessage The request message about new/updated attributes the device is sending.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         AttributeRegistrationMessage attributeRegistrationMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a ParametersUpdateMessage.
     *
     * @param deviceKey The device key for which the ParametersUpdateMessage is regarding.
     * @param parametersUpdateMessage The request message about updated parameters the device is sending.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         ParametersUpdateMessage parametersUpdateMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a ParametersPullMessage.
     *
     * @param deviceKey The device key for which the ParametersPullMessage is regarding.
     * @param pullFeedValuesMessage The request message for new parameter values from the platform.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         ParametersPullMessage parametersPullMessage) = 0;

    /**
     * This method is a serialization method to create send-able MQTT message from a SynchronizeParametersMessage.
     *
     * @param deviceKey The device key for which the SynchronizeParametersMessage is regarding.
     * @param synchronizeParametersMessage The request message for which parameters need to be synchronized.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         SynchronizeParametersMessage synchronizeParametersMessage) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from
     * DetailsSynchronizationRequestMessage.
     *
     * @param deviceKey The device key for which the DetailsSynchronizationRequestMessage is regarding.
     * @param detailsSynchronizationRequestMessage The request message requesting details.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(
      const std::string& deviceKey, DetailsSynchronizationRequestMessage detailsSynchronizationRequestMessage) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FeedValuesMessage.
     *
     * @param message The received MQTT message that is potentially a valid FeedValuesMessage.
     * @return A parsed FeedValuesMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FeedValuesMessage> parseFeedValues(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a ParametersUpdateMessage.
     *
     * @param message The received MQTT message that is potentially a valid ParametersUpdateMessage.
     * @return A parsed ParametersUpdateMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<ParametersUpdateMessage> parseParameters(std::shared_ptr<Message> message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a
     * DetailsSynchronizationResponseMessage.
     *
     * @param message The received MQTT message that is potentially a valid DetailsSynchronizationResponseMessage.
     * @return A parsed DetailsSynchronizationResponseMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<DetailsSynchronizationResponseMessage> parseDetails(std::shared_ptr<Message> message) = 0;
};
}    // namespace wolkabout

#endif    // DATAPROTOCOL_H
