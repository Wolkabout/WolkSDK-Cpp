#ifndef WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
#include <string>
#include "core/model/Message.h"

namespace wolkabout
{
const std::string CHANNEL_DELIMITER = "/";
const std::string CHANNEL_MULTI_LEVEL_WILDCARD = "#";
const std::string CHANNEL_SINGLE_LEVEL_WILDCARD = "+";

const std::string DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string PLATFORM_TO_DEVICE_DIRECTION = "p2d/";

class WolkaboutDataProtocol
{
public:
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedRegistrationMessage feedRegistrationMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedRemovalMessage feedRemovalMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedValuesMessage feedValuesMessage);

    std::unique_ptr<FeedValuesMessage> parseInboundMessage(const Message& reference);


};

} // !namespace wolkabout
#endif    // WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
