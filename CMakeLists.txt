cmake_minimum_required(VERSION 3.25)

project(
    BetterAltTab
        VERSION 1.0.2
        LANGUAGES CXX)
set(PROJECT_LICENSE "LGPLv3")
set(PROJECT_FRIENDLY_NAME ${PROJECT_NAME})
set(PROJECT_LONG_NAME "Better AltTab")
set(PROJECT_DESCRIPTION ${PROJECT_LONG_NAME})
set(PROJECT_COPYRIGHT "Emerson Pinter")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)
include(addpluginsources)
include(pluginconfig)
include(plugintarget)
include(pluginpackage)

######## dependencies
find_library(
    DETOURS_LIBRARY
    detours
    PATHS "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/lib"
    NO_DEFAULT_PATH
    REQUIRED
)

target_include_directories(${PROJECT_NAME} PRIVATE ${DETOURS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DETOURS_LIBRARY})


