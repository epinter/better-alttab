vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO alandtse/CommonLibSSE-NG
        REF e7863a71523a2896c92ea9d3105c0d121dcdba0d
        SHA512  01bacd8c38f5bf9bea3c8c1a71ead2bef792d093884bd196eb28c19ac94f501b04ff82eaae68865a2572710afe1fbb5b72608338ccf189bab79117603a86946f
        HEAD_REF main
)

vcpkg_from_github(
    OUT_SOURCE_PATH SUBMODULE_PATH
    REPO ValveSoftware/openvr
    #commit is defined in alandtse/CommonLibSSE-NG/extern/openvr submodule
    REF 60eb187801956ad277f1cae6680e3a410ee0873b
    SHA512 bb85b4705e7095ac65df9969112b2df8930cee7917cc5f14231c5a0ffeed7a73ffa60727fd32f8786a403656f95a3ec0f80bf3ceabc5b8ede964aefb920bc718
)

#move openvr to extern
file(REMOVE_RECURSE "${SOURCE_PATH}/extern/openvr")
file(COPY "${SUBMODULE_PATH}/" DESTINATION "${SOURCE_PATH}/extern/openvr")

vcpkg_configure_cmake(
        SOURCE_PATH "${SOURCE_PATH}"
        PREFER_NINJA
        OPTIONS -DBUILD_TESTS=off -DSKSE_SUPPORT_XBYAK=on
)

vcpkg_install_cmake()
vcpkg_cmake_config_fixup(PACKAGE_NAME CommonLibSSE CONFIG_PATH lib/cmake)
vcpkg_copy_pdbs()

file(GLOB CMAKE_CONFIGS "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSE/*.cmake")
file(INSTALL ${CMAKE_CONFIGS} DESTINATION "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE")
file(INSTALL "${SOURCE_PATH}/cmake/CommonLibSSE.cmake" DESTINATION "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE")

#fix directxtk with portfile
set(CONFIG_FILE
    "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSEConfig.cmake"
)
file(READ "${CONFIG_FILE}" CONFIG_CONTENT)
file(WRITE "${CONFIG_FILE}"
    "include(CMakeFindDependencyMacro)\nfind_dependency(directxtk CONFIG)\n${CONFIG_CONTENT}"
)

#copy openvr headers
file(INSTALL
    "${SOURCE_PATH}/extern/openvr/headers/"
    DESTINATION "${CURRENT_PACKAGES_DIR}/include"
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSE")