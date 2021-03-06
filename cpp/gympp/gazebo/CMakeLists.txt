# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT). All rights reserved.
# This software may be modified and distributed under the terms of the
# GNU Lesser General Public License v2.1 or any later version.

# ==================
# GAZEBO ENVIRONMENT
# ==================

add_library(GazeboEnvironment
    include/gympp/gazebo/GazeboEnvironment.h
    GazeboEnvironment.cpp)

target_link_libraries(GazeboEnvironment
    PUBLIC
    gympp
    GazeboSimulator
    PRIVATE
    Task
    TaskSingleton
    ScenarioGazebo
    ignition-gazebo3::core)

set_target_properties(GazeboEnvironment PROPERTIES
    PUBLIC_HEADER include/gympp/gazebo/GazeboEnvironment.h)

target_include_directories(GazeboEnvironment PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# ===========
# GYM FACTORY
# ===========

set(GYMFACTORY_PUBLIC_HEADERS
    include/gympp/gazebo/GymFactory.h
    include/gympp/gazebo/Metadata.h)

add_library(GymFactory
    ${GYMFACTORY_PUBLIC_HEADERS}
    GymFactory.cpp)

target_link_libraries(GymFactory
    PUBLIC
    gympp
    ScenarioGazebo
    GazeboSimulator
    GazeboEnvironment
    ignition-gazebo3::core)

set_target_properties(GymFactory PROPERTIES
    PUBLIC_HEADER
    "${GYMFACTORY_PUBLIC_HEADERS}")

target_include_directories(GymFactory PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# ===================
# INSTALL THE TARGETS
# ===================

if(NOT CMAKE_BUILD_TYPE STREQUAL "PyPI")
    install(
        TARGETS
        GazeboEnvironment
        GymFactory
        EXPORT gympp
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gympp/gazebo)
endif()
