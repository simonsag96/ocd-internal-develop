// Copyright 2026 Simon Sagmeister
#pragma once

#include <concepts>

#include "aerodynamics_model_base_cpp/base_class.hpp"
namespace tam::interfaces::concepts
{
/// @brief Concept for steering actuator model types
template <typename T>
concept AerodynamicsModel = std::derived_from<T, tam::interfaces::AerodynamicsModelBase>;
}  // namespace tam::interfaces::concepts
