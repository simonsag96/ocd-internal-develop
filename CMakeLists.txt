cmake_minimum_required(VERSION 3.18)
project(open_car_dynamics LANGUAGES CXX)


find_package(Eigen3 REQUIRED)


# ------------------------------------------------------------------------------
# library
# ------------------------------------------------------------------------------
add_library(${PROJECT_NAME} SHARED)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ------------------------------------------------------------------------------
# List of packages
# Can be obtained with colcon list -p --packages-up-to <your_package>
# ------------------------------------------------------------------------------
set(PKG_DIRS
    lib/ocd_aerodynamics_model_base_cpp
    lib/ocd_aerodynamics_models_cpp
    lib/ocd_drivetrain_model_base_cpp
    lib/ocd_drivetrain_rwd_lsd_cpp
    lib/ocd_drivetrain_wheel_torque_cpp
    lib/ocd_steering_actuator_model_base_cpp
    lib/ocd_steering_actuator_pt1_cpp
    lib/ocd_tire_model_base_cpp
    lib/ocd_tire_models_cpp
    lib/ocd_types_cpp
    lib/ocd_vehicle_dynamics_double_track_cpp
    lib/ocd_vehicle_dynamics_model_base_cpp
    lib/ocd_vehicle_model_base_cpp
    lib/ocd_vehicle_model_cpp
    lib/vehicle_models_cpp
    dependencies/iac_common/submodules/parameter_management/param_management_cpp
    dependencies/iac_common/submodules/time_series_logging/tsl_logger_cpp
    dependencies/iac_common/submodules/time_series_logging/tsl_msgs
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
            ${CMAKE_CURRENT_SOURCE_DIR}/${PKG}/include
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


if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
