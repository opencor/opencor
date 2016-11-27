macro(Build_External INSTALL_DIR)
    if(WIN32)
        set(BUILD_PROCESS buildinstall.bat)
    else()
        set(BUILD_PROCESS ./buildinstall.sh)
    endif()
    file(TO_NATIVE_PATH ${INSTALL_DIR} INSTALL_PATH)
    execute_process(COMMAND ${BUILD_PROCESS} ${INSTALL_PATH}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/External
                    RESULT_VARIABLE EXECUTE_RESULT
                    OUTPUT_QUIET
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to build: ${EXECUTE_RESULT}")
    endif()
endmacro()
