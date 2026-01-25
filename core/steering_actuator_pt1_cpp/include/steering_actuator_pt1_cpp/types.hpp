// Copyright 2026 Simon Sagmeister
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
namespace tam::sim::steering_actuator_pt1
{
// Input to the steering actuator model
struct DriverInput
{
  double steering_angle_rad{0.0};
};
// Output of the steering actuator model
struct Feedback
{
  double steering_angle_rad{0.0};
};
#define STATE_LIST(X) X(position_rad)
namespace x
{
enum States {
#define X(name) name,
  STATE_LIST(X)
#undef X
    CNT_LENGTH_STATE_VECTOR
};
};  // namespace x
struct StateNamesTrait
{
  static constexpr std::array<
    std::string_view, static_cast<std::size_t>(x::States::CNT_LENGTH_STATE_VECTOR)>
    value = {
#define X(name) #name,
      STATE_LIST(X)
#undef X
  };
};
}  // namespace tam::sim::steering_actuator_pt1
