# Include the CMake modules
include(ExternalProject)
include(FetchContent)

# Define the versions
set(PAHO_MQTT_C_VERSION "1.3.8")
set(PAHO_MQTT_CPP_VERSION "1.2.0")
set(NLOHMANN_JSON_VERSION "3.7.3")
set(JSON_SCHEMA_VERSION "2.1.0")

# Include PahoMQTT for both C and C++
ExternalProject_Add(pahomqttc
        GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.c"
        GIT_TAG "v${PAHO_MQTT_C_VERSION}"
        UPDATE_COMMAND ""
        CMAKE_ARGS
        -DPAHO_WITH_SSL=TRUE
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DARP_TOOLCHAIN_ROOT=${ARP_TOOLCHAIN_ROOT}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
        )

ExternalProject_Add(pahomqttcpp
        GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.cpp"
        GIT_TAG "v${PAHO_MQTT_CPP_VERSION}"
        UPDATE_COMMAND ""
        CMAKE_ARGS
        -DPAHO_WITH_SSL=TRUE
        -DPAHO_MQTT_C_INCLUDE_DIRS=${CMAKE_BINARY_DIR}/include
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DARP_TOOLCHAIN_ROOT=${ARP_TOOLCHAIN_ROOT}
        -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
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
FetchContent_Populate(json)
FetchContent_Populate(json-schema)
add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
add_subdirectory(${json-schema_SOURCE_DIR} ${json-schema_BINARY_DIR})

# Reset the flags for schema
set(BUILD_TESTS ${_BUILD_TESTS} CACHE BOOL "Build tests" FORCE)
set(BUILD_EXAMPLES ${_BUILD_EXAMPLES} CACHE BOOL "Build examples" FORCE)

# Adjust the compile options for `nlohmann_json` to disable the shadow and pedantic
target_compile_options(nlohmann_json_schema_validator PRIVATE "-Wno-shadow;-Wno-pedantic")
