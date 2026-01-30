// Copyright 2026 Simon Sagmeister
#pragma once

#include <concepts>

#include "ocd_communication_handler_base_cpp/base_class.hpp"
namespace tam::interfaces::concepts
{
/// @brief Concept for vehicle model types
template <typename T>
concept CommunicationHandler =
  // Enforce ctor: T(rclcpp::Node*)
  std::constructible_from<T, rclcpp::Node *> &&
  // Enforce inheritance with the right nested types
  std::derived_from<
    T, tam::interfaces::CommunicationHandlerBase<
         typename T::DriverInputType, typename T::FeedbackType, typename T::AuxiliaryInputType>>;
}  // namespace tam::interfaces::concepts
