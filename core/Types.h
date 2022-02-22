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

#ifndef WOLKABOUTCORE_TYPES_H
#define WOLKABOUTCORE_TYPES_H

#include <string>
#include <vector>

namespace wolkabout
{
enum class DataType
{
    NONE = -1,
    STRING,
    NUMERIC,
    BOOLEAN,
    HEXADECIMAL,
    LOCATION,
    ENUM,
    VECTOR
};

std::string toString(DataType type);
DataType dataTypeFromString(std::string value);

enum class FeedType
{
    NONE = -1,
    IN,
    IN_OUT,
};

std::string toString(FeedType type);
FeedType feedTypeFromString(std::string value);

class Unit
{
public:
    static const std::string NUMERIC;
    static const std::string BOOLEAN;
    static const std::string PERCENT;
    static const std::string DECIBEL;
    static const std::string LOCATION;
    static const std::string TEXT;
    static const std::string METRES_PER_SQUARE_SECOND;
    static const std::string G;
    static const std::string MOLE;
    static const std::string ATOM;
    static const std::string RADIAN;
    static const std::string REVOLUTION;
    static const std::string DEGREE_ANGLE;
    static const std::string MINUTE_ANGLE;
    static const std::string SECOND_ANGLE;
    static const std::string CENTIRADIAN;
    static const std::string GRADE;
    static const std::string SQUARE_METRE;
    static const std::string ARE;
    static const std::string HECTARE;
    static const std::string KATAL;
    static const std::string BIT;
    static const std::string BYTE;
    static const std::string SECOND;
    static const std::string MINUTE;
    static const std::string HOUR;
    static const std::string DAY;
    static const std::string WEEK;
    static const std::string YEAR;
    static const std::string MONTH;
    static const std::string DAY_SIDEREAL;
    static const std::string YEAR_SIDEREAL;
    static const std::string YEAR_CALENDAR;
    static const std::string POISE;
    static const std::string FARAD;
    static const std::string COULOMB;
    static const std::string E;
    static const std::string FARADAY;
    static const std::string FRANKLIN;
    static const std::string SIEMENS;
    static const std::string AMPERE;
    static const std::string GILBERT;
    static const std::string HENRY;
    static const std::string VOLT;
    static const std::string CENTIVOLT;
    static const std::string MILLIVOLT;
    static const std::string OHM;
    static const std::string JOULE;
    static const std::string ERG;
    static const std::string ELECTRON_VOLT;
    static const std::string NEWTON;
    static const std::string DYNE;
    static const std::string KILOGRAM_FORCE;
    static const std::string POUND_FORCE;
    static const std::string HERTZ;
    static const std::string MEGAHERTZ;
    static const std::string GIGAHERTZ;
    static const std::string LUX;
    static const std::string LAMBERT;
    static const std::string STOKE;
    static const std::string METRE;
    static const std::string KILOMETRE;
    static const std::string CENTIMETRE;
    static const std::string MILLIMETRE;
    static const std::string FOOT;
    static const std::string FOOT_SURVEY_US;
    static const std::string YARD;
    static const std::string INCH;
    static const std::string MILE;
    static const std::string NAUTICAL_MILE;
    static const std::string ANGSTROM;
    static const std::string ASTRONOMICAL_UNIT;
    static const std::string LIGHT_YEAR;
    static const std::string PARSEC;
    static const std::string POINT;
    static const std::string PIXEL;
    static const std::string LUMEN;
    static const std::string CANDELA;
    static const std::string WEBER;
    static const std::string MAXWELL;
    static const std::string TESLA;
    static const std::string GAUSS;
    static const std::string KILOGRAM;
    static const std::string GRAM;
    static const std::string ATOMIC_MASS;
    static const std::string ELECTRON_MASS;
    static const std::string POUND;
    static const std::string OUNCE;
    static const std::string TON_US;
    static const std::string TON_UK;
    static const std::string METRIC_TON;
    static const std::string WATT;
    static const std::string HORSEPOWER;
    static const std::string PASCAL;
    static const std::string HECTOPASCAL;
    static const std::string ATMOSPHERE;
    static const std::string BAR;
    static const std::string MILLIBAR;
    static const std::string MILLIMETER_OF_MERCURY;
    static const std::string INCH_OF_MERCURY;
    static const std::string GRAY;
    static const std::string RAD;
    static const std::string SIEVERT;
    static const std::string REM;
    static const std::string BECQUEREL;
    static const std::string CURIE;
    static const std::string RUTHERFORD;
    static const std::string ROENTGEN;
    static const std::string STERADIAN;
    static const std::string SPHERE;
    static const std::string KELVIN;
    static const std::string CELSIUS;
    static const std::string RANKINE;
    static const std::string FAHRENHEIT;
    static const std::string METRES_PER_SECOND;
    static const std::string MILES_PER_HOUR;
    static const std::string KILOMETRES_PER_HOUR;
    static const std::string KNOT;
    static const std::string MACH;
    static const std::string C;
    static const std::string CUBIC_METRE;
    static const std::string LITRE;
    static const std::string DECILITRE;
    static const std::string MILLILITRE;
    static const std::string CUBIC_INCH;
    static const std::string GALLON_DRY_US;
    static const std::string GALLON_UK;
    static const std::string OUNCE_LIQUID_UK;
    static const std::string UNKNOWN;

private:
    Unit() = default;
    virtual ~Unit() = 0;
};

enum class OutboundDataMode
{
    PUSH,
    PULL,
};

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
    FIRMWARE_UPDATE_REPOSITORY,
    FIRMWARE_VERSION,
    GATEWAY,
    GATEWAY_PARENT,
    EXTERNAL_ID,
    UNKNOWN,
};

std::string toString(ParameterName parameterName);
ParameterName parameterNameFromString(std::string parameterName);

using Parameter = std::pair<ParameterName, std::string>;

enum class MessageType
{
    FEED_VALUES,
    PULL_FEED_VALUES,
    FEED_REGISTRATION,
    FEED_REMOVAL,
    ATTRIBUTE_REGISTRATION,
    PARAMETER_SYNC,
    PULL_PARAMETERS,
    SYNCHRONIZE_PARAMETERS,
    DETAILS_SYNCHRONIZATION_REQUEST,
    DETAILS_SYNCHRONIZATION_RESPONSE,
    TIME_SYNC,
    FILE_UPLOAD_INIT,
    FILE_UPLOAD_STATUS,
    FILE_UPLOAD_ABORT,
    FILE_BINARY_REQUEST,
    FILE_BINARY_RESPONSE,
    FILE_URL_DOWNLOAD_INIT,
    FILE_URL_DOWNLOAD_ABORT,
    FILE_URL_DOWNLOAD_STATUS,
    FILE_LIST_REQUEST,
    FILE_LIST_RESPONSE,
    FILE_DELETE,
    FILE_PURGE,
    FIRMWARE_UPDATE_INSTALL,
    FIRMWARE_UPDATE_STATUS,
    FIRMWARE_UPDATE_ABORT,
    DEVICE_REGISTRATION,
    DEVICE_REGISTRATION_RESPONSE,
    DEVICE_REMOVAL,
    CHILDREN_SYNCHRONIZATION_REQUEST,
    CHILDREN_SYNCHRONIZATION_RESPONSE,
    REGISTERED_DEVICES_REQUEST,
    REGISTERED_DEVICES_RESPONSE,
    PLATFORM_CONNECTION_STATUS,
    GATEWAY_SUBDEVICE,
    ERROR,
    UNKNOWN,
};

std::string toString(MessageType type);
MessageType messageTypeFromString(const std::string& type);

enum class FileTransferStatus
{
    AWAITING_DEVICE,
    FILE_TRANSFER,
    FILE_READY,
    ERROR,
    ABORTED,
    UNKNOWN
};

std::string toString(wolkabout::FileTransferStatus status);

enum class FileTransferError
{
    NONE = -1,
    UNKNOWN,
    TRANSFER_PROTOCOL_DISABLED,
    UNSUPPORTED_FILE_SIZE,
    MALFORMED_URL,
    FILE_HASH_MISMATCH,
    FILE_SYSTEM_ERROR,
    RETRY_COUNT_EXCEEDED
};

std::string toString(wolkabout::FileTransferError error);

enum class DeviceType
{
    NONE = -1,
    STANDALONE,
    GATEWAY
};

enum class FirmwareUpdateStatus
{
    AWAITING_DEVICE,
    INSTALLING,
    SUCCESS,
    ERROR,
    ABORTED,
    UNKNOWN
};

std::string toString(wolkabout::FirmwareUpdateStatus status);

enum class FirmwareUpdateError
{
    NONE = -1,
    UNKNOWN,
    UNKNOWN_FILE,
    INSTALLATION_FAILED
};

std::string toString(wolkabout::FirmwareUpdateError error);

struct FileInformation
{
    std::string name;
    std::uint64_t size;
    std::string hash;
};

struct Location
{
    double latitude;
    double longitude;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_TYPES_H
