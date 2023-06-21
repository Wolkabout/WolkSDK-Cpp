# Include the CMake modules
include(ExternalProject)
include(FetchContent)

# Define the versions
set(PAHO_MQTT_C_VERSION 1.3.8)
set(PAHO_MQTT_CPP_VERSION 1.2.0)
set(NLOHMANN_JSON_VERSION 3.7.3)
set(JSON_SCHEMA_VERSION 2.1.0)

# Include PahoMQTT for both C and C++
ExternalProject_Add(pahomqttc
        GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.c"
        GIT_TAG v${PAHO_MQTT_C_VERSION}
        UPDATE_COMMAND ""
        CMAKE_ARGS
        # Options for pahomqttc
        -DPAHO_WITH_SSL=ON
        -DPAHO_BUILD_SHARED=ON
        -DPAHO_BUILD_STATIC=OFF
        -DPAHO_BUILD_DOCUMENTATION=OFF
        -DPAHO_BUILD_SAMPLES=OFF
        -DPAHO_BUILD_DEB_PACKAGE=OFF
        -DPAHO_ENABLE_TESTING=OFF
        -DPAHO_ENABLE_CPACK=OFF

        # Target path for installation
        -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}

        # In case the library needs to cross-compile
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DARP_TOOLCHAIN_ROOT=${ARP_TOOLCHAIN_ROOT}
        )

ExternalProject_Add(pahomqttcpp
        GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.cpp"
        GIT_TAG v${PAHO_MQTT_CPP_VERSION}
        UPDATE_COMMAND ""
        CMAKE_ARGS
        # Options for pahomqttcpp
        -DPAHO_WITH_SSL=ON
        -DPAHO_BUILD_SHARED=ON
        -DPAHO_BUILD_STATIC=OFF
        -DPAHO_BUILD_DOCUMENTATION=OFF
        -DPAHO_BUILD_SAMPLES=OFF
        -DPAHO_BUILD_DEB_PACKAGE=OFF
        -DPAHO_BUILD_TESTS=OFF

        # Target path for installation
        -DPAHO_MQTT_C_INCLUDE_DIRS=${CMAKE_PREFIX_PATH}/include
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}

        # In case the library needs to cross-compile
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DARP_TOOLCHAIN_ROOT=${ARP_TOOLCHAIN_ROOT}
        DEPENDS pahomqttc
        )

# Set JSON flags
option(JSON_BuildTests "Build the unit tests when BUILD_TESTING is enabled." OFF)

# Include nlohmann json and the schema)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v${NLOHMANN_JSON_VERSION}
)

# Save the flags, for reset after we run the cmake for schema
set(_BUILD_TESTS ${BUILD_TESTS})
set(_BUILD_EXAMPLES ${BUILD_EXAMPLES})
set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)

FetchContent_Declare(
        json-schema
        GIT_REPOSITORY https://github.com/pboettch/json-schema-validator
        GIT_TAG ${JSON_SCHEMA_VERSION}
)

# Make everything available
FetchContent_MakeAvailable(json json-schema)

# Reset the flags for schema
set(BUILD_TESTS ${_BUILD_TESTS} CACHE BOOL "Build tests" FORCE)
set(BUILD_EXAMPLES ${_BUILD_EXAMPLES} CACHE BOOL "Build examples" FORCE)

# Adjust the compile options for `nlohmann_json` to disable the shadow and pedantic
target_compile_options(nlohmann_json_schema_validator PRIVATE "-Wno-shadow;-Wno-pedantic")
