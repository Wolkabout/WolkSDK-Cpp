/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

#include "core/Types.h"

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
    if (type == "BOOLEAN")
        return DataType::BOOLEAN;
    if (type == "NUMERIC")
        return DataType::NUMERIC;
    if (type == "STRING")
        return DataType::STRING;
    if (type == "HEXADECIMAL")
        return DataType::HEXADECIMAL;
    if (type == "LOCATION")
        return DataType::LOCATION;
    if (type == "ENUM")
        return DataType::ENUM;
    if (type == "VECTOR")
        return DataType::VECTOR;
    return DataType::UNKNOWN;
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
    if (type == "IN")
        return FeedType::IN;
    if (type == "IN/OUT")
        return FeedType::IN_OUT;

    return FeedType::IN;
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
    default:
        return "";
    }
}

Unit unitFromString(const std::string& type)
{
    if (type == "NUMERIC")
        return Unit::NUMERIC;
    if (type == "BOOLEAN")
        return Unit::BOOLEAN;
    if (type == "PERCENT")
        return Unit::PERCENT;
    if (type == "DECIBEL")
        return Unit::DECIBEL;
    if (type == "LOCATION")
        return Unit::LOCATION;
    if (type == "METRES_PER_SQUARE_SECOND")
        return Unit::METRES_PER_SQUARE_SECOND;
    if (type == "G")
        return Unit::G;
    if (type == "MOLE")
        return Unit::MOLE;
    if (type == "ATOM")
        return Unit::ATOM;
    if (type == "RADIAN")
        return Unit::RADIAN;
    if (type == "REVOLUTION")
        return Unit::REVOLUTION;
    if (type == "DEGREE_ANGLE")
        return Unit::DEGREE_ANGLE;
    if (type == "MINUTE_ANGLE")
        return Unit::MINUTE_ANGLE;
    if (type == "SECOND_ANGLE")
        return Unit::SECOND_ANGLE;
    if (type == "CENTIRADIAN")
        return Unit::CENTIRADIAN;
    if (type == "GRADE")
        return Unit::GRADE;
    if (type == "SQUARE_METRE")
        return Unit::SQUARE_METRE;
    if (type == "ARE")
        return Unit::ARE;
    if (type == "HECTARE")
        return Unit::HECTARE;
    if (type == "KATAL")
        return Unit::KATAL;
    if (type == "BIT")
        return Unit::BIT;
    if (type == "BYTE")
        return Unit::BYTE;
    if (type == "SECOND")
        return Unit::SECOND;
    if (type == "MINUTE")
        return Unit::MINUTE;
    if (type == "HOUR")
        return Unit::HOUR;
    if (type == "DAY")
        return Unit::DAY;
    if (type == "WEEK")
        return Unit::WEEK;
    if (type == "YEAR")
        return Unit::YEAR;
    if (type == "MONTH")
        return Unit::MONTH;
    if (type == "DAY_SIDEREAL")
        return Unit::DAY_SIDEREAL;
    if (type == "YEAR_SIDEREAL")
        return Unit::YEAR_SIDEREAL;
    if (type == "YEAR_CALENDAR")
        return Unit::YEAR_CALENDAR;
    if (type == "POISE")
        return Unit::POISE;
    if (type == "FARAD")
        return Unit::FARAD;
    if (type == "COULOMB")
        return Unit::COULOMB;
    if (type == "E")
        return Unit::E;
    if (type == "FARADAY")
        return Unit::FARADAY;
    if (type == "FRANKLIN")
        return Unit::FRANKLIN;
    if (type == "SIEMENS")
        return Unit::SIEMENS;
    if (type == "AMPERE")
        return Unit::AMPERE;
    if (type == "GILBERT")
        return Unit::GILBERT;
    if (type == "HENRY")
        return Unit::HENRY;
    if (type == "VOLT")
        return Unit::VOLT;
    if (type == "CENTIVOLT")
        return Unit::CENTIVOLT;
    if (type == "MILLIVOLT")
        return Unit::MILLIVOLT;
    if (type == "OHM")
        return Unit::OHM;
    if (type == "JOULE")
        return Unit::JOULE;
    if (type == "ERG")
        return Unit::ERG;
    if (type == "ELECTRON_VOLT")
        return Unit::ELECTRON_VOLT;
    if (type == "NEWTON")
        return Unit::NEWTON;
    if (type == "DYNE")
        return Unit::DYNE;
    if (type == "KILOGRAM_FORCE")
        return Unit::KILOGRAM_FORCE;
    if (type == "POUND_FORCE")
        return Unit::POUND_FORCE;
    if (type == "HERTZ")
        return Unit::HERTZ;
    if (type == "MEGAHERTZ")
        return Unit::MEGAHERTZ;
    if (type == "GIGAHERTZ")
        return Unit::GIGAHERTZ;
    if (type == "LUX")
        return Unit::LUX;
    if (type == "LAMBERT")
        return Unit::LAMBERT;
    if (type == "STOKE")
        return Unit::STOKE;
    if (type == "METRE")
        return Unit::METRE;
    if (type == "KILOMETRE")
        return Unit::KILOMETRE;
    if (type == "CENTIMETRE")
        return Unit::CENTIMETRE;
    if (type == "MILLIMETRE")
        return Unit::MILLIMETRE;
    if (type == "FOOT")
        return Unit::FOOT;
    if (type == "FOOT_SURVEY_US")
        return Unit::FOOT_SURVEY_US;
    if (type == "YARD")
        return Unit::YARD;
    if (type == "INCH")
        return Unit::INCH;
    if (type == "MILE")
        return Unit::MILE;
    if (type == "NAUTICAL_MILE")
        return Unit::NAUTICAL_MILE;
    if (type == "ANGSTROM")
        return Unit::ANGSTROM;
    if (type == "ASTRONOMICAL_UNIT")
        return Unit::ASTRONOMICAL_UNIT;
    if (type == "LIGHT_YEAR")
        return Unit::LIGHT_YEAR;
    if (type == "PARSEC")
        return Unit::PARSEC;
    if (type == "POINT")
        return Unit::POINT;
    if (type == "PIXEL")
        return Unit::PIXEL;
    if (type == "LUMEN")
        return Unit::LUMEN;
    if (type == "CANDELA")
        return Unit::CANDELA;
    if (type == "WEBER")
        return Unit::WEBER;
    if (type == "MAXWELL")
        return Unit::MAXWELL;
    if (type == "TESLA")
        return Unit::TESLA;
    if (type == "GAUSS")
        return Unit::GAUSS;
    if (type == "KILOGRAM")
        return Unit::KILOGRAM;
    if (type == "GRAM")
        return Unit::GRAM;
    if (type == "ATOMIC_MASS")
        return Unit::ATOMIC_MASS;
    if (type == "ELECTRON_MASS")
        return Unit::ELECTRON_MASS;
    if (type == "POUND")
        return Unit::POUND;
    if (type == "OUNCE")
        return Unit::OUNCE;
    if (type == "TON_US")
        return Unit::TON_US;
    if (type == "TON_UK")
        return Unit::TON_UK;
    if (type == "METRIC_TON")
        return Unit::METRIC_TON;
    if (type == "WATT")
        return Unit::WATT;
    if (type == "HORSEPOWER")
        return Unit::HORSEPOWER;
    if (type == "PASCAL")
        return Unit::PASCAL;
    if (type == "HECTOPASCAL")
        return Unit::HECTOPASCAL;
    if (type == "ATMOSPHERE")
        return Unit::ATMOSPHERE;
    if (type == "BAR")
        return Unit::BAR;
    if (type == "MILLIBAR")
        return Unit::MILLIBAR;
    if (type == "MILLIMETER_OF_MERCURY")
        return Unit::MILLIMETER_OF_MERCURY;
    if (type == "INCH_OF_MERCURY")
        return Unit::INCH_OF_MERCURY;
    if (type == "GRAY")
        return Unit::GRAY;
    if (type == "RAD")
        return Unit::RAD;
    if (type == "SIEVERT")
        return Unit::SIEVERT;
    if (type == "REM")
        return Unit::REM;
    if (type == "BECQUEREL")
        return Unit::BECQUEREL;
    if (type == "CURIE")
        return Unit::CURIE;
    if (type == "RUTHERFORD")
        return Unit::RUTHERFORD;
    if (type == "ROENTGEN")
        return Unit::ROENTGEN;
    if (type == "STERADIAN")
        return Unit::STERADIAN;
    if (type == "SPHERE")
        return Unit::SPHERE;
    if (type == "KELVIN")
        return Unit::KELVIN;
    if (type == "CELSIUS")
        return Unit::CELSIUS;
    if (type == "RANKINE")
        return Unit::RANKINE;
    if (type == "FAHRENHEIT")
        return Unit::FAHRENHEIT;
    if (type == "METRES_PER_SECOND")
        return Unit::METRES_PER_SECOND;
    if (type == "MILES_PER_HOUR")
        return Unit::MILES_PER_HOUR;
    if (type == "KILOMETRES_PER_HOUR")
        return Unit::KILOMETRES_PER_HOUR;
    if (type == "KNOT")
        return Unit::KNOT;
    if (type == "MACH")
        return Unit::MACH;
    if (type == "C")
        return Unit::C;
    if (type == "CUBIC_METRE")
        return Unit::CUBIC_METRE;
    if (type == "LITRE")
        return Unit::LITRE;
    if (type == "DECILITRE")
        return Unit::DECILITRE;
    if (type == "MILLILITRE")
        return Unit::MILLILITRE;
    if (type == "CUBIC_INCH")
        return Unit::CUBIC_INCH;
    if (type == "GALLON_DRY_US")
        return Unit::GALLON_DRY_US;
    if (type == "GALLON_UK")
        return Unit::GALLON_UK;
    if (type == "OUNCE_LIQUID_UK")
        return Unit::OUNCE_LIQUID_UK;
    return Unit::UNKNOWN;
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
    if (type == "MQTT")
        return ConnectivityType::MQTT;
    if (type == "HTTP")
        return ConnectivityType::HTTP;
    if (type == "AMQP")
        return ConnectivityType::AMQP;
    if (type == "GATEWAY")
        return ConnectivityType::GATEWAY;
    return ConnectivityType::UNDEFINED;
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
    if (mode == "PUSH")
        return OutboundDataMode::PUSH;
    if (mode == "PULL")
        return OutboundDataMode::PULL;
    return OutboundDataMode::UNKNOWN;
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
    if (parameterName == "CONNECTIVITY_TYPE")
        return ParameterName::CONNECTIVITY_TYPE;
    if (parameterName == "OUTBOUND_DATA_MODE")
        return ParameterName::OUTBOUND_DATA_MODE;
    if (parameterName == "OUTBOUND_DATA_RETENTION_TIME")
        return ParameterName::OUTBOUND_DATA_RETENTION_TIME;
    if (parameterName == "MAXIMUM_MESSAGE_SIZE")
        return ParameterName::MAXIMUM_MESSAGE_SIZE;
    if (parameterName == "FILE_TRANSFER_PLATFORM_ENABLED")
        return ParameterName::FILE_TRANSFER_PLATFORM_ENABLED;
    if (parameterName == "FILE_TRANSFER_URL_ENABLED")
        return ParameterName::FILE_TRANSFER_URL_ENABLED;
    if (parameterName == "FIRMWARE_UPDATE_ENABLED")
        return ParameterName::FIRMWARE_UPDATE_ENABLED;
    if (parameterName == "FIRMWARE_UPDATE_CHECK_TIME")
        return ParameterName::FIRMWARE_UPDATE_CHECK_TIME;
    if (parameterName == "FIRMWARE_VERSION")
        return ParameterName::FIRMWARE_VERSION;
    if (parameterName == "GATEWAY")
        return ParameterName::GATEWAY;
    if (parameterName == "GATEWAY_PARENT")
        return ParameterName::GATEWAY_PARENT;
    if (parameterName == "EXTERNAL_ID")
        return ParameterName::EXTERNAL_ID;
    return ParameterName::UNKNOWN;
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
        return "file_upload_initiate";
    case MessageType::FILE_UPLOAD_STATUS:
        return "file_upload_status";
    case MessageType::FILE_UPLOAD_ABORT:
        return "file_upload_abort";
    case MessageType::FILE_BINARY_REQUEST:
        return "file_binary_request";
    case MessageType::FILE_BINARY_RESPONSE:
        return "file_binary_response";
    case MessageType::FILE_URL_DOWNLOAD_INIT:
        return "file_url_download_initiate";
    case MessageType::FILE_URL_DOWNLOAD_ABORT:
        return "file_url_download_abort";
    case MessageType::FILE_URL_DOWNLOAD_STATUS:
        return "file_url_download_status";
    case MessageType::FILE_LIST_REQUEST:
    case MessageType::FILE_LIST_RESPONSE:
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
    if (type == "feed_values")
        return MessageType::FEED_VALUES;
    if (type == "pull_feed_values")
        return MessageType::PULL_FEED_VALUES;
    if (type == "feed_registration")
        return MessageType::FEED_REGISTRATION;
    if (type == "feed_removal")
        return MessageType::FEED_REMOVAL;
    if (type == "attribute_registration")
        return MessageType::ATTRIBUTE_REGISTRATION;
    if (type == "parameters")
        return MessageType::PARAMETER_SYNC;
    if (type == "pull_parameters")
        return MessageType::PULL_PARAMETERS;
    if (type == "time")
        return MessageType::TIME_SYNC;
    if (type == "file_upload_initiate")
        return MessageType::FILE_UPLOAD_INIT;
    if (type == "file_upload_status")
        return MessageType::FILE_UPLOAD_STATUS;
    if (type == "file_upload_abort")
        return MessageType::FILE_UPLOAD_ABORT;
    if (type == "file_binary_request")
        return MessageType::FILE_BINARY_REQUEST;
    if (type == "file_binary_response")
        return MessageType::FILE_BINARY_RESPONSE;
    if (type == "file_url_download_initiate")
        return MessageType::FILE_URL_DOWNLOAD_INIT;
    if (type == "file_url_download_abort")
        return MessageType::FILE_URL_DOWNLOAD_ABORT;
    if (type == "file_url_download_status")
        return MessageType::FILE_URL_DOWNLOAD_STATUS;
    if (type == "file_list")
        return MessageType::FILE_LIST_REQUEST;
    if (type == "file_delete")
        return MessageType::FILE_DELETE;
    if (type == "file_purge")
        return MessageType::FILE_PURGE;
    if (type == "firmware_update_install")
        return MessageType::FIRMWARE_UPDATE_INSTALL;
    if (type == "firmware_update_status")
        return MessageType::FIRMWARE_UPDATE_STATUS;
    if (type == "firmware_update_abort")
        return MessageType::FIRMWARE_UPDATE_ABORT;
    if (type == "gateway_device_registration")
        return MessageType::GATEWAY_DEVICE_REGISTRATION;
    if (type == "gateway_device_removal")
        return MessageType::GATEWAY_DEVICE_REMOVAL;
    return MessageType::UNKNOWN;
}

std::string toString(wolkabout::FileUploadStatus status)
{
    switch (status)
    {
    case FileUploadStatus::AWAITING_DEVICE:
        return "AWAITING_DEVICE";
    case FileUploadStatus::FILE_TRANSFER:
        return "FILE_TRANSFER";
    case FileUploadStatus::FILE_READY:
        return "FILE_READY";
    case FileUploadStatus::ERROR:
        return "ERROR";
    case FileUploadStatus::ABORTED:
        return "ABORTED";
    case FileUploadStatus::UNKNOWN:
        return "UNKNOWN";
    default:
        return "";
    }
}

std::string toString(wolkabout::FileUploadError error)
{
    switch (error)
    {
    case FileUploadError::UNKNOWN:
        return "UNKNOWN";
    case FileUploadError::TRANSFER_PROTOCOL_DISABLED:
        return "TRANSFER_PROTOCOL_DISABLED";
    case FileUploadError::UNSUPPORTED_FILE_SIZE:
        return "UNSUPPORTED_FILE_SIZE";
    case FileUploadError::MALFORMED_URL:
        return "MALFORMED_URL";
    case FileUploadError::FILE_HASH_MISMATCH:
        return "FILE_HASH_MISMATCH";
    case FileUploadError::FILE_SYSTEM_ERROR:
        return "FILE_SYSTEM_ERROR";
    case FileUploadError::RETRY_COUNT_EXCEEDED:
        return "RETRY_COUNT_EXCEEDED";
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

Reading::Reading(std::string reference, std::string value, std::uint64_t rtcTimestamp)
: m_reference(reference), m_value(value), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(const Reading& reading)
: m_reference(reading.getReference()), m_value(reading.getStringValue()), m_timestamp(reading.getTimestamp())
{
}

void Reading::setTimestamp(std::uint64_t& timestamp) {}

float Reading::getNumericValue() const
{
    return std::stof(m_value);
}

bool Reading::getBoolValue() const
{
    return (m_value == "true");
}

int Reading::getHexValue() const
{
    std::istringstream iss(m_value);
    int result = 0;
    if ((iss >> std::hex >> result))
    {
        return result;
    }
    return 0;
}

Location Reading::getLocationValue() const
{
    auto location = Location{};
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

std::vector<float> Reading::getVectorValue() const
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
const std::uint64_t& Reading::getTimestamp() const
{
    return m_timestamp;
}

void Reading::setTimestamp(std::string timestamp)
{
    m_timestamp = std::strtoull(timestamp.c_str(), nullptr, 10);
}
}    // namespace wolkabout
