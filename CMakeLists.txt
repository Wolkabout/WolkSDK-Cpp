# Copyright 2023 Wolkabout Technology s.r.o.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Set the version policy
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
cmake_policy(SET CMP0048 NEW)

# Define the project
cmake_minimum_required(VERSION 3.11)
project(SDKCore VERSION 3.4.0)
set(PROJECT_VERSION_SUFFIX)
set(NAMESPACE Wolkabout)
set(EMAIL elab@wolkabout.com)
set(REPO_LINK https://github.com/Wolkabout/WolkUtil-CXX)
set(SOURCE_ROOT_FOLDER core)

# Define the names for all modules
set(UTIL_TARGET_NAME ${PROJECT_NAME}Util)
set(UTIL_SUBDIRECTORY_PATH util)
set(AWS_LOG_UPLOADER_TARGET_NAME ${PROJECT_NAME}AwsLogUploader)
set(AWS_LOG_UPLOADER_SUBDIRECTORY_PATH aws)
set(CONNECTIVITY_TARGET_NAME ${PROJECT_NAME})
set(CONNECTIVITY_SUBDIRECTORY_PATH connectivity)

# Options for our library
OPTION(BUILD_CONNECTIVITY "Build the library with Paho MQTT and allow MQTT connection to the platform." ON)
OPTION(BUILD_POCO "Build the library with Poco." ON)
OPTION(BUILD_GTEST "Build the library with GTest." ON)
OPTION(BUILD_AWS_LOG_UPLOADER "Build the library with AwsLogUploader." OFF)
OPTION(BUILD_TESTS "Build the unit tests" ON)
OPTION(INSTALL_TARGET "Add the installation target for the project." ON)

# Check the GTEST and TESTS option
if (NOT ${BUILD_GTEST} AND ${BUILD_TESTS})
    message(FATAL_ERROR "The library can not build TESTS without GTEST.")
endif ()
if (${BUILD_TESTS})
    enable_testing()
endif ()

# Add the poco options
include(cmake/poco-options.cmake)

# Set the paths
include(cmake/paths.cmake)

# Set up the C++ settings
include(cmake/cxx-settings.cmake)

# Add the util library
add_subdirectory(util)

# If AWS LogUploader should be built
if (${BUILD_AWS_LOG_UPLOADER})
    add_subdirectory(aws)
endif ()

# If Connectivity should be built
if (${BUILD_CONNECTIVITY})
    add_subdirectory(connectivity)
endif ()

# If install should be included
if (${INSTALL_TARGET})
    include(cmake/install.cmake)
endif ()
