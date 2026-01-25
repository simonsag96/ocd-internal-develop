cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME vehicle_model_nodes)
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
set(VEHICLE_MODELS direct_torque
)

foreach(pkg_name ${NODE_DEPENDENCIES})
  find_package(${pkg_name} REQUIRED)
endforeach()

set(COMPONENT_NAMES)
set(COMPONENT_PATHS)
set(EXECUTABLE_NAMES)

foreach(vehicle_model_name ${VEHICLE_MODELS})
  list(APPEND EXECUTABLE_NAMES ${vehicle_model_name}_node)
  list(APPEND COMPONENT_PATHS components/${vehicle_model_name}_node.cpp)
  list(APPEND COMPONENT_NAMES ${vehicle_model_name}Node)
endforeach()

add_library(${PROJECT_NAME}_components SHARED ${COMPONENT_PATHS})
find_package(rclcpp_components)

# target_include_directories(
#   ${PROJECT_NAME}_components
#   PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)

target_compile_options(${PROJECT_NAME}_components PRIVATE -fopenmp)


ament_target_dependencies(${PROJECT_NAME}_components rclcpp_components ${NODE_DEPENDENCIES})

foreach(vehicle_model_name ${VEHICLE_MODELS})
  rclcpp_components_register_node(
    ${PROJECT_NAME}_components
    PLUGIN ${vehicle_model_name}Node
    EXECUTABLE ${vehicle_model_name}_node
  )
endforeach()

ament_export_targets(export_${PROJECT_NAME}_components)

    
install(TARGETS ${PROJECT_NAME}_components
        EXPORT export_${PROJECT_NAME}_components
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_dependencies(rclcpp_components ${NODE_DEPENDENCIES}) # other dependencies so client does not need to call find_package on them

ament_package()