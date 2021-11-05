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

#include "Message.h"

#include <string>
#include <utility>

namespace wolkabout
{
Message::Message(std::string content, std::string channel)
: m_content(std::move(content)), m_channel(std::move(channel))
{
}

const std::string& Message::getContent() const
{
    return m_content;
}

const std::string& Message::getChannel() const
{
    return m_channel;
}

FeedValuesMessage::FeedValuesMessage(std::vector<Reading> readings) : m_messageType(MessageType::FEED_VALUES)
{
    for (auto reading : readings)
    {
        m_readings[reading.getTimestamp()].emplace_back(reading);
    }
}

FeedValuesMessage::FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings)
: m_messageType(MessageType::FEED_VALUES)
{
    for (auto reading : readings)
    {
        m_readings[reading->getTimestamp()].emplace_back(*reading);
    }
}

const MessageType& FeedValuesMessage::getMessageType()
{
    return m_messageType;
}

const std::map<unsigned long long int, std::vector<Reading>>& FeedValuesMessage::getReadings() const
{
    return m_readings;
}
PullFeedValuesMessage::PullFeedValuesMessage() : m_messageType(MessageType::PULL_FEED_VALUES) {}

const MessageType& PullFeedValuesMessage::getMessageType()
{
    return m_messageType;
}

FeedRegistrationMessage::FeedRegistrationMessage(std::vector<Feed> feeds)
: m_feeds(std::move(feeds)), m_messageType(MessageType::FEED_REGISTRATION)
{
}

const std::vector<Feed>& FeedRegistrationMessage::getFeeds() const
{
    return m_feeds;
}

const MessageType& FeedRegistrationMessage::getMessageType()
{
    return m_messageType;
}

FeedRemovalMessage::FeedRemovalMessage(std::vector<std::string> feedRefsToRemove)
: m_messageType(MessageType::FEED_REMOVAL), m_references(std::move(feedRefsToRemove))
{
}
const MessageType& FeedRemovalMessage::getMessageType()
{
    return m_messageType;
}
const std::vector<std::string>& FeedRemovalMessage::getReferences() const
{
    return m_references;
}
AttributeRegistrationMessage::AttributeRegistrationMessage(std::vector<Attribute> attributes)
: m_messageType(MessageType::ATTRIBUTE_REGISTRATION), m_attributes(std::move(attributes))
{
}
const MessageType& AttributeRegistrationMessage::getMessageType()
{
    return m_messageType;
}
const std::vector<Attribute>& AttributeRegistrationMessage::getAttributes() const
{
    return m_attributes;
}
AttributeRegistrationMessage::AttributeRegistrationMessage(std::vector<std::shared_ptr<Attribute>> attributes)
: m_messageType(MessageType::ATTRIBUTE_REGISTRATION)
{
    for (auto attribute : attributes)
    {
        m_attributes.push_back(*attribute);
    }
}

ParametersUpdateMessage::ParametersUpdateMessage(std::vector<Parameters> parameterList)
: m_messageType(MessageType::PARAMETER_SYNC), m_parameterList(std::move(parameterList))
{
}
const MessageType& ParametersUpdateMessage::getMessageType()
{
    return m_messageType;
}
const std::vector<Parameters>& ParametersUpdateMessage::getParameters() const
{
    return m_parameterList;
}
ParametersPullMessage::ParametersPullMessage() : m_messageType(MessageType::PULL_PARAMETERS) {}
const MessageType& ParametersPullMessage::getMessageType()
{
    return m_messageType;
}
}    // namespace wolkabout
