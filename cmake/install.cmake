# Import the required modules
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Define the destination for public headers
set(PUBLIC_HEADERS_DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${SOURCE_ROOT_FOLDER})
string(TOLOWER ${PUBLIC_HEADERS_DESTINATION} PUBLIC_HEADERS_DESTINATION)

# Define the component names
set(REL_COMPONENT RELEASE)
set(DEV_COMPONENT DEV)

# Define the targets lists
set(TARGETS_LIST ${UTIL_TARGET_NAME} ${AWS_LOG_UPLOADER_TARGET_NAME} ${CONNECTIVITY_TARGET_NAME})
set(TARGETS_DIRECTORIES ${UTIL_SUBDIRECTORY_PATH} ${AWS_LOG_UPLOADER_SUBDIRECTORY_PATH} ${CONNECTIVITY_SUBDIRECTORY_PATH})

# Go through the targets
list(LENGTH TARGETS_LIST TARGETS_LENGTH)
math(EXPR TARGETS_LENGTH ${TARGETS_LENGTH}-1)
foreach (INDEX RANGE ${TARGETS_LENGTH})
    list(GET TARGETS_LIST ${INDEX} TARGET)
    list(GET TARGETS_DIRECTORIES ${INDEX} TARGET_DIRECTORY)

    if (NOT TARGET "${TARGET}")
        continue()
    endif ()

    # Create the install rule for the main target
    install(TARGETS ${TARGET}
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY
            COMPONENT ${REL_COMPONENT}
            NAMELINK_SKIP
            PUBLIC_HEADER
            DESTINATION ${PUBLIC_HEADERS_DESTINATION}
            INCLUDES DESTINATION ${PUBLIC_HEADERS_DESTINATION})

    # Install all the header files
    get_target_property(HEADER_FILES ${TARGET} SOURCES)
    list(FILTER HEADER_FILES INCLUDE REGEX .*.h$)
    list(TRANSFORM HEADER_FILES PREPEND ${TARGET_DIRECTORY}/)
    string(LENGTH ${TARGET_DIRECTORY}/${SOURCE_ROOT_FOLDER}/ SOURCE_ROOT_FOLDER_LENGTH)
    foreach (HEADER_FILE ${HEADER_FILES})
        string(FIND ${HEADER_FILE} / SLASH_POSITION REVERSE)
        math(EXPR SLASH_POSITION ${SLASH_POSITION}+1)
        string(SUBSTRING ${HEADER_FILE} 0 ${SLASH_POSITION} HEADER_SUBDIRECTORY)
        string(SUBSTRING ${HEADER_SUBDIRECTORY} ${SOURCE_ROOT_FOLDER_LENGTH} -1 HEADER_SUBDIRECTORY)
        install(FILES ${HEADER_FILE}
                COMPONENT ${DEV_COMPONENT}
                DESTINATION ${PUBLIC_HEADERS_DESTINATION}/${HEADER_SUBDIRECTORY})
    endforeach ()

    # Add the util extra targets
    if (${TARGET} STREQUAL ${UTIL_TARGET_NAME})
        install(TARGETS spdlog
                EXPORT ${PROJECT_NAME}Targets
                LIBRARY
                COMPONENT ${REL_COMPONENT}
                NAMELINK_SKIP
                PUBLIC_HEADER
                DESTINATION ${PUBLIC_HEADERS_DESTINATION}
                INCLUDES DESTINATION ${PUBLIC_HEADERS_DESTINATION})
    endif ()

    # Add the connectivity extra targets
    if (${TARGET} STREQUAL ${CONNECTIVITY_TARGET_NAME})
        # Install libpaho files
        install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/
                COMPONENT ${REL_COMPONENT}
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                FILES_MATCHING PATTERN REGEX "libpaho(.*).so")
        install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/
                COMPONENT ${REL_COMPONENT}
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                FILES_MATCHING PATTERN REGEX "(.*)[Pp]aho(.*).cmake")
        install(DIRECTORY ${CMAKE_BINARY_DIR}/include/
                COMPONENT ${DEV_COMPONENT}
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

        # Install the json schema validator target
        install(TARGETS nlohmann_json nlohmann_json_schema_validator
                EXPORT ${PROJECT_NAME}Targets
                LIBRARY
                COMPONENT ${REL_COMPONENT}
                NAMELINK_SKIP
                PUBLIC_HEADER
                DESTINATION ${PUBLIC_HEADERS_DESTINATION}
                INCLUDES DESTINATION ${PUBLIC_HEADERS_DESTINATION})
    endif ()

endforeach ()

# Create the config file
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${NAMESPACE}${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION cmake)

# Generate the version file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${NAMESPACE}${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion
)

# Expose the Targets.cmake in the CURRENT_BINARY_DIR
export(EXPORT ${PROJECT_NAME}Targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${NAMESPACE}${PROJECT_NAME}Targets.cmake"
        NAMESPACE ${NAMESPACE}::)

# Install the CMake files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${NAMESPACE}${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${NAMESPACE}${PROJECT_NAME}ConfigVersion.cmake"
        COMPONENT ${DEV_COMPONENT}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${NAMESPACE}${PROJECT_NAME}")
install(EXPORT
        ${PROJECT_NAME}Targets
        FILE
        "${NAMESPACE}${PROJECT_NAME}Targets.cmake"
        NAMESPACE ${NAMESPACE}::
        COMPONENT ${DEV_COMPONENT}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${NAMESPACE}${PROJECT_NAME}")
