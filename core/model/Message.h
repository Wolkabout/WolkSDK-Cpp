/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

    virtual MessageType getMessageType() = 0;
};

class FeedValuesMessage : public MessageObject
{
public:
    explicit FeedValuesMessage(const std::vector<Reading>& readings);

    explicit FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings);

    MessageType getMessageType() override;

    const std::map<std::uint64_t, std::vector<Reading>>& getReadings() const;

private:
    std::map<std::uint64_t, std::vector<Reading>> m_readings;
};

class PullFeedValuesMessage : public MessageObject
{
public:
    MessageType getMessageType() override;
};

class FeedRegistrationMessage : public MessageObject
{
public:
    explicit FeedRegistrationMessage(std::vector<Feed> feeds);

    MessageType getMessageType() override;

    const std::vector<Feed>& getFeeds() const;

private:
    std::vector<Feed> m_feeds;
};

class FeedRemovalMessage : public MessageObject
{
public:
    explicit FeedRemovalMessage(std::vector<std::string> feedRefsToRemove);

    MessageType getMessageType() override;

    const std::vector<std::string>& getReferences() const;

private:
    std::vector<std::string> m_references;
};

class AttributeRegistrationMessage : public MessageObject
{
public:
    explicit AttributeRegistrationMessage(std::vector<Attribute> attributes);

    explicit AttributeRegistrationMessage(const std::vector<std::shared_ptr<Attribute>>& attributes);

    MessageType getMessageType() override;

    const std::vector<Attribute>& getAttributes() const;

private:
    std::vector<Attribute> m_attributes;
};

class ParametersUpdateMessage : public MessageObject
{
public:
    explicit ParametersUpdateMessage(std::vector<Parameters> parameterList);

    MessageType getMessageType() override;

    const std::vector<Parameters>& getParameters() const;

private:
    std::vector<Parameters> m_parameterList;
};

class ParametersPullMessage : public MessageObject
{
public:
    MessageType getMessageType() override;
};

}    // namespace wolkabout

#endif
