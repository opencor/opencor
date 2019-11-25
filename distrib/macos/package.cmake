# Our different CPack generators require using different prefixes

if("${CPACK_GENERATOR}" STREQUAL "ZIP")
    set(CPACK_INSTALL_PREFIX /)
else()
    set(CPACK_INSTALL_PREFIX /Applications/OpenCOR)
endif()
