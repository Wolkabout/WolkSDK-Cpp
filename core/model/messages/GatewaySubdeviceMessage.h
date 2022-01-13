#ifndef WOLKABOUTCORE_GATEWAYSUBDEVICEMESSAGE_H
#define WOLKABOUTCORE_GATEWAYSUBDEVICEMESSAGE_H

#include "core/model/Message.h"

#include <string>

namespace wolkabout
{
class GatewaySubdeviceMessage
{
public:
    GatewaySubdeviceMessage(const Message& message, std::string subdeviceKey);

    const Message& getMessage() const;

    const std::string& getSubdeviceKey() const;

private:
    Message m_message;
    std::string m_subdeviceKey;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_GATEWAYSUBDEVICEMESSAGE_H
