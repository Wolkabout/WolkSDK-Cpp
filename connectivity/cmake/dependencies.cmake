# Include the CMake modules
include(ExternalProject)
include(FetchContent)

# Define the versions
set(PAHO_MQTT_C_VERSION "1.3.8")
set(PAHO_MQTT_CPP_VERSION "1.2.0")
set(NLOHMANN_JSON_VERSION "3.7.3")
set(JSON_SCHEMA_VERSION "2.1.0")

# Set the PahoMQTT flags
SET(PAHO_WITH_SSL TRUE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")
SET(PAHO_ENABLE_TESTING FALSE CACHE BOOL "Build tests and run")
SET(PAHO_ENABLE_CPACK FALSE CACHE BOOL "Enable CPack")

# Include PahoMQTT for both C and C++
ExternalProject_Add(pahomqttc
        GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.c"
        GIT_TAG "v${PAHO_MQTT_C_VERSION}"
        UPDATE_COMMAND ""
        CMAKE_ARGS
        -DPAHO_WITH_SSL=TRUE
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DARP_TOOLCHAIN_ROOT=${ARP_TOOLCHAIN_ROOT}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        )

ExternalProject_Add(pahomqttcpp
        GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.cpp"
        GIT_TAG "v${PAHO_MQTT_CPP_VERSION}"
        UPDATE_COMMAND ""
        CMAKE_ARGS -DPAHO_WITH_SSL=TRUE
        -DPAHO_MQTT_C_INCLUDE_DIRS=${CMAKE_PREFIX_PATH}/include
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DARP_TOOLCHAIN_ROOT=${ARP_TOOLCHAIN_ROOT}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        DEPENDS pahomqttc
        )

# Set JSON flags
option(JSON_BuildTests "Build the unit tests when BUILD_TESTING is enabled." OFF)

# Include nlohmann json and the schema)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v${NLOHMANN_JSON_VERSION}
)

FetchContent_Declare(
        json-schema
        GIT_REPOSITORY https://github.com/pboettch/json-schema-validator
        GIT_TAG ${JSON_SCHEMA_VERSION}
)

# Make everything available
FetchContent_MakeAvailable(nlohmann_json json-schema)

# Adjust the compile options for `nlohmann_json` to disable the shadow error
target_compile_options(nlohmann_json_schema_validator PRIVATE "-Wno-shadow;-Wno-pedantic")
