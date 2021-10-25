#include "Feed.h"
namespace wolkabout
{
const std::string& Feed::getName() const
{
    return m_name;
}
void Feed::setName(const std::string& name)
{
    m_name = name;
}
const std::string& Feed::getReference() const
{
    return m_reference;
}
void Feed::setReference(const std::string& reference)
{
    m_reference = reference;
}
FeedType Feed::getFeedType() const
{
    return m_feedType;
}
void Feed::setFeedType(FeedType feedType)
{
    m_feedType = feedType;
}
Unit Feed::getUnit() const
{
    return m_unit;
}
void Feed::setUnit(Unit unit)
{
    m_unit = unit;
}
}    // namespace wolkabout
