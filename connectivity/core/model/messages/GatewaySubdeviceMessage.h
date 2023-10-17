#ifndef WOLKABOUTCORE_GATEWAYSUBDEVICEMESSAGE_H
#define WOLKABOUTCORE_GATEWAYSUBDEVICEMESSAGE_H

#include "core/model/Message.h"

#include <string>

namespace wolkabout
{
class GatewaySubdeviceMessage
{
public:
    explicit GatewaySubdeviceMessage(const Message& message);

    const Message& getMessage() const;

private:
    Message m_message;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_GATEWAYSUBDEVICEMESSAGE_H
