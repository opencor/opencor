MACRO(RETRIEVE_CLANG_SETTINGS)
    # Retrieve some Clang settings

    IF(MSVC)
        SET(CLANG_MSVC_DEFINITIONS
            _CRT_NONSTDC_NO_DEPRECATE
            _CRT_NONSTDC_NO_WARNINGS

            _CRT_SECURE_NO_DEPRECATE
            _CRT_SECURE_NO_WARNINGS

            _SCL_SECURE_NO_DEPRECATE
            _SCL_SECURE_NO_WARNINGS
        )
    ELSEIF(APPLE)
        SET(CLANG_MSVC_DEFINITIONS)
    ELSE()
        SET(CLANG_MSVC_DEFINITIONS
            __STDC_FORMAT_MACROS
        )
    ENDIF()

    SET(CLANG_DEFINITIONS
        ${CLANG_MSVC_DEFINITIONS}

        __STDC_CONSTANT_MACROS
        __STDC_LIMIT_MACROS
    )

    IF(MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4291 /wd4351 /wd4355 /wd4624 /wd4996")
        # Note: to build Clang generates quite a few warnings from MSVC, so
        #       disable them since we have nothing to do with them...
    ENDIF()
ENDMACRO()
