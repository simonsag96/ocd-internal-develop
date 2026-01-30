cmake_minimum_required(VERSION 3.18)

# Force local install directory inside the repository (overrides any cached value)

project(open_car_dynamics_cpp VERSION 1.0 LANGUAGES CXX)


find_package(Eigen3 REQUIRED)


# ------------------------------------------------------------------------------
# library
# ------------------------------------------------------------------------------
add_library(${PROJECT_NAME} SHARED)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ----------------------------------------------------------------------------
# List of packages
# Packages directly inside `lib/` are discovered automatically. Additional
# dependency packages located elsewhere are appended below.
# ------------------------------------------------------------------------------

# Find direct children of lib/ (e.g. lib/ocd_...)
file(GLOB LIB_PKGS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "lib/*")
set(PKG_DIRS "")
foreach(PKG ${LIB_PKGS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PKG}")
        list(APPEND PKG_DIRS ${PKG})
    endif()
endforeach()

# Additional packages not located directly under lib/
list(APPEND PKG_DIRS
    dependencies/iac_common/submodules/parameter_management/param_management_cpp
    dependencies/iac_common/submodules/time_series_logging/tsl_logger_cpp
    dependencies/iac_common/tum_helpers_cpp
    dependencies/iac_common/tum_types_cpp
    dependencies/iac_common/vendor_pkgs/matplotlib_cpp
)

# ------------------------------------------------------------------------------
# Include directories
# ------------------------------------------------------------------------------
foreach(PKG ${PKG_DIRS})
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${PKG}/include>
            $<INSTALL_INTERFACE:include>
    )
endforeach()

# ------------------------------------------------------------------------------
# Gather all source files
# ------------------------------------------------------------------------------
set(SRC_FILES "")
foreach(PKG ${PKG_DIRS})
    file(GLOB_RECURSE PKG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${PKG}/src/*.cpp")
    list(APPEND SRC_FILES ${PKG_SRC})
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})


# ------------------------------------------------------------------------------
# Link dependencies
# ------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

# ------------------------------------------------------------------------------
# Compiler settings
# ------------------------------------------------------------------------------
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# For now, we keep warnings only for tum_helpers_cpp
target_compile_definitions(${PROJECT_NAME}
  PRIVATE BUILDING_CURVILINEAR_COSY
)

# ----------------------------------------------------------------------------
# Project Settings
# ----------------------------------------------------------------------------

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME ${PROJECT_NAME}
)


if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


# ------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------
# This makes `cmake --install` write into <repo>/install by default.
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install path" FORCE)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
)
# Install the targets for find_package to work downstream
install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)
# Install all the headers
foreach(PKG ${PKG_DIRS})  
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PKG}/include/
        DESTINATION include
    )
endforeach()

# ------------------------------------------------------------------------------
# Optional: Generate and install Config and Version files for find_package
# ------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

# ConfigVersion file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Config file template (create cmake/Config.cmake.in)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

# Install the Config and Version files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# ------------------------------------------------------------------------------
# Install the setup.sh
# ------------------------------------------------------------------------------

install(FILES cmake/setup.sh.in
        DESTINATION .
        RENAME setup.sh
        PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE)