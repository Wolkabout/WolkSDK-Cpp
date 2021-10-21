#include <sstream>
#include <algorithm>
#include "Types.h"
namespace wolkabout
{

const char DELIMITER = ',';

std::string toString(DataType type)
{
    switch (type)
    {
    case DataType::BOOLEAN:
        return "BOOLEAN";
    case DataType::NUMERIC:
        return "NUMERIC";
    case DataType::STRING:
        return "STRING";
    case DataType::HEXADECIMAL:
        return "HEXADECIMAL";
    case DataType::LOCATION:
        return "LOCATION";
    case DataType::ENUM:
        return "ENUM";
    case DataType::VECTOR:
        return "VECTOR";
    default:
        return "";
    }
}

std::string toString(FeedType type)
{
    switch (type)
    {
    case FeedType::IN:
        return "IN";
    case FeedType::IN_OUT:
        return "IN/OUT";
    default:
        return "";
    }
}

std::string toString(MessageType type)
{
    switch(type)
    {
    case MessageType::FEED_VALUES:
        return "feed_values";
    case MessageType::PULL_FEED_VALUES:
        return "pull_feed_values";
    case MessageType::FEED_REGISTRATION:
        return "feed_registration";
    case MessageType::FEED_REMOVAL:
        return "feed_removal";
    case MessageType::ATTRIBUTE_REGISTRATION:
        return "attribute_registration";
    case MessageType::PARAMETER_SYNC:
        return "parameters";
    case MessageType::PULL_PARAMETERS:
        return "pull_parameters";
    case MessageType::TIME_SYNC:
        return "time";
    case MessageType::FILE_UPLOAD_INIT:
        return "file_upload_init";
    case MessageType::FILE_UPLOAD_STATUS:
        return "file_upload_status";
    case MessageType::FILE_UPLOAD_ABORT:
        return "file_upload_abort";
    case MessageType::FILE_BINARY_REQUEST:
        return "file_binary_request";
    case MessageType::FILE_BINARY_RESPONSE:
        return "file_binary_response";
    case MessageType::FILE_URL_DOWNLOAD_INIT:
        return "file_url_download_init";
    case MessageType::FILE_URL_DOWNLOAD_ABORT:
        return "file_url_download_abort";
    case MessageType::FILE_URL_DOWNLOAD_STATUS:
        return "file_url_download_status";
    case MessageType::FILE_LIST:
        return "file_list";
    case MessageType::FILE_DELETE:
        return "file_delete";
    case MessageType::FILE_PURGE:
        return "file_purge";
    case MessageType::FIRMWARE_UPDATE_INSTALL:
        return "firmware_update_install";
    case MessageType::FIRMWARE_UPDATE_STATUS:
        return "firmware_update_status";
    case MessageType::FIRMWARE_UPDATE_ABORT:
        return "firmware_update_abort";
    case MessageType::GATEWAY_DEVICE_REGISTRATION:
        return "gateway_device_registration";
    case MessageType::GATEWAY_DEVICE_REMOVAL:
        return "gateway_device_removal";
    default:
        return "";
    }
}

const std::string& Reading::getReference() const
{
    return m_reference;
}
void Reading::setReference(const std::string& reference)
{
    m_reference = reference;
}
void Reading::setValue(const std::string& value)
{
    m_value = value;
}
const std::string& Reading::getStringValue() const
{
    return m_value;
}
Reading::Reading(std::string reference, std::string value, unsigned long long int rtcTimestamp)
: m_reference(reference), m_value(value), m_timestamp(rtcTimestamp) {}

Reading::Reading(const Reading& reading)
: m_reference(reading.getReference())
, m_value(reading.getStringValue())
, m_timestamp(reading.getTimestamp())
{}

void Reading::setTimestamp(unsigned long long int& timestamp) {}

const float Reading::getNumericValue() const
{
    return std::stof(m_value);
}

const bool Reading::getBoolValue() const
{
    return (m_value == "true");
}

const int Reading::getHexValue() const
{
    std::istringstream iss(m_value);
    int result = 0;
    if ((iss >> std::hex >> result))
    {
        return result;
    }
    return 0;
}

const Location Reading::getLocationValue() const
{
    Location location;
    if ((std::count(m_value.begin(), m_value.end(), DELIMITER) != 1))
    {
        location.x = 0;
        location.y = 0;
    }
    else
    {
        auto delimiterPosition = m_value.find(DELIMITER);
        auto xValue = m_value.substr(0, delimiterPosition);
        auto yValue = m_value.substr(delimiterPosition);
        location.x = std::stof(xValue);
        location.y = std::stof(yValue);
    }

    return location;
}

const std::vector<float> Reading::getVectorValue() const
{
    std::vector<float> vectorVal{};
    auto valueCopy = m_value;
    size_t pos;

    while ((pos = valueCopy.find(DELIMITER)) != std::string::npos) {
        auto token = valueCopy.substr(0, pos);
        vectorVal.emplace_back(std::stof(token));

        valueCopy.erase(0, pos + 1);
    }

    return vectorVal;
}
const unsigned long long int& Reading::getTimestamp() const
{
    return m_timestamp;
}


} // !namespace wolkabout