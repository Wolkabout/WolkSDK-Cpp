#include "WolkaboutDataProtocol.h"

#include "core/utilities/json.hpp"
#include "core/model/Feed.h"
#include "core/Types.h"

using nlohmann::json;

namespace wolkabout
{

static void to_json(json& j, const Feed& feed)
{
    j = json{
      {"name", feed.getName()},
      {"type", toString(feed.getFeedType())},
      {"unitGuid", toString(feed.getUnit())},
      {"reference", feed.getReference()}
    };
}

static void to_json(json& j, const Reading& reading)
{
    j = json{
      {reading.getReference(), reading.getStringValue()}
    };
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRegistrationMessage feedRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + deviceKey + toString(feedRegistrationMessage.getMessageType());

    auto feeds = feedRegistrationMessage.getFeeds();
    json payload = json(feeds);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}
std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRemovalMessage feedRemovalMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + deviceKey + toString(feedRemovalMessage.getMessageType());

    auto feeds = feedRemovalMessage.getReferences();
    json payload = json(feeds);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}
std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedValuesMessage feedValuesMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + deviceKey + toString(feedValuesMessage.getMessageType());

    auto values = feedValuesMessage.getReadings();

    json payload;

    for(auto const& member : values)
    {
        json forTimestamp = json(member.second);
//        forTimestamp.emplace_back("timestamp", std::to_string(member.first));
    }

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

} // !namespace wolkabout