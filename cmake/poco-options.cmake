# Options for Poco
if (${BUILD_POCO})

    # The Utility bundle
    OPTION(POCO_BUILD_UTIL "" ON)
    if (${POCO_BUILD_UTIL})
        OPTION(POCO_ENABLE_UTIL "" ON)
    endif ()

    # The Network bundle
    OPTION(POCO_BUILD_NET "" OFF)
    if (${POCO_BUILD_NET})
        OPTION(POCO_ENABLE_NET "" ON)
        OPTION(POCO_ENABLE_NETSSL "" ON)
        OPTION(POCO_ENABLE_CRYPTO "" ON)
        if (WIN32)
            OPTION(POCO_ENABLE_NETSSL_WIN "" OFF)
        else ()
            OPTION(POCO_ENABLE_NETSSL_WIN "" ON)
        endif ()
    endif ()

    # The Data bundle
    OPTION(POCO_BUILD_DATA "" OFF)
    if (${POCO_BUILD_DATA})
        OPTION(POCO_ENABLE_DATA "" ON)
        OPTION(POCO_ENABLE_DATA_SQLITE "" ON)
        OPTION(POCO_ENABLE_DATA_MYSQL "" OFF)
        OPTION(POCO_ENABLE_DATA_ODBC "" ON)
    endif ()

    # The Encodings bundle
    OPTION(POCO_BUILD_ENCODINGS "" OFF)
    if (${POCO_BUILD_ENCODINGS})
        OPTION(POCO_ENABLE_ENCODINGS "" ON)
        OPTION(POCO_ENABLE_ENCODINGS_COMPILER "" ON)
        OPTION(POCO_ENABLE_XML "" ON)
        OPTION(POCO_ENABLE_JSON "" ON)
    endif ()

    # The MongoDB bundle
    OPTION(POCO_BUILD_MONGODB "" OFF)
    if (${POCO_BUILD_MONGODB})
        OPTION(POCO_ENABLE_MONGODB "" ON)
    endif ()

    # The Redis bundle
    OPTION(POCO_BUILD_REDIS "" OFF)
    if (${POCO_BUILD_REDIS})
        OPTION(POCO_ENABLE_REDIS "" ON)
    endif ()

    # The ZIP bundle
    OPTION(POCO_BUILD_ZIP "" OFF)
    if (${POCO_BUILD_ZIP})
        OPTION(POCO_ENABLE_SEVENZIP "" ON)
        OPTION(POCO_ENABLE_ZIP "" ON)
    endif ()

    # The PageCompiler bundle
    OPTION(POCO_BUILD_PAGECOMPILER "" OFF)
    if (${POCO_BUILD_PAGECOMPILER})
        OPTION(POCO_ENABLE_PAGECOMPILER "" ON)
        OPTION(POCO_ENABLE_PAGECOMPILER_FILE2PAGE "" ON)
    endif ()

    # The PDF bundle
    OPTION(POCO_BUILD_PDF "" OFF)
    if (${POCO_BUILD_PDF})
        OPTION(POCO_ENABLE_PDF "" ON)
    endif ()

    # The Apache connection bundle
    OPTION(POCO_BUILD_APACHECONNECTOR "" OFF)
    if (${POCO_BUILD_APACHECONNECTOR})
        OPTION(POCO_ENABLE_APACHECONNECTOR "" ON)
    endif ()

    # The CppParser bundle
    OPTION(POCO_BUILD_CPPPARSER "" OFF)
    if (${POCO_BUILD_CPPPARSER})
        OPTION(POCO_ENABLE_CPPPARSER "" ON)
    endif ()

    # The PocoDoc bundle
    OPTION(POCO_BUILD_POCODOC "" OFF)
    if (${POCO_BUILD_POCODOC})
        OPTION(POCO_ENABLE_POCODOC "" ON)
    endif ()
endif ()
