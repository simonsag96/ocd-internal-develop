
// Copyright 2026 Simon Sagmeister
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>

#include "tum_types_cpp/common.hpp"
namespace tam::sim::drivetrain
{
// Input to the steering actuator model
#define STATE_LIST(X) \
  X(omega_FL_radps)   \
  X(omega_FR_radps)   \
  X(omega_RL_radps)   \
  X(omega_RR_radps)
// Expaned defined state list to an enum and a trait for state names
namespace x
{
enum States {
#define X(name) name,
  STATE_LIST(X)
#undef X
    CNT_LENGTH_STATE_VECTOR
};
};  // namespace x
struct StateNamesTorque
{
  static constexpr std::array<
    std::string_view, static_cast<std::size_t>(x::States::CNT_LENGTH_STATE_VECTOR)>
    value = {
#define X(name) #name,
      STATE_LIST(X)
#undef X
  };
};
#undef STATE_LIST
}  // namespace tam::sim::drivetrain
