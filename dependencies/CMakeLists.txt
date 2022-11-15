include(ExternalProject)

set(SPDLOG_VERSION "1.8.5")
set(PAHO_MQTT_C_VERSION "1.3.8")
set(PAHO_MQTT_CPP_VERSION "1.2.0")
set(GTEST_VERSION "1.10.0")
set(POCO_VERSION "1.10.1")
set(AWS_SDK_VERSION "1.8.173")
set(NLOHMANN_JSON_VERSION "3.11.2")
set(JSON_SCHEMA_VERSION "2.1.0")

if (NOT TARGET spdlog)
    ExternalProject_Add(spdlog
            GIT_REPOSITORY "https://github.com/gabime/spdlog"
            GIT_TAG "v${SPDLOG_VERSION}"
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            )
endif ()

if (BUILD_CONNECTIVITY)
    ExternalProject_Add(pahomqttc
            GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.c"
            GIT_TAG "v${PAHO_MQTT_C_VERSION}"
            UPDATE_COMMAND ""
            CMAKE_ARGS
            -DPAHO_WITH_SSL=TRUE
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            )

    ExternalProject_Add(pahomqttcpp
            GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.cpp"
            GIT_TAG "v${PAHO_MQTT_CPP_VERSION}"
            UPDATE_COMMAND ""
            CMAKE_ARGS -DPAHO_WITH_SSL=TRUE
            -DPAHO_MQTT_C_INCLUDE_DIRS=${CMAKE_PREFIX_PATH}/include
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            DEPENDS pahomqttc
            )
endif ()

if (BUILD_GTEST)
    ExternalProject_Add(libgtest
            GIT_REPOSITORY "https://github.com/google/googletest"
            GIT_TAG "release-${GTEST_VERSION}"
            CMAKE_ARGS -DBUILD_SHARED_LIBS=ON
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            )
endif ()

if (BUILD_POCO)
    if (NOT DEFINED POCO_INSTALL_DIR)
        set(POCO_INSTALL_DIR ${CMAKE_PREFIX_PATH})
    endif ()

    ExternalProject_Add(libpoco
            GIT_REPOSITORY "https://github.com/pocoproject/poco"
            GIT_TAG "poco-${POCO_VERSION}-release"
            CMAKE_ARGS
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DDISABLE_CPP14=TRUE
            -DENABLE_ENCODINGS=${POCO_ENABLE_ENCODINGS}
            -DENABLE_ENCODINGS_COMPILER=${POCO_ENABLE_ENCODINGS_COMPILER}
            -DENABLE_XML=${POCO_ENABLE_XML}
            -DENABLE_JSON=${POCO_ENABLE_JSON}
            -DENABLE_MONGODB=${POCO_ENABLE_MONGODB}
            -DENABLE_REDIS=${POCO_ENABLE_REDIS}
            -DENABLE_PDF=${POCO_ENABLE_PDF}
            -DENABLE_UTIL=${POCO_ENABLE_UTIL}
            -DENABLE_NET=${POCO_ENABLE_NET}
            -DENABLE_NETSSL=${POCO_ENABLE_NETSSL}
            -DENABLE_NETSSL_WIN=${POCO_ENABLE_NETSSL_WIN}
            -DENABLE_CRYPTO=${POCO_ENABLE_CRYPTO}
            -DENABLE_DATA=${POCO_ENABLE_DATA}
            -DENABLE_DATA_SQLITE=${POCO_ENABLE_DATA_SQLITE}
            -DENABLE_DATA_MYSQL=${POCO_ENABLE_DATA_MYSQL}
            -DENABLE_DATA_ODBC=${POCO_ENABLE_DATA_ODBC}
            -DENABLE_SEVENZIP=${POCO_ENABLE_SEVENZIP}
            -DENABLE_ZIP=${POCO_ENABLE_ZIP}
            -DENABLE_APACHECONNECTOR=${POCO_ENABLE_APACHECONNECTOR}
            -DENABLE_CPPPARSER=${POCO_ENABLE_CPPPARSER}
            -DENABLE_POCODOC=${POCO_ENABLE_POCODOC}
            -DENABLE_PAGECOMPILER=${POCO_ENABLE_PAGECOMPILER}
            -DENABLE_PAGECOMPILER_FILE2PAGE=${POCO_ENABLE_PAGECOMPILER_FILE2PAGE}
            -DCMAKE_INSTALL_PREFIX=${POCO_INSTALL_DIR}
            -DCMAKE_INSTALL_RPATH=$ORIGIN
            )
endif ()

if (BUILD_AWS_LOG_UPLOADER)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(AWS_UNITY_BUILD "OFF")
        set(AWS_MINIMIZE_SIZE "OFF")
    else (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(AWS_UNITY_BUILD "ON")
        set(AWS_MINIMIZE_SIZE "ON")
    endif ()

    ExternalProject_Add(aws-sdk-cpp
            GIT_REPOSITORY "https://github.com/aws/aws-sdk-cpp"
            GIT_TAG "${AWS_SDK_VERSION}"
            CMAKE_ARGS
            -DBUILD_SHARED_LIBS=ON
            -DBUILD_ONLY=s3
            -DENABLE_UNITY_BUILD=${AWS_UNITY_BUILD}
            -DMINIMIZE_SIZE=${AWS_MINIMIZE_SIZE}
            -DENABLE_TESTING=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH})
endif ()

if (NOT TARGET nlohmann_json)
    ExternalProject_Add(nlohmann_json
            GIT_REPOSITORY "https://github.com/nlohmann/json"
            GIT_TAG "v${NLOHMANN_JSON_VERSION}"
            CMAKE_ARGS
            -DJSON_BuildTests=OFF
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH})
endif ()

if (NOT TARGET nlohmann_json_schema_validator)
    ExternalProject_Add(json-schema
            GIT_REPOSITORY "https://github.com/pboettch/json-schema-validator"
            GIT_TAG "${JSON_SCHEMA_VERSION}"
            CMAKE_ARGS
            -DBUILD_SHARED_LIBS=ON
            -DBUILD_TESTS=OFF
            -DBUILD_EXAMPLES=OFF
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH})
    add_dependencies(json-schema nlohmann_json)
endif ()
