cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME open_car_dynamics)
project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

set(NODE_DEPENDENCIES
  vehicle_model_node 
  rclcpp_components
  vehicle_model_cpp
  drivetrain_wheel_torque_cpp
  drivetrain_wheel_torque_communication_handler_cpp
  steering_actuator_pt1_cpp
  steering_actuator_pt1_communication_handler_cpp
  vehicle_dynamics_double_track_cpp
  tire_models_cpp
  aerodynamics_models_cpp
)
# Generate those automatically
set(VEHICLE_MODELS 
  WheelTorque__PT1__DoubleTrack__MF52__DefaultAerodynamics
)

foreach(pkg_name ${NODE_DEPENDENCIES})
  find_package(${pkg_name} REQUIRED)
endforeach()

set(COMPONENT_NAMES)
set(EXECUTABLE_NAMES)

foreach(vehicle_model_name ${VEHICLE_MODELS})
  list(APPEND EXECUTABLE_NAMES ${vehicle_model_name})
  list(APPEND COMPONENT_NAMES ${vehicle_model_name})
endforeach()

add_library(${PROJECT_NAME} SHARED components/components.cpp)
find_package(rclcpp_components)

target_compile_options(${PROJECT_NAME} PRIVATE -fopenmp)


ament_target_dependencies(${PROJECT_NAME} ${NODE_DEPENDENCIES})

foreach(vehicle_model_name ${VEHICLE_MODELS})
  rclcpp_components_register_node(
    ${PROJECT_NAME}
    PLUGIN  ${PROJECT_NAME}::${vehicle_model_name}
    EXECUTABLE ${vehicle_model_name}
  )
endforeach()

ament_export_targets(export_${PROJECT_NAME})

    
install(TARGETS ${PROJECT_NAME}
        EXPORT export_${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_dependencies(rclcpp_components ${NODE_DEPENDENCIES}) # other dependencies so client does not need to call find_package on them

ament_package()