/*
* Copyright 2019 WolkAbout Technology s.r.o.
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

#include "core/protocol/Protocol.h"

#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class Message;

class DataProtocol : public Protocol
{
public:
   virtual std::string extractReferenceFromChannel(const std::string& topic) const = 0;

   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                FeedRegistrationMessage feedRegistrationMessage) = 0;
   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedRemovalMessage feedRemovalMessage) = 0;
   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedValuesMessage feedValuesMessage) = 0;
   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                PullFeedValuesMessage pullFeedValuesMessage) = 0;

   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                AttributeRegistrationMessage attributeRegistrationMessage) = 0;
   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                ParametersUpdateMessage parametersUpdateMessage) = 0;
   virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                ParametersPullMessage parametersPullMessage) = 0;

   virtual std::unique_ptr<MessageObject> parseInboundMessage(const Message& reference) = 0;
};
}    // namespace wolkabout

#endif    // DATAPROTOCOL_H