// Copyright 2026 Simon Sagmeister
#pragma once
#include <math.h>

#include <string>
#include <tum_types_cpp/common.hpp>

#include "param_management_cpp/param_reference_manager.hpp"
#include "tire_model_base_cpp/base_class.hpp"
namespace tam::sim::tire_models
{
// Linear
// =========================================================
class Linear : public tam::interfaces::TireModelBase
{
  struct Parameters
  {
    double C_sx;
    double C_alpha;
  } p_;

public:
  types::TireModelOutput evaluate(types::TireModelInput const & input) override;
  void declare_parameters(
    tam::pmg::ParamReferenceManager * param_manager, std::string name_prefix) override;
  void register_log_signals(tam::tsl::ReferenceLogger *, std::string = "") const override {}
};
}  // namespace tam::sim::tire_models
