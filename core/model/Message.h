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

#include "Attribute.h"
#include "Feed.h"
#include "core/Types.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class Message
{
public:
    Message(std::string content, std::string channel);
    virtual ~Message() = default;

    const std::string& getContent() const;
    const std::string& getChannel() const;

private:
    std::string m_content;
    std::string m_channel;
};

class MessageObject
{
public:
    virtual ~MessageObject() = default;
    virtual const MessageType& getMessageType() = 0;
};

class FeedValuesMessage : public MessageObject
{
private:
    std::map<unsigned long long int, std::vector<Reading>> m_readings;
    MessageType m_messageType;

public:
    FeedValuesMessage(std::vector<Reading> readings);
    FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings);
    const MessageType& getMessageType() override;
    const std::map<unsigned long long int, std::vector<Reading>>& getReadings() const;
};

class PullFeedValuesMessage : public MessageObject
{
private:
    MessageType m_messageType;

public:
    PullFeedValuesMessage();
    const MessageType& getMessageType() override;
};

class FeedRegistrationMessage : public MessageObject
{
private:
    MessageType m_messageType;
    std::vector<Feed> m_feeds;

public:
    FeedRegistrationMessage(std::vector<Feed> feeds);
    const MessageType& getMessageType() override;
    const std::vector<Feed>& getFeeds() const;
};

class FeedRemovalMessage : public MessageObject
{
private:
    MessageType m_messageType;
    std::vector<std::string> m_references;

public:
    FeedRemovalMessage(std::vector<std::string> feedRefsToRemove);
    const MessageType& getMessageType() override;
    const std::vector<std::string>& getReferences() const;
};

class AttributeRegistrationMessage : public MessageObject
{
private:
    MessageType m_messageType;
    std::vector<Attribute> m_attributes;

public:
    AttributeRegistrationMessage(std::vector<Attribute> attributes);
    AttributeRegistrationMessage(std::vector<std::shared_ptr<Attribute>> attributes);
    const MessageType& getMessageType() override;
    const std::vector<Attribute>& getAttributes() const;
};

class ParametersUpdateMessage : public MessageObject
{
private:
    MessageType m_messageType;
    std::vector<Parameters> m_parameterList;

public:
    ParametersUpdateMessage(std::vector<Parameters> parameterList);
    const MessageType& getMessageType() override;
    const std::vector<Parameters>& getParameters() const;
};

class ParametersPullMessage : public MessageObject
{
private:
    MessageType m_messageType;

public:
    ParametersPullMessage();
    const MessageType& getMessageType() override;
};

}    // namespace wolkabout

#endif
