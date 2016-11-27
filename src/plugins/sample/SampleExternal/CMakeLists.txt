PROJECT(SampleExternalPlugin)

## From main CMakeLists.txt
set(USE_PREBUILT_SAMPLEEXTERNAL_PLUGIN ON)


# Ensure none of our variables are cached...

unset(SAMPLEEXTERNAL_FOUND CACHE)
unset(SAMPLEEXTERNAL_INCLUDE_DIRS CACHE)
unset(SAMPLEEXTERNAL_LIBRARIES CACHE)
unset(SAMPLEEXTERNAL_ROOT_DIR CACHE)
unset(SAMPLEEXTERNAL_VERSION CACHE)

set(SAMPLEEXTERNAL_VERSION "1.1")

set(SAMPLEEXTERNAL_ROOT_DIR "${PROJECT_SOURCE_DIR}/packages/${REMOTE_EXTERNAL_BINARIES_DIR}")

string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_ROOT_DIR ${SAMPLEEXTERNAL_ROOT_DIR})

if(WIN32)
    set(CHECKED_FILES lib/external.dll lib/external.lib)
elseif(APPLE)
    set(CHECKED_FILES lib/libexternal.1.1.dylib)
else()
    set(CHECKED_FILES lib/libexternal.1.1.so)
endif()


if(USE_PREBUILT_SAMPLEEXTERNAL_PLUGIN)

    # Retrieve the plugin's file(s)

    if(WIN32)
        if(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
                SampleExternal ${SAMPLEEXTERNAL_VERSION} f527db0389671fff19eb459b3201045bdd01e215
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 752ffc9f68a4dd7a7d922603f901941ec7d56a62
                    cf9af3753108556adb3c3e2eb4744d7f516cb1d2
                )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} SampleExternal sha...)
        endif()
    elseif(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
            SampleExternal ${SAMPLEEXTERNAL_VERSION} e39a5d247f157b76b0f072245ad419a79430ca77
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 8be9cfb3551db8a76b8fdd1eda2627a5f2ca63a6
            )
    else()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} SampleExternal sha...)
    endif()

else()

    include(cmake/BuildExternal.cmake)

    Build_External(${SAMPLEEXTERNAL_ROOT_DIR})

    if(APPLE)
        ## Make dylibs relative...
    endif()

    CREATE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
        SampleExternal ${SAMPLEEXTERNAL_VERSION}
        PACKAGED_FILES lib include
        CHECKED_FILES ${CHECKED_FILES}
        )

endif()

# Find the now built Sample

##find_package(SampleExternal MODULE REQUIRED)
set(SAMPLEEXTERNAL_FOUND TRUE)

if(SAMPLEEXTERNAL_FOUND)
    if(WIN32)
        set(SAMPLEEXTERNAL_EXTERNAL_LIBRARIES external.dll)
    elseif(APPLE)
        set(SAMPLEEXTERNAL_EXTERNAL_LIBRARIES libexternal.1.1.dylib)
    else()
        set(SAMPLEEXTERNAL_EXTERNAL_LIBRARIES libexternal.1.1.so)
    endif()
    set(SAMPLEEXTERNAL_EXTERNAL_LIBRARY_DIR ${SAMPLEEXTERNAL_ROOT_DIR}/lib)
    set(SAMPLEEXTERNAL_INCLUDE_DIRS ${SAMPLEEXTERNAL_ROOT_DIR}/include)
endif()

# Export the package settings

set(SAMPLEEXTERNAL_FOUND ${SAMPLEEXTERNAL_FOUND} PARENT_SCOPE)
set(SAMPLEEXTERNAL_INCLUDE_DIRS ${SAMPLEEXTERNAL_INCLUDE_DIRS} PARENT_SCOPE)
set(SAMPLEEXTERNAL_LIBRARIES "${SAMPLEEXTERNAL_EXTERNAL_LIBRARY_DIR}/${SAMPLEEXTERNAL_EXTERNAL_LIBRARIES}" PARENT_SCOPE)
set(SAMPLEEXTERNAL_ROOT_DIR ${SAMPLEEXTERNAL_ROOT_DIR} PARENT_SCOPE)
set(SAMPLEEXTERNAL_VERSION ${SAMPLEEXTERNAL_VERSION} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(SampleExternal
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/sampleexternalplugin.cpp
    HEADERS_MOC
        src/sampleexternalplugin.h
    INCLUDE_DIRS
        src
        ${SAMPLEEXTERNAL_INCLUDE_DIRS}
    QT_MODULES
        Core
    EXTERNAL_BINARIES
        ${SAMPLEEXTERNAL_EXTERNAL_LIBRARIES}
    EXTERNAL_BINARIES_DIR
        ${SAMPLEEXTERNAL_EXTERNAL_LIBRARY_DIR}
)
