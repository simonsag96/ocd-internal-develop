// Copyright 2026 Simon Sagmeister
#pragma once

#include <concepts>

#include "vehicle_model_base_cpp/base_class.hpp"
namespace tam::interfaces::concepts
{
/// @brief Concept for vehicle model types
template <typename T>
concept VehicleModel = std::derived_from<
  T, tam::interfaces::VehicleModelBase<
       typename T::DrivetrainDriverInputType, typename T::DrivetrainFeedbackType,
       typename T::SteeringActuatorDriverInputType, typename T::SteeringActuatorFeedbackType,
       typename T::DrivetrainAuxiliaryInputType, typename T::SteeringActuatorAuxiliaryInputType>>;

}  // namespace tam::interfaces::concepts
