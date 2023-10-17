# Include the util dependencies
include(cmake/dependencies.cmake)

# Create the library
add_library(${AWS_LOG_UPLOADER_TARGET_NAME})
add_library(${NAMESPACE}::${AWS_LOG_UPLOADER_TARGET_NAME} ALIAS ${AWS_LOG_UPLOADER_TARGET_NAME})

# Link the libraries
target_link_libraries(${AWS_LOG_UPLOADER_TARGET_NAME} ${UTIL_TARGET_NAME} aws-cpp-sdk-core aws-cpp-sdk-s3)

# Set some target properties
set_target_properties(${AWS_LOG_UPLOADER_TARGET_NAME} PROPERTIES OUTPUT_NAME "${NAMESPACE}${AWS_LOG_UPLOADER_TARGET_NAME}")
set_target_properties(${AWS_LOG_UPLOADER_TARGET_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
set_target_properties(${AWS_LOG_UPLOADER_TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${AWS_LOG_UPLOADER_TARGET_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

# Add the sources
target_sources(${AWS_LOG_UPLOADER_TARGET_NAME} PRIVATE core/log_uploader/AwsLogUploader.cpp)

# Add the headers
target_sources(${AWS_LOG_UPLOADER_TARGET_NAME} PRIVATE core/log_uploader/AwsLogUploader.h)

# Add the private include directories
target_include_directories(${AWS_LOG_UPLOADER_TARGET_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/${AWS_LOG_UPLOADER_SUBDIRECTORY_PATH})

# Add the public include directories
target_include_directories(${AWS_LOG_UPLOADER_TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${AWS_LOG_UPLOADER_SUBDIRECTORY_PATH}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
