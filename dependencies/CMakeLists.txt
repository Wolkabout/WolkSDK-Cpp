include(ExternalProject)

set(PAHO_MQTT_C_VERSION "1.3.1")
set(PAHO_MQTT_CPP_VERSION "1.1")
set(GTEST_VERSION "1.10.0")
set(POCO_VERSION "1.9.4")

if (BUILD_PAHO_MQTT)
    ExternalProject_Add(pahomqttc
            GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.c"
            GIT_TAG "v${PAHO_MQTT_C_VERSION}"
            UPDATE_COMMAND ""
            CMAKE_ARGS -DPAHO_WITH_SSL=TRUE
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            )

    ExternalProject_Add(pahomqttcpp
            GIT_REPOSITORY "https://github.com/eclipse/paho.mqtt.cpp"
            GIT_TAG "v${PAHO_MQTT_CPP_VERSION}"
            UPDATE_COMMAND ""
            CMAKE_ARGS -DPAHO_WITH_SSL=TRUE
            -DPAHO_MQTT_C_INCLUDE_DIRS=${CMAKE_PREFIX_PATH}/include
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            DEPENDS pahomqttc
            )
endif ()

if (BUILD_GTEST)
    ExternalProject_Add(libgtest
            GIT_REPOSITORY "https://github.com/google/googletest"
            GIT_TAG "release-${GTEST_VERSION}"
            CMAKE_ARGS -DCMAKE_INSTALL_LIBDIR=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_LIBRARY_INCLUDE_DIRECTORY}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            )
endif ()

if (BUILD_POCO)
    ExternalProject_Add(libpoco
            GIT_REPOSITORY "https://github.com/pocoproject/poco"
            GIT_TAG "poco-${POCO_VERSION}-release"
            CMAKE_ARGS -DPOCO_STATIC=ON
            -DDISABLE_CPP14=TRUE
            -DENABLE_ENCODINGS=OFF
            -DENABLE_ENCODINGS_COMPILER=OFF
            -DENABLE_XML=OFF
            -DENABLE_JSON=OFF
            -DENABLE_MONGODB=OFF
            -DENABLE_REDIS=OFF
            -DENABLE_PDF=OFF
            -DENABLE_UTIL=OFF
            -DENABLE_NET=ON
            -DENABLE_NETSSL=ON
            -DENABLE_NETSSL_WIN=OFF
            -DENABLE_CRYPTO=ON
            -DENABLE_DATA=OFF
            -DENABLE_DATA_SQLITE=OFF
            -DENABLE_DATA_MYSQL=OFF
            -DENABLE_DATA_ODBC=OFF
            -DENABLE_SEVENZIP=OFF
            -DENABLE_ZIP=OFF
            -DENABLE_APACHECONNECTOR=OFF
            -DENABLE_CPPPARSER=OFF
            -DENABLE_POCODOC=OFF
            -DENABLE_PAGECOMPILER=OFF
            -DENABLE_PAGECOMPILER_FILE2PAGE=OFF
            -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
            )
endif ()