PROJECT(libgit2Plugin)

MACRO(RETRIEVE_LIBGIT2_SETTINGS)
    # Retrieve some libgit2 settings

    IF(WIN32)
        SET(LIBGIT2_DEFINITIONS
            _CRT_NONSTDC_NO_DEPRECATE
            _CRT_SECURE_NO_DEPRECATE
            _SCL_SECURE_NO_WARNINGS
            _WIN32_WINNT=0x0501

            GIT_WINHTTP
            WIN32
            WIN32_SHA1
        )
    ELSEIF(APPLE)
        SET(LIBGIT2_DEFINITIONS
            GIT_COMMON_CRYPTO
            GIT_SSL
        )
    ELSE()
        SET(LIBGIT2_DEFINITIONS
            GIT_SSL
            OPENSSL_SHA1
        )
    ENDIF()

    LIST(APPEND LIBGIT2_DEFINITIONS
        _FILE_OFFSET_BITS=64

        GIT_THREADS
        NO_GZIP
        NO_VIZ
        STDC
    )

    IF(${ARCHITECTURE} EQUAL 32)
        LIST(APPEND LIBGIT2_DEFINITIONS
            GIT_ARCH_32
        )
    ELSE()
        LIST(APPEND LIBGIT2_DEFINITIONS
            GIT_ARCH_64
        )
    ENDIF()
ENDMACRO()

# Make sure that OpenSSL is available on Linux and OS X

IF(NOT WIN32)
    FIND_PACKAGE(OpenSSL REQUIRED)
ENDIF()

# Determine to which dependencies we must have access

SET(DEPENDENCIES_INCLUDE_DIRS
    deps/http-parser
    deps/zlib
)

IF(WIN32)
    LIST(APPEND DEPENDENCIES_INCLUDE_DIRS
        deps/regex
    )
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_LIBGIT2_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.dll c71fc7862f34f0d98e44f952c854f847525bb926)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.lib dd6e280e256162139a8454fc807653d3a7f4a9d0)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.dll 797a9038c8c556dcd5ecbfbbd72493637cca0f3d)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.lib 48cee789ce7ecab5b3baf8c7fddd3ee2182ebab5)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.dll 0a135237d0d12bb3adcdd4c689d0417bedd84d08)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.lib 580825915a4e7533319aeefd2d9cb1d3cb2db27a)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.dll 5e9a7014f3e909e2c5fa74f3826ffc32ee81f937)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libgit2.lib da6d11798c74081478ccc5e94760bc9d81e1261a)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} liblibgit2.dylib 61fe7a55c55a9ce9c3fbda7705a66e2741bfd414)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} liblibgit2.so 7c5f4fe27622dbf9e5cf610c64ee0699ee83d18b)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} liblibgit2.so bf3c709c698b378324a4ed44d5901fe7217844bd)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(LIBGIT2_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}libgit2${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(LIBGIT2_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}libgit2${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(libgit2
        INCLUDE_DIRS
            ${DEPENDENCIES_INCLUDE_DIRS}

            include
            src
        QT_LIBRARIES
            QtCore
    )
ELSE()
    # Retrieve some libgit2 settings

    RETRIEVE_LIBGIT2_SETTINGS()

    # Ignore some warnings on Windows and OS X

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4133 /wd4244 /wd4267")
        # Note: on Windows, some warnings get generated, but we have nothing to
        #       do with them...
    ELSEIF(APPLE)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
        # Note: on OS X, some warnings get generated, but we have nothing to do
        #       with them...
    ENDIF()

    # Determine which dependencies we must build

    SET(DEPENDENCIES_SOURCES
        deps/http-parser/http_parser.c

        deps/zlib/adler32.c
        deps/zlib/crc32.c
        deps/zlib/crc32.h
        deps/zlib/deflate.c
        deps/zlib/infback.c
        deps/zlib/inffast.c
        deps/zlib/inflate.c
        deps/zlib/inftrees.c
        deps/zlib/trees.c
        deps/zlib/zutil.c
    )

    IF(WIN32)
        LIST(APPEND DEPENDENCIES_SOURCES
            deps/regex/regex.c
        )
    ENDIF()

    # Determine which additional files to build, if any

    IF(WIN32)
        SET(ADDITIONAL_SOURCES
            src/hash/hash_win32.c

            src/win32/dir.c
            src/win32/error.c
            src/win32/findfile.c
            src/win32/map.c
            src/win32/path_w32.c
            src/win32/path_w32.h
            src/win32/posix_w32.c
            src/win32/precompiled.c
            src/win32/pthread.c
            src/win32/utf-conv.c
            src/win32/w32_util.c
        )
    ELSE()
        SET(ADDITIONAL_SOURCES
            src/unix/map.c
            src/unix/realpath.c
        )
    ENDIF()

    # Add the plugin

    SET(LIBGIT2_PLUGIN libgit2 PARENT_SCOPE)

    ADD_PLUGIN(libgit2
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            ${DEPENDENCIES_SOURCES}

            src/annotated_commit.c
            src/attr.c
            src/attr_file.c
            src/attrcache.c
            src/blame.c
            src/blame_git.c
            src/blob.c
            src/branch.c
            src/buf_text.c
            src/buffer.c
            src/cache.c
            src/checkout.c
            src/cherrypick.c
            src/clone.c
            src/commit.c
            src/commit_list.c
            src/config.c
            src/config_cache.c
            src/config_file.c
            src/crlf.c
            src/date.c
            src/delta-apply.c
            src/delta.c
            src/describe.c
            src/diff.c
            src/diff_driver.c
            src/diff_file.c
            src/diff_patch.c
            src/diff_print.c
            src/diff_stats.c
            src/diff_tform.c
            src/diff_xdiff.c
            src/errors.c
            src/fetch.c
            src/fetchhead.c
            src/filebuf.c
            src/fileops.c
            src/filter.c
            src/fnmatch.c
            src/global.c
            src/graph.c
            src/hash.c
            src/hashsig.c
            src/ident.c
            src/ignore.c
            src/index.c
            src/indexer.c
            src/iterator.c
            src/libgit2plugin.cpp
            src/merge.c
            src/merge_file.c
            src/message.c
            src/mwindow.c
            src/netops.c
            src/notes.c
            src/object.c
            src/object_api.c
            src/odb.c
            src/odb_loose.c
            src/odb_mempack.c
            src/odb_pack.c
            src/oid.c
            src/oidarray.c
            src/openssl_stream.c
            src/pack-objects.c
            src/pack.c
            src/path.c
            src/pathspec.c
            src/pool.c
            src/posix.c
            src/pqueue.c
            src/push.c
            src/rebase.c
            src/refdb.c
            src/refdb_fs.c
            src/reflog.c
            src/refs.c
            src/refspec.c
            src/remote.c
            src/repository.c
            src/reset.c
            src/revert.c
            src/revparse.c
            src/revwalk.c
            src/settings.c
            src/sha1_lookup.c
            src/signature.c
            src/socket_stream.c
            src/sortedcache.c
            src/stash.c
            src/status.c
            src/strmap.c
            src/submodule.c
            src/sysdir.c
            src/tag.c
            src/thread-utils.c
            src/trace.c
            src/transaction.c
            src/transport.c
            src/tree-cache.c
            src/tree.c
            src/tsort.c
            src/util.c
            src/vector.c
            src/zstream.c

            src/transports/auth_negotiate.c
            src/transports/auth.c
            src/transports/cred_helpers.c
            src/transports/cred.c
            src/transports/git.c
            src/transports/http.c
            src/transports/local.c
            src/transports/smart_pkt.c
            src/transports/smart_protocol.c
            src/transports/smart.c
            src/transports/ssh.c
            src/transports/winhttp.c

            src/xdiff/xdiffi.c
            src/xdiff/xemit.c
            src/xdiff/xhistogram.c
            src/xdiff/xmerge.c
            src/xdiff/xpatience.c
            src/xdiff/xprepare.c
            src/xdiff/xutils.c

            ${ADDITIONAL_SOURCES}
        HEADERS_MOC
            src/libgit2plugin.h
        INCLUDE_DIRS
            ${DEPENDENCIES_INCLUDE_DIRS}

            include
            src
        DEFINITIONS
            ${LIBGIT2_DEFINITIONS}
        QT_MODULES
            Core
        QT_LIBRARIES
            QtCore
        EXTERNAL_BINARIES
            ${OPENSSL_LIBRARIES}
    )
ENDIF()
