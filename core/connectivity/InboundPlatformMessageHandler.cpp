/**
 * Copyright 2022 Wolkabout Technology s.r.o.
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

#include "core/connectivity/InboundPlatformMessageHandler.h"

#include "core/model/Message.h"
#include "core/protocol/Protocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/StringUtils.h"

#include <algorithm>

namespace wolkabout
{
InboundPlatformMessageHandler::InboundPlatformMessageHandler(std::vector<std::string> deviceKeys)
: m_deviceKeys{std::move(deviceKeys)}, m_commandBuffer{new CommandBuffer}
{
}

InboundPlatformMessageHandler::~InboundPlatformMessageHandler()
{
    m_commandBuffer->stop();
}

void InboundPlatformMessageHandler::messageReceived(const std::string& channel, const std::string& payload)
{
    LOG(DEBUG) << "Message received on channel: '" << channel << "' : '" << payload << "'";

    std::lock_guard<std::mutex> lg{m_lock};

    auto it = std::find_if(m_channelHandlers.begin(), m_channelHandlers.end(),
                           [&](const std::pair<std::string, std::weak_ptr<MessageListener>>& kvp) {
                               return StringUtils::mqttTopicMatch(kvp.first, channel);
                           });

    if (it != m_channelHandlers.end())
    {
        auto channelHandler = it->second;
        addToCommandBuffer([=] {
            if (auto handler = channelHandler.lock())
            {
                handler->messageReceived(std::make_shared<Message>(payload, channel));
            }
        });
    }
    else
    {
        LOG(DEBUG) << "Handler for device channel not found: " << channel;
    }
}

std::vector<std::string> InboundPlatformMessageHandler::getChannels() const
{
    std::lock_guard<std::mutex> lg{m_lock};
    return m_subscriptionList;
}

void InboundPlatformMessageHandler::addListener(std::weak_ptr<MessageListener> listener)
{
    std::lock_guard<std::mutex> locker{m_lock};

    if (auto handler = listener.lock())
    {
        for (const auto& channel : handler->getProtocol().getInboundChannels())
        {
            LOG(DEBUG) << "Adding listener for channel: " << channel;
            m_channelHandlers[channel] = listener;
            m_subscriptionList.emplace_back(channel);
        }

        for (const auto& deviceKey : m_deviceKeys)
        {
            for (const auto& channel : handler->getProtocol().getInboundChannelsForDevice(deviceKey))
            {
                LOG(DEBUG) << "Adding listener for channel: " << channel;
                m_channelHandlers[channel] = listener;
                m_subscriptionList.emplace_back(channel);
            }
        }

        m_listeners.emplace_back(listener);
    }
}

void InboundPlatformMessageHandler::addToCommandBuffer(std::function<void()> command)
{
    m_commandBuffer->pushCommand(std::make_shared<std::function<void()>>(command));
}
}    // namespace wolkabout
