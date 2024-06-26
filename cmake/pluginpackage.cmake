######## install and distribution
install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/dist/${PROJECT_NAME}.ini"
        "${CMAKE_CURRENT_SOURCE_DIR}/dist/${PROJECT_NAME}.toml"
        CONFIGURATIONS Debug Release RelWithDebInfo
        DESTINATION ${PLUGIN_INSTALL_DEST} OPTIONAL)

if(PLUGIN_INSTALL_PDB)
        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
                CONFIGURATIONS Release Debug RelWithDebInfo
                DESTINATION ${PLUGIN_INSTALL_DEST} OPTIONAL)
endif()

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PLUGIN_INSTALL_DEST})

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dist/Data")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dist/Data/"
            DESTINATION ".")
endif()

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${PROJECT_LONG_NAME})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_PROJECT_VERSION}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
include(CPack)