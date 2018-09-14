# "Librarize" Paho MQTT
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

include_directories(SYSTEM "${SSL_INCLUDE_DIR}")

file(GLOB_RECURSE PAHO_MQTT_SRC "${CMAKE_CURRENT_LIST_DIR}/*.c" "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

link_directories(${CMAKE_BINARY_DIR}/lib)

add_library(paho-mqtt SHARED ${PAHO_MQTT_SRC})
target_link_libraries(paho-mqtt ${SSL_LIBS} pthread)
set_target_properties(paho-mqtt PROPERTIES LINK_FLAGS "-Wl,-rpath,./")

target_include_directories(paho-mqtt PUBLIC ${CMAKE_CURRENT_LIST_DIR})
