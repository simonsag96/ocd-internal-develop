// Copyright 2026 Simon Sagmeister
#pragma once

#include <concepts>

#include "tire_model_base_cpp/base_class.hpp"
namespace tam::interfaces::concepts
{
/// @brief Concept for steering actuator model types
template <typename T>
concept TireModel = std::derived_from<T, tam::interfaces::TireModelBase>;
}  // namespace tam::interfaces::concepts
