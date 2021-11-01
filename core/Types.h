#ifndef WOLKABOUTCORE_TYPES_H
#define WOLKABOUTCORE_TYPES_H
#include <string>
#include <vector>

namespace wolkabout
{
enum class DataType
{
    STRING,
    NUMERIC,
    BOOLEAN,
    HEXADECIMAL,
    LOCATION,
    ENUM,
    VECTOR,
};

std::string toString(DataType type);
DataType dataTypeFromString(const std::string& type);

enum class FeedType
{
    IN,
    IN_OUT,
};

std::string toString(FeedType type);
FeedType feedTypeFromString(const std::string& type);

enum class Unit
{
    NUMERIC,
    BOOLEAN,
    PERCENT,
    DECIBEL,
    LOCATION,
    METRES_PER_SQUARE_SECOND,
    G,
    MOLE,
    ATOM,
    RADIAN,
    REVOLUTION,
    DEGREE_ANGLE,
    MINUTE_ANGLE,
    SECOND_ANGLE,
    CENTIRADIAN,
    GRADE,
    SQUARE_METRE,
    ARE,
    HECTARE,
    KATAL,
    BIT,
    BYTE,
    SECOND,
    MINUTE,
    HOUR,
    DAY,
    WEEK,
    YEAR,
    MONTH,
    DAY_SIDEREAL,
    YEAR_SIDEREAL,
    YEAR_CALENDAR,
    POISE,
    FARAD,
    COULOMB,
    E,
    FARADAY,
    FRANKLIN,
    SIEMENS,
    AMPERE,
    GILBERT,
    HENRY,
    VOLT,
    CENTIVOLT,
    MILLIVOLT,
    OHM,
    JOULE,
    ERG,
    ELECTRON_VOLT,
    NEWTON,
    DYNE,
    KILOGRAM_FORCE,
    POUND_FORCE,
    HERTZ,
    MEGAHERTZ,
    GIGAHERTZ,
    LUX,
    LAMBERT,
    STOKE,
    METRE,
    KILOMETRE,
    CENTIMETRE,
    MILLIMETRE,
    FOOT,
    FOOT_SURVEY_US,
    YARD,
    INCH,
    MILE,
    NAUTICAL_MILE,
    ANGSTROM,
    ASTRONOMICAL_UNIT,
    LIGHT_YEAR,
    PARSEC,
    POINT,
    PIXEL,
    LUMEN,
    CANDELA,
    WEBER,
    MAXWELL,
    TESLA,
    GAUSS,
    KILOGRAM,
    GRAM,
    ATOMIC_MASS,
    ELECTRON_MASS,
    POUND,
    OUNCE,
    TON_US,
    TON_UK,
    METRIC_TON,
    WATT,
    HORSEPOWER,
    PASCAL,
    HECTOPASCAL,
    ATMOSPHERE,
    BAR,
    MILLIBAR,
    MILLIMETER_OF_MERCURY,
    INCH_OF_MERCURY,
    GRAY,
    RAD,
    SIEVERT,
    REM,
    BECQUEREL,
    CURIE,
    RUTHERFORD,
    ROENTGEN,
    STERADIAN,
    SPHERE,
    KELVIN,
    CELSIUS,
    RANKINE,
    FAHRENHEIT,
    METRES_PER_SECOND,
    MILES_PER_HOUR,
    KILOMETRES_PER_HOUR,
    KNOT,
    MACH,
    C,
    CUBIC_METRE,
    LITRE,
    DECILITRE,
    MILLILITRE,
    CUBIC_INCH,
    GALLON_DRY_US,
    GALLON_UK,
    OUNCE_LIQUID_UK,
};

std::string toString(Unit unit);
Unit unitFromString(const std::string& unit);

enum class ConnectivityType
{
    MQTT,
    HTTP,
    AMQP,
    GATEWAY,
};

std::string toString(ConnectivityType conn);
ConnectivityType connectivityTypeFromString(const std::string& type);

enum class OutboundDataMode
{
    PUSH,
    PULL,
};

std::string toString(OutboundDataMode mode);
OutboundDataMode outboundDataModeFromString(const std::string& mode);

enum class ParameterName
{
    CONNECTIVITY_TYPE,
    OUTBOUND_DATA_MODE,
    OUTBOUND_DATA_RETENTION_TIME,
    MAXIMUM_MESSAGE_SIZE,
    FILE_TRANSFER_PLATFORM_ENABLED,
    FILE_TRANSFER_URL_ENABLED,
    FIRMWARE_UPDATE_ENABLED,
    FIRMWARE_UPDATE_CHECK_TIME,
    FIRMWARE_VERSION,
    GATEWAY,
    GATEWAY_PARENT,
    EXTERNAL_ID,
};

std::string toString(ParameterName parameterName);
ParameterName parameterNameFromString(std::string parameterName);

typedef std::pair<ParameterName, std::string> Parameters;

enum class MessageType
{
    FEED_VALUES,
    PULL_FEED_VALUES,
    FEED_REGISTRATION,
    FEED_REMOVAL,
    ATTRIBUTE_REGISTRATION,
    PARAMETER_SYNC,
    PULL_PARAMETERS,
    TIME_SYNC,
    FILE_UPLOAD_INIT,
    FILE_UPLOAD_STATUS,
    FILE_UPLOAD_ABORT,
    FILE_BINARY_REQUEST,
    FILE_BINARY_RESPONSE,
    FILE_URL_DOWNLOAD_INIT,
    FILE_URL_DOWNLOAD_ABORT,
    FILE_URL_DOWNLOAD_STATUS,
    FILE_LIST,
    FILE_DELETE,
    FILE_PURGE,
    FIRMWARE_UPDATE_INSTALL,
    FIRMWARE_UPDATE_STATUS,
    FIRMWARE_UPDATE_ABORT,

    GATEWAY_DEVICE_REGISTRATION,
    GATEWAY_DEVICE_REMOVAL,
};

std::string toString(MessageType type);
MessageType messageTypeFromString(const std::string& type);

struct Location
{
    float x;
    float y;
};

class Reading
{
private:
    std::string m_reference;
    std::string m_value;
    unsigned long long int m_timestamp;

public:
    Reading() = default;
    Reading(std::string reference, std::string value, unsigned long long int rtcTimestamp = 0);
    Reading(const Reading& reading);

    void setReference(const std::string& reference);
    void setValue(const std::string& value);
    void setTimestamp(unsigned long long int& timestamp);
    void setTimestamp(std::string timestamp);
    const std::string& getReference() const;
    // TODO move to utility methods
    const std::string& getStringValue() const;
    const float getNumericValue() const;
    const bool getBoolValue() const;
    const int getHexValue() const;
    const Location getLocationValue() const;
    const std::vector<float> getVectorValue() const;
    const unsigned long long int& getTimestamp() const;
};

}    // namespace wolkabout

#endif    // WOLKABOUTCORE_TYPES_H