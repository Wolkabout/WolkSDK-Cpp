#include "WolkaboutDataProtocol.h"

#include <memory>
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
    j = json{reading.getReference(), reading.getStringValue()};
}

static void to_json(json& j, const std::vector<Reading>& readings)
{
    for(auto reading : readings)
    {
        j += json{reading.getReference(), reading.getStringValue()};
    }
}

static void to_json(json& j, const Attribute& attribute)
{
    j = json{
      {"name", attribute.getName()}, {"dataType", toString(attribute.getDataType())}, {"value", attribute.getValue()}};
}

static void to_json(json& j, const Parameters& param)
{
    j[toString(param.first)] = param.second;
}

static void from_json(const json& j, std::vector<Parameters>& p)
{
    for (auto& el : j.items())
    {
        Parameters param{ parameterNameFromString(el.key()), el.value().dump() };
        p.emplace_back(param);
    }
}

static void from_json(const json& j, std::vector<Reading>& r)
{
    for(auto& el: j.items())
    {
        if(el.key() != "timestamp")
        {
            Reading reading(el.key(), el.value().dump());
            r.emplace_back(reading);
        }
        else
        {
            for(auto addTs : r)
            {
                addTs.setTimestamp(el.value().dump());
            }
        }
    }
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
        json forTimestamp;
        for (auto reading : member.second)
        {
            forTimestamp[reading.getReference()] = reading.getStringValue();
        }
        if (member.first != 0)
        {
            forTimestamp["timestamp"] = member.first;
        }
        payload += json(forTimestamp);
    }
    auto jsonstring = json(payload).dump();
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

    json payload;

    for(auto parameter : parametersUpdateMessage.getParameters())
    {
        payload[toString(parameter.first)] = parameter.second;
    }

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersPullMessage parametersPullMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersPullMessage.getMessageType());

    return std::unique_ptr<Message>(new Message("", topic));
}

std::vector<std::string> WolkaboutDataProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER + toString(MessageType::PARAMETER_SYNC)
        , PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER + toString(MessageType::FEED_VALUES)
    };
}
std::string WolkaboutDataProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    unsigned first = topic.find(CHANNEL_DELIMITER);
    unsigned last = topic.rfind(CHANNEL_DELIMITER);

    return topic.substr(first, last - first);
}
MessageType WolkaboutDataProtocol::getMessageType(std::shared_ptr<Message> message)
{
    auto channel = message->getChannel();
    unsigned last = channel.rfind(CHANNEL_DELIMITER);

    return messageTypeFromString(channel.substr(last + 1));
}
std::shared_ptr<FeedValuesMessage> WolkaboutDataProtocol::parseFeedValues(std::shared_ptr<Message> message)
{
    auto content = message->getContent();
    auto jsonContent = json::parse(content);
    // TODO quadruple check
    std::vector<Reading> readings = jsonContent.get<std::vector<Reading>>();

    return std::make_shared<FeedValuesMessage>(readings);
}
std::shared_ptr<ParametersUpdateMessage> WolkaboutDataProtocol::parseParameters(std::shared_ptr<Message> message)
{
    json j = json::parse(message->getContent());

    std::vector<Parameters> parameters = j.get<std::vector<Parameters>>();
    return std::make_shared<ParametersUpdateMessage>(parameters);
}
std::vector<std::string> WolkaboutDataProtocol::getInboundChannels() const
{
    return {
      PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER + toString(MessageType::PARAMETER_SYNC)
        , PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER + toString(MessageType::FEED_VALUES)
    };
}

}    // namespace wolkabout
