#include "Types.h"

#include <algorithm>
#include <sstream>
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

DataType dataTypeFromString(const std::string& type)
{
    switch (type)
    {
    case "BOOLEAN":
        return DataType::BOOLEAN;
    case "NUMERIC":
        return DataType::NUMERIC;
    case "STRING":
        return DataType::STRING;
    case "HEXADECIMAL":
        return DataType::HEXADECIMAL;
    case "LOCATION":
        return DataType::LOCATION;
    case "ENUM":
        return DataType::ENUM;
    case "VECTOR":
        return DataType::VECTOR;
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

FeedType feedTypeFromString(const std::string& type)
{
    switch (type)
    {
    case "IN":
        return FeedType::IN;
    case "IN/OUT":
        return FeedType::IN_OUT;
    default:
        return FeedType::IN;
    }
}

std::string toString(Unit unit)
{
    switch (unit)
    {
    case Unit::NUMERIC:
        return "NUMERIC";
    case Unit::BOOLEAN:
        return "BOOLEAN";
    case Unit::PERCENT:
        return "PERCENT";
    case Unit::DECIBEL:
        return "DECIBEL";
    case Unit::LOCATION:
        return "LOCATION";
    case Unit::METRES_PER_SQUARE_SECOND:
        return "METRES_PER_SQUARE_SECOND";
    case Unit::G:
        return "G";
    case Unit::MOLE:
        return "MOLE";
    case Unit::ATOM:
        return "ATOM";
    case Unit::RADIAN:
        return "RADIAN";
    case Unit::REVOLUTION:
        return "REVOLUTION";
    case Unit::DEGREE_ANGLE:
        return "DEGREE_ANGLE";
    case Unit::MINUTE_ANGLE:
        return "MINUTE_ANGLE";
    case Unit::SECOND_ANGLE:
        return "SECOND_ANGLE";
    case Unit::CENTIRADIAN:
        return "CENTIRADIAN";
    case Unit::GRADE:
        return "GRADE";
    case Unit::SQUARE_METRE:
        return "SQUARE_METRE";
    case Unit::ARE:
        return "ARE";
    case Unit::HECTARE:
        return "HECTARE";
    case Unit::KATAL:
        return "KATAL";
    case Unit::BIT:
        return "BIT";
    case Unit::BYTE:
        return "BYTE";
    case Unit::SECOND:
        return "SECOND";
    case Unit::MINUTE:
        return "MINUTE";
    case Unit::HOUR:
        return "HOUR";
    case Unit::DAY:
        return "DAY";
    case Unit::WEEK:
        return "WEEK";
    case Unit::YEAR:
        return "YEAR";
    case Unit::MONTH:
        return "MONTH";
    case Unit::DAY_SIDEREAL:
        return "DAY_SIDEREAL";
    case Unit::YEAR_SIDEREAL:
        return "YEAR_SIDEREAL";
    case Unit::YEAR_CALENDAR:
        return "YEAR_CALENDAR";
    case Unit::POISE:
        return "POISE";
    case Unit::FARAD:
        return "FARAD";
    case Unit::COULOMB:
        return "COULOMB";
    case Unit::E:
        return "E";
    case Unit::FARADAY:
        return "FARADAY";
    case Unit::FRANKLIN:
        return "FRANKLIN";
    case Unit::SIEMENS:
        return "SIEMENS";
    case Unit::AMPERE:
        return "AMPERE";
    case Unit::GILBERT:
        return "GILBERT";
    case Unit::HENRY:
        return "HENRY";
    case Unit::VOLT:
        return "VOLT";
    case Unit::CENTIVOLT:
        return "CENTIVOLT";
    case Unit::MILLIVOLT:
        return "MILLIVOLT";
    case Unit::OHM:
        return "OHM";
    case Unit::JOULE:
        return "JOULE";
    case Unit::ERG:
        return "ERG";
    case Unit::ELECTRON_VOLT:
        return "ELECTRON_VOLT";
    case Unit::NEWTON:
        return "NEWTON";
    case Unit::DYNE:
        return "DYNE";
    case Unit::KILOGRAM_FORCE:
        return "KILOGRAM_FORCE";
    case Unit::POUND_FORCE:
        return "POUND_FORCE";
    case Unit::HERTZ:
        return "HERTZ";
    case Unit::MEGAHERTZ:
        return "MEGAHERTZ";
    case Unit::GIGAHERTZ:
        return "GIGAHERTZ";
    case Unit::LUX:
        return "LUX";
    case Unit::LAMBERT:
        return "LAMBERT";
    case Unit::STOKE:
        return "STOKE";
    case Unit::METRE:
        return "METRE";
    case Unit::KILOMETRE:
        return "KILOMETRE";
    case Unit::CENTIMETRE:
        return "CENTIMETRE";
    case Unit::MILLIMETRE:
        return "MILLIMETRE";
    case Unit::FOOT:
        return "FOOT";
    case Unit::FOOT_SURVEY_US:
        return "FOOT_SURVEY_US";
    case Unit::YARD:
        return "YARD";
    case Unit::INCH:
        return "INCH";
    case Unit::MILE:
        return "MILE";
    case Unit::NAUTICAL_MILE:
        return "NAUTICAL_MILE";
    case Unit::ANGSTROM:
        return "ANGSTROM";
    case Unit::ASTRONOMICAL_UNIT:
        return "ASTRONOMICAL_UNIT";
    case Unit::LIGHT_YEAR:
        return "LIGHT_YEAR";
    case Unit::PARSEC:
        return "PARSEC";
    case Unit::POINT:
        return "POINT";
    case Unit::PIXEL:
        return "PIXEL";
    case Unit::LUMEN:
        return "LUMEN";
    case Unit::CANDELA:
        return "CANDELA";
    case Unit::WEBER:
        return "WEBER";
    case Unit::MAXWELL:
        return "MAXWELL";
    case Unit::TESLA:
        return "TESLA";
    case Unit::GAUSS:
        return "GAUSS";
    case Unit::KILOGRAM:
        return "KILOGRAM";
    case Unit::GRAM:
        return "GRAM";
    case Unit::ATOMIC_MASS:
        return "ATOMIC_MASS";
    case Unit::ELECTRON_MASS:
        return "ELECTRON_MASS";
    case Unit::POUND:
        return "POUND";
    case Unit::OUNCE:
        return "OUNCE";
    case Unit::TON_US:
        return "TON_US";
    case Unit::TON_UK:
        return "TON_UK";
    case Unit::METRIC_TON:
        return "METRIC_TON";
    case Unit::WATT:
        return "WATT";
    case Unit::HORSEPOWER:
        return "HORSEPOWER";
    case Unit::PASCAL:
        return "PASCAL";
    case Unit::HECTOPASCAL:
        return "HECTOPASCAL";
    case Unit::ATMOSPHERE:
        return "ATMOSPHERE";
    case Unit::BAR:
        return "BAR";
    case Unit::MILLIBAR:
        return "MILLIBAR";
    case Unit::MILLIMETER_OF_MERCURY:
        return "MILLIMETER_OF_MERCURY";
    case Unit::INCH_OF_MERCURY:
        return "INCH_OF_MERCURY";
    case Unit::GRAY:
        return "GRAY";
    case Unit::RAD:
        return "RAD";
    case Unit::SIEVERT:
        return "SIEVERT";
    case Unit::REM:
        return "REM";
    case Unit::BECQUEREL:
        return "BECQUEREL";
    case Unit::CURIE:
        return "CURIE";
    case Unit::RUTHERFORD:
        return "RUTHERFORD";
    case Unit::ROENTGEN:
        return "ROENTGEN";
    case Unit::STERADIAN:
        return "STERADIAN";
    case Unit::SPHERE:
        return "SPHERE";
    case Unit::KELVIN:
        return "KELVIN";
    case Unit::CELSIUS:
        return "CELSIUS";
    case Unit::RANKINE:
        return "RANKINE";
    case Unit::FAHRENHEIT:
        return "FAHRENHEIT";
    case Unit::METRES_PER_SECOND:
        return "METRES_PER_SECOND";
    case Unit::MILES_PER_HOUR:
        return "MILES_PER_HOUR";
    case Unit::KILOMETRES_PER_HOUR:
        return "KILOMETRES_PER_HOUR";
    case Unit::KNOT:
        return "KNOT";
    case Unit::MACH:
        return "MACH";
    case Unit::C:
        return "C";
    case Unit::CUBIC_METRE:
        return "CUBIC_METRE";
    case Unit::LITRE:
        return "LITRE";
    case Unit::DECILITRE:
        return "DECILITRE";
    case Unit::MILLILITRE:
        return "MILLILITRE";
    case Unit::CUBIC_INCH:
        return "CUBIC_INCH";
    case Unit::GALLON_DRY_US:
        return "GALLON_DRY_US";
    case Unit::GALLON_UK:
        return "GALLON_UK";
    case Unit::OUNCE_LIQUID_UK:
        return "OUNCE_LIQUID_UK";
    }
}

Unit unitFromString(const std::string& unit)
{
    switch (unit)
    {
    case "NUMERIC":
        return Unit::NUMERIC;
    case "BOOLEAN":
        return Unit::BOOLEAN;
    case "PERCENT":
        return Unit::PERCENT;
    case "DECIBEL":
        return Unit::DECIBEL;
    case "LOCATION":
        return Unit::LOCATION;
    case "METRES_PER_SQUARE_SECOND":
        return Unit::METRES_PER_SQUARE_SECOND;
    case "G":
        return Unit::G;
    case "MOLE":
        return Unit::MOLE;
    case "ATOM":
        return Unit::ATOM;
    case "RADIAN":
        return Unit::RADIAN;
    case "REVOLUTION":
        return Unit::REVOLUTION;
    case "DEGREE_ANGLE":
        return Unit::DEGREE_ANGLE;
    case "MINUTE_ANGLE":
        return Unit::MINUTE_ANGLE;
    case "SECOND_ANGLE":
        return Unit::SECOND_ANGLE;
    case "CENTIRADIAN":
        return Unit::CENTIRADIAN;
    case "GRADE":
        return Unit::GRADE;
    case "SQUARE_METRE":
        return Unit::SQUARE_METRE;
    case "ARE":
        return Unit::ARE;
    case "HECTARE":
        return Unit::HECTARE;
    case "KATAL":
        return Unit::KATAL;
    case "BIT":
        return Unit::BIT;
    case "BYTE":
        return Unit::BYTE;
    case "SECOND":
        return Unit::SECOND;
    case "MINUTE":
        return Unit::MINUTE;
    case "HOUR":
        return Unit::HOUR;
    case "DAY":
        return Unit::DAY;
    case "WEEK":
        return Unit::WEEK;
    case "YEAR":
        return Unit::YEAR;
    case "MONTH":
        return Unit::MONTH;
    case "DAY_SIDEREAL":
        return Unit::DAY_SIDEREAL;
    case "YEAR_SIDEREAL":
        return Unit::YEAR_SIDEREAL;
    case "YEAR_CALENDAR":
        return Unit::YEAR_CALENDAR;
    case "POISE":
        return Unit::POISE;
    case "FARAD":
        return Unit::FARAD;
    case "COULOMB":
        return Unit::COULOMB;
    case "E":
        return Unit::E;
    case "FARADAY":
        return Unit::FARADAY;
    case "FRANKLIN":
        return Unit::FRANKLIN;
    case "SIEMENS":
        return Unit::SIEMENS;
    case "AMPERE":
        return Unit::AMPERE;
    case "GILBERT":
        return Unit::GILBERT;
    case "HENRY":
        return Unit::HENRY;
    case "VOLT":
        return Unit::VOLT;
    case "CENTIVOLT":
        return Unit::CENTIVOLT;
    case "MILLIVOLT":
        return Unit::MILLIVOLT;
    case "OHM":
        return Unit::OHM;
    case "JOULE":
        return Unit::JOULE;
    case "ERG":
        return Unit::ERG;
    case "ELECTRON_VOLT":
        return Unit::ELECTRON_VOLT;
    case "NEWTON":
        return Unit::NEWTON;
    case "DYNE":
        return Unit::DYNE;
    case "KILOGRAM_FORCE":
        return Unit::KILOGRAM_FORCE;
    case "POUND_FORCE":
        return Unit::POUND_FORCE;
    case "HERTZ":
        return Unit::HERTZ;
    case "MEGAHERTZ":
        return Unit::MEGAHERTZ;
    case "GIGAHERTZ":
        return Unit::GIGAHERTZ;
    case "LUX":
        return Unit::LUX;
    case "LAMBERT":
        return Unit::LAMBERT;
    case "STOKE":
        return Unit::STOKE;
    case "METRE":
        return Unit::METRE;
    case "KILOMETRE":
        return Unit::KILOMETRE;
    case "CENTIMETRE":
        return Unit::CENTIMETRE;
    case "MILLIMETRE":
        return Unit::MILLIMETRE;
    case "FOOT":
        return Unit::FOOT;
    case "FOOT_SURVEY_US":
        return Unit::FOOT_SURVEY_US;
    case "YARD":
        return Unit::YARD;
    case "INCH":
        return Unit::INCH;
    case "MILE":
        return Unit::MILE;
    case "NAUTICAL_MILE":
        return Unit::NAUTICAL_MILE;
    case "ANGSTROM":
        return Unit::ANGSTROM;
    case "ASTRONOMICAL_UNIT":
        return Unit::ASTRONOMICAL_UNIT;
    case "LIGHT_YEAR":
        return Unit::LIGHT_YEAR;
    case "PARSEC":
        return Unit::PARSEC;
    case "POINT":
        return Unit::POINT;
    case "PIXEL":
        return Unit::PIXEL;
    case "LUMEN":
        return Unit::LUMEN;
    case "CANDELA":
        return Unit::CANDELA;
    case "WEBER":
        return Unit::WEBER;
    case "MAXWELL":
        return Unit::MAXWELL;
    case "TESLA":
        return Unit::TESLA;
    case "GAUSS":
        return Unit::GAUSS;
    case "KILOGRAM":
        return Unit::KILOGRAM;
    case "GRAM":
        return Unit::GRAM;
    case "ATOMIC_MASS":
        return Unit::ATOMIC_MASS;
    case "ELECTRON_MASS":
        return Unit::ELECTRON_MASS;
    case "POUND":
        return Unit::POUND;
    case "OUNCE":
        return Unit::OUNCE;
    case "TON_US":
        return Unit::TON_US;
    case "TON_UK":
        return Unit::TON_UK;
    case "METRIC_TON":
        return Unit::METRIC_TON;
    case "WATT":
        return Unit::WATT;
    case "HORSEPOWER":
        return Unit::HORSEPOWER;
    case "PASCAL":
        return Unit::PASCAL;
    case "HECTOPASCAL":
        return Unit::HECTOPASCAL;
    case "ATMOSPHERE":
        return Unit::ATMOSPHERE;
    case "BAR":
        return Unit::BAR;
    case "MILLIBAR":
        return Unit::MILLIBAR;
    case "MILLIMETER_OF_MERCURY":
        return Unit::MILLIMETER_OF_MERCURY;
    case "INCH_OF_MERCURY":
        return Unit::INCH_OF_MERCURY;
    case "GRAY":
        return Unit::GRAY;
    case "RAD":
        return Unit::RAD;
    case "SIEVERT":
        return Unit::SIEVERT;
    case "REM":
        return Unit::REM;
    case "BECQUEREL":
        return Unit::BECQUEREL;
    case "CURIE":
        return Unit::CURIE;
    case "RUTHERFORD":
        return Unit::RUTHERFORD;
    case "ROENTGEN":
        return Unit::ROENTGEN;
    case "STERADIAN":
        return Unit::STERADIAN;
    case "SPHERE":
        return Unit::SPHERE;
    case "KELVIN":
        return Unit::KELVIN;
    case "CELSIUS":
        return Unit::CELSIUS;
    case "RANKINE":
        return Unit::RANKINE;
    case "FAHRENHEIT":
        return Unit::FAHRENHEIT;
    case "METRES_PER_SECOND":
        return Unit::METRES_PER_SECOND;
    case "MILES_PER_HOUR":
        return Unit::MILES_PER_HOUR;
    case "KILOMETRES_PER_HOUR":
        return Unit::KILOMETRES_PER_HOUR;
    case "KNOT":
        return Unit::KNOT;
    case "MACH":
        return Unit::MACH;
    case "C":
        return Unit::C;
    case "CUBIC_METRE":
        return Unit::CUBIC_METRE;
    case "LITRE":
        return Unit::LITRE;
    case "DECILITRE":
        return Unit::DECILITRE;
    case "MILLILITRE":
        return Unit::MILLILITRE;
    case "CUBIC_INCH":
        return Unit::CUBIC_INCH;
    case "GALLON_DRY_US":
        return Unit::GALLON_DRY_US;
    case "GALLON_UK":
        return Unit::GALLON_UK;
    case "OUNCE_LIQUID_UK":
        return Unit::OUNCE_LIQUID_UK;
    }
}

std::string toString(ConnectivityType conn)
{
    switch (conn)
    {
    case ConnectivityType::MQTT:
        return "MQTT";
    case ConnectivityType::HTTP:
        return "HTTP";
    case ConnectivityType::AMQP:
        return "AMQP";
    case ConnectivityType::GATEWAY:
        return "GATEWAY";
    default:
        return "";
    }
}
ConnectivityType connectivityTypeFromString(const std::string& type)
{
    switch (type)
    {
    case "MQTT":
        return ConnectivityType::MQTT;
    case "HTTP":
        return ConnectivityType::HTTP;
    case "AMQP":
        return ConnectivityType::AMQP;
    case "GATEWAY":
        return ConnectivityType::GATEWAY;
    }
}

std::string toString(OutboundDataMode mode)
{
    switch (mode)
    {
    case OutboundDataMode::PUSH:
        return "PUSH";
    case OutboundDataMode::PULL:
        return "PULL";
    default:
        return "";
    }
}
OutboundDataMode outboundDataModeFromString(const std::string& mode)
{
    switch (mode)
    {
    case "PUSH":
        return OutboundDataMode::PUSH;
    case "PULL":
        return OutboundDataMode::PULL;
    }
}

std::string toString(ParameterName parameterName)
{
    switch (parameterName)
    {
    case ParameterName::CONNECTIVITY_TYPE:
        return "CONNECTIVITY_TYPE";
    case ParameterName::OUTBOUND_DATA_MODE:
        return "OUTBOUND_DATA_MODE";
    case ParameterName::OUTBOUND_DATA_RETENTION_TIME:
        return "OUTBOUND_DATA_RETENTION_TIME";
    case ParameterName::MAXIMUM_MESSAGE_SIZE:
        return "MAXIMUM_MESSAGE_SIZE";
    case ParameterName::FILE_TRANSFER_PLATFORM_ENABLED:
        return "FILE_TRANSFER_PLATFORM_ENABLED";
    case ParameterName::FILE_TRANSFER_URL_ENABLED:
        return "FILE_TRANSFER_URL_ENABLED";
    case ParameterName::FIRMWARE_UPDATE_ENABLED:
        return "FIRMWARE_UPDATE_ENABLED";
    case ParameterName::FIRMWARE_UPDATE_CHECK_TIME:
        return "FIRMWARE_UPDATE_CHECK_TIME";
    case ParameterName::FIRMWARE_VERSION:
        return "FIRMWARE_VERSION";
    case ParameterName::GATEWAY:
        return "GATEWAY";
    case ParameterName::GATEWAY_PARENT:
        return "GATEWAY_PARENT";
    case ParameterName::EXTERNAL_ID:
        return "EXTERNAL_ID";
    default:
        return "";
    }
}
ParameterName parameterNameFromString(std::string parameterName)
{
    switch (parameterName)
    {
    case "CONNECTIVITY_TYPE":
        return ParameterName::CONNECTIVITY_TYPE;
    case "OUTBOUND_DATA_MODE":
        return ParameterName::OUTBOUND_DATA_MODE;
    case "OUTBOUND_DATA_RETENTION_TIME":
        return ParameterName::OUTBOUND_DATA_RETENTION_TIME;
    case "MAXIMUM_MESSAGE_SIZE":
        return ParameterName::MAXIMUM_MESSAGE_SIZE;
    case "FILE_TRANSFER_PLATFORM_ENABLED":
        return ParameterName::FILE_TRANSFER_PLATFORM_ENABLED;
    case "FILE_TRANSFER_URL_ENABLED":
        return ParameterName::FILE_TRANSFER_URL_ENABLED;
    case "FIRMWARE_UPDATE_ENABLED":
        return ParameterName::FIRMWARE_UPDATE_ENABLED;
    case "FIRMWARE_UPDATE_CHECK_TIME":
        return ParameterName::FIRMWARE_UPDATE_CHECK_TIME;
    case "FIRMWARE_VERSION":
        return ParameterName::FIRMWARE_VERSION;
    case "GATEWAY":
        return ParameterName::GATEWAY;
    case "GATEWAY_PARENT":
        return ParameterName::GATEWAY_PARENT;
    case "EXTERNAL_ID":
        return ParameterName::EXTERNAL_ID;
    }
}

std::string toString(MessageType type)
{
    switch (type)
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

MessageType messageTypeFromString(const std::string& type)
{
    switch (type)
    {
    case "feed_values":
        return MessageType::FEED_VALUES;
    case "pull_feed_values":
        return MessageType::PULL_FEED_VALUES;
    case "feed_registration":
        return MessageType::FEED_REGISTRATION;
    case "feed_removal":
        return MessageType::FEED_REMOVAL;
    case "attribute_registration":
        return MessageType::ATTRIBUTE_REGISTRATION;
    case "parameters":
        return MessageType::PARAMETER_SYNC;
    case "pull_parameters":
        return MessageType::PULL_PARAMETERS;
    case "time":
        return MessageType::TIME_SYNC;
    case "file_upload_init":
        return MessageType::FILE_UPLOAD_INIT;
    case "file_upload_status":
        return MessageType::FILE_UPLOAD_STATUS;
    case "file_upload_abort":
        return MessageType::FILE_UPLOAD_ABORT;
    case "file_binary_request":
        return MessageType::FILE_BINARY_REQUEST;
    case "file_binary_response":
        return MessageType::FILE_BINARY_RESPONSE;
    case "file_url_download_init":
        return MessageType::FILE_URL_DOWNLOAD_INIT;
    case "file_url_download_abort":
        return MessageType::FILE_URL_DOWNLOAD_ABORT;
    case "file_url_download_status":
        return MessageType::FILE_URL_DOWNLOAD_STATUS;
    case "file_list":
        return MessageType::FILE_LIST;
    case "file_delete":
        return MessageType::FILE_DELETE;
    case "file_purge":
        return MessageType::FILE_PURGE;
    case "firmware_update_install":
        return MessageType::FIRMWARE_UPDATE_INSTALL;
    case "firmware_update_status":
        return MessageType::FIRMWARE_UPDATE_STATUS;
    case "firmware_update_abort":
        return MessageType::FIRMWARE_UPDATE_ABORT;
    case "gateway_device_registration":
        return MessageType::GATEWAY_DEVICE_REGISTRATION;
    case "gateway_device_removal":
        return MessageType::GATEWAY_DEVICE_REMOVAL;
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
: m_reference(reference), m_value(value), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(const Reading& reading)
: m_reference(reading.getReference()), m_value(reading.getStringValue()), m_timestamp(reading.getTimestamp())
{
}

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

    while ((pos = valueCopy.find(DELIMITER)) != std::string::npos)
    {
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

}    // namespace wolkabout