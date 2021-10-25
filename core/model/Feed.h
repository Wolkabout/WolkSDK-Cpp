#ifndef WOLKABOUTCORE_FEED_H
#define WOLKABOUTCORE_FEED_H
#include "core/Types.h"

#include <functional>
#include <string>

namespace wolkabout
{
class Feed
{
private:
    std::string m_name;
    std::string m_reference;
    FeedType m_feedType;
    Unit m_unit;

public:
    const std::string& getName() const;
    void setName(const std::string& name);
    const std::string& getReference() const;
    void setReference(const std::string& reference);
    FeedType getFeedType() const;
    void setFeedType(FeedType feedType);
    Unit getUnit() const;
    void setUnit(Unit unit);
};

}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FEED_H
