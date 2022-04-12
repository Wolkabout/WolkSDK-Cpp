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

DataType dataTypeFromString(std::string value)
{
    // Convert the value into uppercase
    std::transform(value.cbegin(), value.cend(), value.begin(), ::toupper);

    if (value == "BOOLEAN")
        return DataType::BOOLEAN;
    else if (value == "NUMERIC")
        return DataType::NUMERIC;
    else if (value == "STRING")
        return DataType::STRING;
    else if (value == "HEXADECIMAL")
        return DataType::HEXADECIMAL;
    else if (value == "LOCATION")
        return DataType::LOCATION;
    else if (value == "ENUM")
        return DataType::ENUM;
    else if (value == "VECTOR")
        return DataType::VECTOR;
    return DataType::NONE;
}

std::string toString(FeedType type)
{
    switch (type)
    {
    case FeedType::IN:
        return "IN";
    case FeedType::IN_OUT:
        return "IN_OUT";
    default:
        return "";
    }
}

FeedType feedTypeFromString(std::string value)
{
    // Convert the value into uppercase
    std::transform(value.cbegin(), value.cend(), value.begin(), ::toupper);

    // Check the value
    if (value == "IN")
        return FeedType::IN;
    else if (value == "IN_OUT")
        return FeedType::IN_OUT;
    return FeedType::NONE;
}

const std::string Unit::NUMERIC = "NUMERIC";
const std::string Unit::BOOLEAN = "BOOLEAN";
const std::string Unit::PERCENT = "PERCENT";
const std::string Unit::DECIBEL = "DECIBEL";
const std::string Unit::LOCATION = "LOCATION";
const std::string Unit::TEXT = "TEXT";
const std::string Unit::METRES_PER_SQUARE_SECOND = "METRES_PER_SQUARE_SECOND";
const std::string Unit::G = "G";
const std::string Unit::MOLE = "MOLE";
const std::string Unit::ATOM = "ATOM";
const std::string Unit::RADIAN = "RADIAN";
const std::string Unit::REVOLUTION = "REVOLUTION";
const std::string Unit::DEGREE_ANGLE = "DEGREE_ANGLE";
const std::string Unit::MINUTE_ANGLE = "MINUTE_ANGLE";
const std::string Unit::SECOND_ANGLE = "SECOND_ANGLE";
const std::string Unit::CENTIRADIAN = "CENTIRADIAN";
const std::string Unit::GRADE = "GRADE";
const std::string Unit::SQUARE_METRE = "SQUARE_METRE";
const std::string Unit::ARE = "ARE";
const std::string Unit::HECTARE = "HECTARE";
const std::string Unit::KATAL = "KATAL";
const std::string Unit::BIT = "BIT";
const std::string Unit::BYTE = "BYTE";
const std::string Unit::SECOND = "SECOND";
const std::string Unit::MINUTE = "MINUTE";
const std::string Unit::HOUR = "HOUR";
const std::string Unit::DAY = "DAY";
const std::string Unit::WEEK = "WEEK";
const std::string Unit::YEAR = "YEAR";
const std::string Unit::MONTH = "MONTH";
const std::string Unit::DAY_SIDEREAL = "DAY_SIDEREAL";
const std::string Unit::YEAR_SIDEREAL = "YEAR_SIDEREAL";
const std::string Unit::YEAR_CALENDAR = "YEAR_CALENDAR";
const std::string Unit::POISE = "POISE";
const std::string Unit::FARAD = "FARAD";
const std::string Unit::COULOMB = "COULOMB";
const std::string Unit::E = "E";
const std::string Unit::FARADAY = "FARADAY";
const std::string Unit::FRANKLIN = "FRANKLIN";
const std::string Unit::SIEMENS = "SIEMENS";
const std::string Unit::AMPERE = "AMPERE";
const std::string Unit::GILBERT = "GILBERT";
const std::string Unit::HENRY = "HENRY";
const std::string Unit::VOLT = "VOLT";
const std::string Unit::CENTIVOLT = "CENTIVOLT";
const std::string Unit::MILLIVOLT = "MILLIVOLT";
const std::string Unit::OHM = "OHM";
const std::string Unit::JOULE = "JOULE";
const std::string Unit::ERG = "ERG";
const std::string Unit::ELECTRON_VOLT = "ELECTRON_VOLT";
const std::string Unit::NEWTON = "NEWTON";
const std::string Unit::DYNE = "DYNE";
const std::string Unit::KILOGRAM_FORCE = "KILOGRAM_FORCE";
const std::string Unit::POUND_FORCE = "POUND_FORCE";
const std::string Unit::HERTZ = "HERTZ";
const std::string Unit::MEGAHERTZ = "MEGAHERTZ";
const std::string Unit::GIGAHERTZ = "GIGAHERTZ";
const std::string Unit::LUX = "LUX";
const std::string Unit::LAMBERT = "LAMBERT";
const std::string Unit::STOKE = "STOKE";
const std::string Unit::METRE = "METRE";
const std::string Unit::KILOMETRE = "KILOMETRE";
const std::string Unit::CENTIMETRE = "CENTIMETRE";
const std::string Unit::MILLIMETRE = "MILLIMETRE";
const std::string Unit::FOOT = "FOOT";
const std::string Unit::FOOT_SURVEY_US = "FOOT_SURVEY_US";
const std::string Unit::YARD = "YARD";
const std::string Unit::INCH = "INCH";
const std::string Unit::MILE = "MILE";
const std::string Unit::NAUTICAL_MILE = "NAUTICAL_MILE";
const std::string Unit::ANGSTROM = "ANGSTROM";
const std::string Unit::ASTRONOMICAL_UNIT = "ASTRONOMICAL_UNIT";
const std::string Unit::LIGHT_YEAR = "LIGHT_YEAR";
const std::string Unit::PARSEC = "PARSEC";
const std::string Unit::POINT = "POINT";
const std::string Unit::PIXEL = "PIXEL";
const std::string Unit::LUMEN = "LUMEN";
const std::string Unit::CANDELA = "CANDELA";
const std::string Unit::WEBER = "WEBER";
const std::string Unit::MAXWELL = "MAXWELL";
const std::string Unit::TESLA = "TESLA";
const std::string Unit::GAUSS = "GAUSS";
const std::string Unit::KILOGRAM = "KILOGRAM";
const std::string Unit::GRAM = "GRAM";
const std::string Unit::ATOMIC_MASS = "ATOMIC_MASS";
const std::string Unit::ELECTRON_MASS = "ELECTRON_MASS";
const std::string Unit::POUND = "POUND";
const std::string Unit::OUNCE = "OUNCE";
const std::string Unit::TON_US = "TON_US";
const std::string Unit::TON_UK = "TON_UK";
const std::string Unit::METRIC_TON = "METRIC_TON";
const std::string Unit::WATT = "WATT";
const std::string Unit::HORSEPOWER = "HORSEPOWER";
const std::string Unit::PASCAL = "PASCAL";
const std::string Unit::HECTOPASCAL = "HECTOPASCAL";
const std::string Unit::ATMOSPHERE = "ATMOSPHERE";
const std::string Unit::BAR = "BAR";
const std::string Unit::MILLIBAR = "MILLIBAR";
const std::string Unit::MILLIMETER_OF_MERCURY = "MILLIMETER_OF_MERCURY";
const std::string Unit::INCH_OF_MERCURY = "INCH_OF_MERCURY";
const std::string Unit::GRAY = "GRAY";
const std::string Unit::RAD = "RAD";
const std::string Unit::SIEVERT = "SIEVERT";
const std::string Unit::REM = "REM";
const std::string Unit::BECQUEREL = "BECQUEREL";
const std::string Unit::CURIE = "CURIE";
const std::string Unit::RUTHERFORD = "RUTHERFORD";
const std::string Unit::ROENTGEN = "ROENTGEN";
const std::string Unit::STERADIAN = "STERADIAN";
const std::string Unit::SPHERE = "SPHERE";
const std::string Unit::KELVIN = "KELVIN";
const std::string Unit::CELSIUS = "CELSIUS";
const std::string Unit::RANKINE = "RANKINE";
const std::string Unit::FAHRENHEIT = "FAHRENHEIT";
const std::string Unit::METRES_PER_SECOND = "METRES_PER_SECOND";
const std::string Unit::MILES_PER_HOUR = "MILES_PER_HOUR";
const std::string Unit::KILOMETRES_PER_HOUR = "KILOMETRES_PER_HOUR";
const std::string Unit::KNOT = "KNOT";
const std::string Unit::MACH = "MACH";
const std::string Unit::C = "C";
const std::string Unit::CUBIC_METRE = "CUBIC_METRE";
const std::string Unit::LITRE = "LITRE";
const std::string Unit::DECILITRE = "DECILITRE";
const std::string Unit::MILLILITRE = "MILLILITRE";
const std::string Unit::CUBIC_INCH = "CUBIC_INCH";
const std::string Unit::GALLON_DRY_US = "GALLON_DRY_US";
const std::string Unit::GALLON_UK = "GALLON_UK";
const std::string Unit::OUNCE_LIQUID_UK = "OUNCE_LIQUID_UK";
const std::string Unit::UNKNOWN = "UNKNOWN";

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
    case ParameterName::FIRMWARE_UPDATE_REPOSITORY:
        return "FIRMWARE_UPDATE_REPOSITORY";
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
    if (parameterName == "FIRMWARE_UPDATE_REPOSITORY")
        return ParameterName::FIRMWARE_UPDATE_REPOSITORY;
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
    case MessageType::SYNCHRONIZE_PARAMETERS:
        return "synchronize_parameters";
    case MessageType::DETAILS_SYNCHRONIZATION_REQUEST:
    case MessageType::DETAILS_SYNCHRONIZATION_RESPONSE:
        return "details_synchronization";
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
    case MessageType::DEVICE_REGISTRATION:
        return "device_registration";
    case MessageType::DEVICE_REGISTRATION_RESPONSE:
        return "device_registration_response";
    case MessageType::DEVICE_REMOVAL:
        return "device_removal";
    case MessageType::CHILDREN_SYNCHRONIZATION_REQUEST:
    case MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE:
        return "children_synchronization";
    case MessageType::REGISTERED_DEVICES_REQUEST:
    case MessageType::REGISTERED_DEVICES_RESPONSE:
        return "registered_devices";
    case MessageType::PLATFORM_CONNECTION_STATUS:
        return "connection_status";
    case MessageType::ERROR_MESSAGE:
        return "error";
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
    if (type == "synchronize_parameters")
        return MessageType::SYNCHRONIZE_PARAMETERS;
    if (type == "details_synchronization")
        return MessageType::DETAILS_SYNCHRONIZATION_RESPONSE;
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
    if (type == "device_registration")
        return MessageType::DEVICE_REGISTRATION;
    if (type == "device_registration_response")
        return MessageType::DEVICE_REGISTRATION_RESPONSE;
    if (type == "device_removal")
        return MessageType::DEVICE_REMOVAL;
    if (type == "children_synchronization")
        return MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE;
    if (type == "registered_devices")
        return MessageType::REGISTERED_DEVICES_RESPONSE;
    if (type == "connection_status")
        return MessageType::PLATFORM_CONNECTION_STATUS;
    if (type == "error")
        return MessageType::ERROR_MESSAGE;
    return MessageType::UNKNOWN;
}

std::string toString(wolkabout::FileTransferStatus status)
{
    switch (status)
    {
    case FileTransferStatus::AWAITING_DEVICE:
        return "AWAITING_DEVICE";
    case FileTransferStatus::FILE_TRANSFER:
        return "FILE_TRANSFER";
    case FileTransferStatus::FILE_READY:
        return "FILE_READY";
    case FileTransferStatus::ERROR_TRANSFER:
        return "ERROR";
    case FileTransferStatus::ABORTED:
        return "ABORTED";
    case FileTransferStatus::UNKNOWN:
        return "UNKNOWN";
    default:
        return "";
    }
}

std::string toString(wolkabout::FileTransferError error)
{
    switch (error)
    {
    case FileTransferError::UNKNOWN:
        return "UNKNOWN";
    case FileTransferError::TRANSFER_PROTOCOL_DISABLED:
        return "TRANSFER_PROTOCOL_DISABLED";
    case FileTransferError::UNSUPPORTED_FILE_SIZE:
        return "UNSUPPORTED_FILE_SIZE";
    case FileTransferError::MALFORMED_URL:
        return "MALFORMED_URL";
    case FileTransferError::FILE_HASH_MISMATCH:
        return "FILE_HASH_MISMATCH";
    case FileTransferError::FILE_SYSTEM_ERROR:
        return "FILE_SYSTEM_ERROR";
    case FileTransferError::RETRY_COUNT_EXCEEDED:
        return "RETRY_COUNT_EXCEEDED";
    default:
        return "";
    }
}

std::string toString(wolkabout::FirmwareUpdateStatus status)
{
    switch (status)
    {
    case FirmwareUpdateStatus::AWAITING_DEVICE:
        return "AWAITING_DEVICE";
    case FirmwareUpdateStatus::INSTALLING:
        return "INSTALLING";
    case FirmwareUpdateStatus::SUCCESS:
        return "SUCCESS";
    case FirmwareUpdateStatus::ERROR_UPDATE:
        return "ERROR";
    case FirmwareUpdateStatus::ABORTED:
        return "ABORTED";
    case FirmwareUpdateStatus::UNKNOWN:
        return "UNKNOWN";
    default:
        return "";
    }
}

std::string toString(wolkabout::FirmwareUpdateError error)
{
    switch (error)
    {
    case FirmwareUpdateError::UNKNOWN:
        return "UNKNOWN";
    case FirmwareUpdateError::UNKNOWN_FILE:
        return "UNKNOWN_FILE";
    case FirmwareUpdateError::INSTALLATION_FAILED:
        return "INSTALLATION_FAILED";
    default:
        return "";
    }
}
}    // namespace wolkabout
