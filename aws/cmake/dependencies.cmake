# Include the CMake modules
include(FetchContent)

# Define the AWS version
set(AWS_SDK_VERSION "1.8.173")

# Set the flags for AWS based on the CMake build type
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(ENABLE_UNITY_BUILD "If enabled, the SDK will be built using a single unified .cpp file for each service library.  Reduces the size of static library binaries on Windows and Linux" OFF)
    option(MINIMIZE_SIZE "If enabled, the SDK will be built via a unity aggregation process that results in smaller static libraries; additionally, release binaries will favor size optimizations over speed" OFF)
else (CMAKE_BUILD_TYPE STREQUAL "Release")
    option(ENABLE_UNITY_BUILD "If enabled, the SDK will be built using a single unified .cpp file for each service library.  Reduces the size of static library binaries on Windows and Linux" ON)
    option(MINIMIZE_SIZE "If enabled, the SDK will be built via a unity aggregation process that results in smaller static libraries; additionally, release binaries will favor size optimizations over speed" ON)
endif ()

# Set the options
option(ENABLE_TESTING "Flag to enable/disable building unit and integration tests" OFF)
set(BUILD_ONLY s3 CACHE STRING "A semi-colon delimited list of the projects to build")

# Add AWS
FetchContent_Declare(
        aws-sdk-cpp
        GIT_REPOSITORY https://github.com/aws/aws-sdk-cpp
        GIT_TAG ${AWS_SDK_VERSION}
)

# Make available
FetchContent_Populate(aws-sdk-cpp)
add_subdirectory(${aws-sdk-cpp_SOURCE_DIR} ${aws-sdk-cpp_BINARY_DIR})
