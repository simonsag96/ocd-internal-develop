// Copyright 2026 Simon Sagmeister
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

#include "aerodynamics_models_cpp/default.hpp"
#include "drivetrain_wheel_torque_communication_handler_cpp/comm_handler.hpp"
#include "steering_actuator_pt1_communication_handler_cpp/comm_handler.hpp"
#include "tire_models_cpp/mf_52.hpp"
#include "vehicle_dynamics_double_track_cpp/vehicle_dynamics_model.hpp"
#include "vehicle_model_cpp/vehicle_model.hpp"
#include "vehicle_model_node/vehicle_model_node.hpp"


#define OCD_NODE_COMPONENT(                                                                        \
  ClassName, DT_T, DT_COMM_HANDLER_T, SA_T, SA_COMM_HANDLER_T, VD_T, AERO_T, TIRE_T)               \
  namespace open_car_dynamics                                                                      \
  {                                                                                                \
  class ClassName : public tam::sim::VehicleModelNode<                                             \
                      tam::sim::VehicleModel<DT_T, SA_T, VD_T<TIRE_T, AERO_T>>, DT_COMM_HANDLER_T, \
                      SA_COMM_HANDLER_T>                                                           \
  {                                                                                                \
  public:                                                                                          \
    explicit ClassName(const rclcpp::NodeOptions & options)                                        \
    : tam::sim::VehicleModelNode<                                                                  \
        tam::sim::VehicleModel<DT_T, SA_T, VD_T<TIRE_T, AERO_T>>, DT_COMM_HANDLER_T,               \
        SA_COMM_HANDLER_T>(                                                                        \
        std::make_unique<tam::sim::VehicleModel<DT_T, SA_T, VD_T<TIRE_T, AERO_T>>>(), options)     \
    {                                                                                              \
      this->reset();                                                                               \
    }                                                                                              \
  };                                                                                               \
  RCLCPP_COMPONENTS_REGISTER_NODE(open_car_dynamics::ClassName);                                   \
  }  // namespace open_car_dynamics

OCD_NODE_COMPONENT(
  WheelTorque__PT1__DoubleTrack__MF52__DefaultAerodynamics,
  tam::sim::drivetrain::DrivetrainWheelTorqueModel,
  tam::sim::communication_handlers::DrivetrainWheelTorqueCommunicationHandler,
  tam::sim::steering_actuator_pt1::PT1SteeringActuator,
  tam::sim::communication_handlers::SteeringActuatorPT1CommunicationHandler,
  tam::sim::vd_double_track::VehicleDynamicsDoubleTrackModel,
  tam::sim::aerodynamics::DefaultAerodynamicsModel, tam::sim::tire_models::MF52)
