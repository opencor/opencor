macro(Build_External INSTALL_DIR)
    execute_process(COMMAND ./buildinstall.sh ${INSTALL_DIR}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/External
                    )
endmacro()
