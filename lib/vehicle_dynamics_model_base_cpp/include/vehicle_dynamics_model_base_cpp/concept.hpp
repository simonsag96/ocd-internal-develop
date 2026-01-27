// Copyright 2026 Simon Sagmeister
#pragma once

#include <concepts>

#include "vehicle_dynamics_model_base_cpp/base_class.hpp"
namespace tam::interfaces::concepts
{
/// @brief Concept for steering actuator model types
template <typename T>
concept VehicleDynamicsModel =
  std::derived_from<
    T, tam::interfaces::VehicleDynamicsModelBase<
         T::k_state_vector_length, typename T::StateNamesTrait>> &&
  (!std::is_abstract_v<T>);  // Check that all pure virtual functions are implemented by the derived
                             // class // NOLINT

}  // namespace tam::interfaces::concepts
