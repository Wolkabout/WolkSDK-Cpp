# Include the CMake modules
include(FetchContent)

# Add the pre-installed libraries
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# Define the spdlog version
set(SPDLOG_VERSION 1.8.5)

# Add spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v${SPDLOG_VERSION}
)

# Make available
FetchContent_MakeAvailable(spdlog)
