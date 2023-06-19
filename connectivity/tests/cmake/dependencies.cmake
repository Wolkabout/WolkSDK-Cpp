# Include the CMake modules
include(FetchContent)

# Define the gtest/gmock version
set(GTEST_VERSION "1.10.0")

# Add gtest/gmock
FetchContent_Declare(
        libgtest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-${GTEST_VERSION}
)

# Make available
FetchContent_Populate(libgtest)
add_subdirectory(${libgtest_SOURCE_DIR} ${libgtest_BINARY_DIR})

# Adjust the compile options for `nlohmann_json` to disable the compilation errors with our flags
target_compile_options(gtest PRIVATE "-Wno-error=maybe-uninitialized;-Wno-error=suggest-attribute=noreturn")
