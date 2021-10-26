#ifndef WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
#include "core/model/Message.h"
#include "core/protocol/Protocol.h"

#include <string>

namespace wolkabout
{
const std::string CHANNEL_DELIMITER = "/";
const std::string CHANNEL_MULTI_LEVEL_WILDCARD = "#";
const std::string CHANNEL_SINGLE_LEVEL_WILDCARD = "+";

const std::string DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string PLATFORM_TO_DEVICE_DIRECTION = "p2d/";

class WolkaboutDataProtocol : public Protocol
{
private:
    std::unique_ptr<MessageObject> feedValuesFromContent(std::string content);
    std::unique_ptr<MessageObject> parametersFromContent(std::string content);
public:
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 FeedRegistrationMessage feedRegistrationMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedRemovalMessage feedRemovalMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey, FeedValuesMessage feedValuesMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 PullFeedValuesMessage pullFeedValuesMessage);

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 AttributeRegistrationMessage attributeRegistrationMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 ParametersUpdateMessage parametersUpdateMessage);
    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 ParametersPullMessage parametersPullMessage);

    std::unique_ptr<MessageObject> parseInboundMessage(const Message& reference);
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;
    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;
};

}    // namespace wolkabout
#endif    // WOLKABOUTCORE_WOLKABOUTDATAPROTOCOL_H
