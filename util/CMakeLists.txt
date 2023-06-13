# Include the util dependencies
include(cmake/dependencies.cmake)

# Create the library
add_library(${UTIL_TARGET_NAME})
add_library(${NAMESPACE}::${UTIL_TARGET_NAME} ALIAS ${UTIL_TARGET_NAME})

# Link the libraries
target_link_libraries(${UTIL_TARGET_NAME} PRIVATE OpenSSL::Crypto spdlog::spdlog Threads::Threads)

# Set some target properties
set_target_properties(${UTIL_TARGET_NAME} PROPERTIES OUTPUT_NAME "${NAMESPACE}${UTIL_TARGET_NAME}")
set_target_properties(${UTIL_TARGET_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
set_target_properties(${UTIL_TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${UTIL_TARGET_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

# Add the sources
target_sources(${UTIL_TARGET_NAME} PRIVATE core/utility/ByteUtils.cpp
        core/utility/CommandBuffer.cpp
        core/utility/FileSystemUtils.cpp
        core/utility/Logger.cpp
        core/utility/LogManager.cpp
        core/utility/StringUtils.cpp
        core/utility/Timer.cpp)

# Add the headers
target_sources(${UTIL_TARGET_NAME} PRIVATE core/utility/Buffer.h
        core/utility/ByteUtils.h
        core/utility/CommandBuffer.h
        core/utility/FileSystemUtils.h
        core/utility/Logger.h
        core/utility/LogManager.h
        core/utility/LogUploader.h
        core/utility/Service.h
        core/utility/StringUtils.h
        core/utility/Timer.h)

# Add the private include directories
target_include_directories(${UTIL_TARGET_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/${UTIL_SUBDIRECTORY_PATH})

# Add the public include directories
target_include_directories(${UTIL_TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${UTIL_SUBDIRECTORY_PATH}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
