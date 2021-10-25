#include "WolkaboutDataProtocol.h"

#include <string>

#include "core/Types.h"
#include "core/model/Feed.h"
#include "core/utilities/StringUtils.h"
#include "core/utilities/json.hpp"

using nlohmann::json;

namespace wolkabout
{
static void to_json(json& j, const Feed& feed)
{
    j = json{{"name", feed.getName()},
             {"type", toString(feed.getFeedType())},
             {"unitGuid", toString(feed.getUnit())},
             {"reference", feed.getReference()}};
}

static void to_json(json& j, const Reading& reading)
{
    j = json{{reading.getReference(), reading.getStringValue()}};
}

static void to_json(json& j, const Attribute& attribute)
{
    j = json{
      {"name", attribute.getName()}, {"dataType", toString(attribute.getDataType())}, {"value", attribute.getValue()}};
}

static void to_json(json& j, const Parameters& param)
{
    j = json{{param.first, param.second}};
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRegistrationMessage feedRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedRegistrationMessage.getMessageType());

    auto feeds = feedRegistrationMessage.getFeeds();
    json payload = json(feeds);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}
std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRemovalMessage feedRemovalMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedRemovalMessage.getMessageType());

    auto feeds = feedRemovalMessage.getReferences();
    json payload = json(feeds);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}
std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedValuesMessage feedValuesMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedValuesMessage.getMessageType());

    auto values = feedValuesMessage.getReadings();

    json payload;

    for (auto const& member : values)
    {
        json forTimestamp = json(member.second);
        if (member.first != 0)
        {
            forTimestamp.emplace_back(json{{"timestamp", std::to_string(member.first)}});
        }
        payload += json{{forTimestamp}};
    }

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    PullFeedValuesMessage pullFeedValuesMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(pullFeedValuesMessage.getMessageType());

    return std::unique_ptr<Message>(new Message("", topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(
  const std::string& deviceKey, AttributeRegistrationMessage attributeRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(attributeRegistrationMessage.getMessageType());

    auto attributes = attributeRegistrationMessage.getAttributes();
    json payload = json(attributes);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersUpdateMessage parametersUpdateMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersUpdateMessage.getMessageType());

    json payload = json(parametersUpdateMessage.getParameters());

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersPullMessage parametersPullMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersPullMessage.getMessageType());

    return std::unique_ptr<Message>(new Message("", topic));
}

std::unique_ptr<MessageObject> WolkaboutDataProtocol::parseInboundMessage(const Message& reference)
{
    auto tokens = StringUtils::tokenize(reference.getChannel(), CHANNEL_DELIMITER);

    if (tokens.at(0) != PLATFORM_TO_DEVICE_DIRECTION)
    {
        // message not inbound
        return nullptr;
    }
    auto deviceKey = tokens.at(1);

    if (tokens.at(2) == "feed_values")
    {
        return feedValuesFromContent(reference.getContent());
    }
    else if (tokens.at(2) == "parameters")
    {
        return parametersFromContent(reference.getContent());
    }

    return nullptr;
}
std::unique_ptr<MessageObject> WolkaboutDataProtocol::feedValuesFromContent(std::string content)
{
    return nullptr;
}
std::unique_ptr<MessageObject> WolkaboutDataProtocol::parametersFromContent(std::string content)
{
    return nullptr;
}

}    // namespace wolkabout
