// Copyright 2026 Simon Sagmeister
#pragma once
#include <pybind11/pybind11.h>

#include <type_traits>

#include "ocd_aerodynamics_models_cpp/default.hpp"
#include "ocd_aerodynamics_models_cpp/ride_height.hpp"
#include "ocd_drivetrain_fx_cpp/drivetrain_fx_model.hpp"
#include "ocd_drivetrain_wheel_torque_cpp/drivetrain_direct_torque_model.hpp"
#include "ocd_drivetrain_rwd_lsd_cpp/drivetrain_rwd_lsd_model.hpp"
#include "ocd_steering_actuator_pt1_cpp/steering_actuator_pt1_model.hpp"
#include "ocd_tire_models_cpp/linear.hpp"
#include "ocd_tire_models_cpp/mf_52.hpp"
#include "ocd_tire_models_cpp/mf_simple.hpp"
#include "ocd_tire_models_cpp/mf_simple_extended.hpp"
#include "ocd_vehicle_dynamics_double_track_cpp/vehicle_dynamics_model.hpp"
#include "ocd_vehicle_model_cpp/vehicle_model.hpp"

namespace py = pybind11;
// Helper functions that only participate in overload resolution when the method exists
template <typename VehicleModel, typename AuxType>
auto bind_drivetrain_aux_if_exists(py::class_<VehicleModel> & cls)
  -> decltype(std::declval<VehicleModel>().set_auxiliary_input_drivetrain(std::declval<AuxType>()), void()) // NOLINT
{
  cls.def("set_auxiliary_input_drivetrain", &VehicleModel::set_auxiliary_input_drivetrain);
}
template <typename VehicleModel, typename AuxType>
void bind_drivetrain_aux_if_exists(...)
{
  // Do nothing if the method doesn't exist
}
template <typename VehicleModel, typename AuxType>
auto bind_steering_aux_if_exists(py::class_<VehicleModel> & cls)
  -> decltype(std::declval<VehicleModel>().set_auxiliary_input_steering_actuator(std::declval<AuxType>()), void()) // NOLINT
{
  cls.def(
    "set_auxiliary_input_steering_actuator", &VehicleModel::set_auxiliary_input_steering_actuator);
}
template <typename VehicleModel, typename AuxType>
void bind_steering_aux_if_exists(...)
{
  // Do nothing if the method doesn't exist
}
// Macro to bind a vehicle model with conditional auxiliary input binding
#define _BIND_VEHICLE_MODEL_CLASS(m, VehicleModelClass, class_name)                                \
  do {                                                                                             \
    auto cls =                                                                                     \
      py::class_<VehicleModelClass>(m, class_name)                                                 \
        .def(py::init<>())                                                                         \
        .def("step", &VehicleModelClass::step)                                                     \
        .def("set_drivetrain_input", &VehicleModelClass::set_drivetrain_input)                     \
        .def("get_drivetrain_feedback", &VehicleModelClass::get_drivetrain_feedback)               \
        .def("set_steering_input", &VehicleModelClass::set_steering_input)                         \
        .def("get_steering_actuator_feedback", &VehicleModelClass::get_steering_actuator_feedback) \
        .def("set_external_influences", &VehicleModelClass::set_external_influences)               \
        .def("get_vehicle_model_output", &VehicleModelClass::get_vehicle_model_output)             \
        .def("get_logger", &VehicleModelClass::get_logger)                                         \
        .def("get_param_manager", &VehicleModelClass::get_param_manager)                           \
        .def("reset", &VehicleModelClass::reset);                                                  \
                                                                                                   \
    /* Use SFINAE-based helpers to conditionally bind auxiliary inputs */                          \
    bind_drivetrain_aux_if_exists<                                                                 \
      VehicleModelClass, VehicleModelClass::DrivetrainAuxiliaryInputType>(cls);                    \
    bind_steering_aux_if_exists<                                                                   \
      VehicleModelClass, VehicleModelClass::SteeringActuatorAuxiliaryInputType>(cls);              \
  } while (0)

#define BIND_OCD_VEHICLE_MODEL(m, ClassName, DT_T, SA_T, VD_T, TIRE_T, AERO_T) \
  using ClassName = tam::ocd::VehicleModel<DT_T, SA_T, VD_T<TIRE_T, AERO_T>>; \
  _BIND_VEHICLE_MODEL_CLASS(m, ClassName, #ClassName)
