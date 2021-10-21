/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "core/Types.h"
#include "Feed.h"
#include "Attribute.h"

namespace wolkabout
{
class Message
{
public:
    virtual ~Message() = default;
    virtual const MessageType& getMessageType() = 0;
    virtual const std::string getChannel() = 0;
};

class FeedValuesMessage: public Message
{
private:
    std::map<unsigned long long int, std::vector<Reading>> m_readings;
    MessageType m_messageType;

public:
    FeedValuesMessage(std::vector<Reading> readings);
    const MessageType& getMessageType() override;
    const std::string getChannel() override;
    const std::map<unsigned long long int, std::vector<Reading>>& getReadings() const;
};

class PullFeedValuesMessage: public Message
{
private:
    MessageType m_messageType;
public:
    PullFeedValuesMessage();
    const MessageType& getMessageType() override;
    const std::string getChannel() override;
};

class FeedRegistrationMessage: public Message
{
private:
    MessageType m_messageType;
    std::vector<Feed> m_feeds;
public:
    FeedRegistrationMessage(std::vector<Feed> feeds);
    const MessageType& getMessageType() override;
    const std::string getChannel() override;
    const std::vector<Feed>& getFeeds() const;
};

class FeedRemovalMessage: public Message
{
private:
    MessageType m_messageType;
    std::vector<std::string> m_references;
public:
    FeedRemovalMessage(std::vector<std::string> feedRefsToRemove);
    const MessageType& getMessageType() override;
    const std::string getChannel() override;
    const std::vector<std::string>& getReferences() const;
};

class AttributeRegistrationMessage: public Message
{
private:
    MessageType m_messageType;
    std::vector<Attribute> m_attributes;
public:
    AttributeRegistrationMessage(std::vector<Attribute> attributes);
    const MessageType& getMessageType() override;
    const std::string getChannel() override;
    const std::vector<Attribute>& getAttributes() const;
};


}    // namespace wolkabout

#endif
