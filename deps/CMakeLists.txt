# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT). All rights reserved.
# This software may be modified and distributed under the terms of the
# GNU Lesser General Public License v2.1 or any later version.

include(FetchContent)

# ===
# YCM
# ===

find_package(YCM QUIET)

if(NOT ${YCM_FOUND})

    FetchContent_Declare(
        ycm
        GIT_REPOSITORY https://github.com/robotology/ycm.git)

    FetchContent_GetProperties(ycm)

    if(NOT ycm_POPULATED)
        FetchContent_Populate(ycm)

        add_subdirectory(${ycm_SOURCE_DIR}
                         ${ycm_BINARY_DIR}
                         EXCLUDE_FROM_ALL)
    endif()

    set(CMAKE_MODULE_PATH
        "${CMAKE_MODULE_PATH};${ycm_SOURCE_DIR}/modules" PARENT_SCOPE)
else()
    set(CMAKE_MODULE_PATH
        "${CMAKE_MODULE_PATH};${YCM_MODULE_PATH}" PARENT_SCOPE)
endif()

# ====================
# TINY-PROCESS-LIBRARY
# ====================

find_package(tiny-process-library QUIET)

if(NOT ${tiny-process-library_FOUND})

    FetchContent_Declare(
        tinyprocesslibrary
        GIT_REPOSITORY https://gitlab.com/eidheim/tiny-process-library.git)

    FetchContent_GetProperties(tinyprocesslibrary)

    if(NOT tinyprocesslibrary_POPULATED)
        FetchContent_Populate(tinyprocesslibrary)

        # Disable building the tests
        set(BUILD_TESTING OFF)

        # Build the library as static
        set(default_build_type ${BUILD_SHARED_LIBS})
        set(BUILD_SHARED_LIBS OFF)
        add_subdirectory(${tinyprocesslibrary_SOURCE_DIR}
                         ${tinyprocesslibrary_BINARY_DIR}
                         EXCLUDE_FROM_ALL)
        set(BUILD_SHARED_LIBS ${default_build_type})

        set_target_properties(tiny-process-library PROPERTIES
            PUBLIC_HEADER ${tinyprocesslibrary_SOURCE_DIR}/process.hpp
            POSITION_INDEPENDENT_CODE ON)

        mark_as_advanced(BUILD_TESTING)
    endif()
endif()

# =====
# CLARA
# =====

add_library(Clara INTERFACE)
target_sources(Clara INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/clara/clara.hpp)
target_include_directories(Clara INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/clara>)
