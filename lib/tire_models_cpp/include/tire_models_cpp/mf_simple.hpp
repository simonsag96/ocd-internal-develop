// Copyright 2026 Simon Sagmeister
#pragma once
#include <math.h>

#include <string>
#include <tum_types_cpp/common.hpp>

#include "param_management_cpp/param_reference_manager.hpp"
#include "tire_model_base_cpp/base_class.hpp"
namespace tam::sim::tire_models
{
// MF Simple
// =========================================================
class MF_Simple : public tam::interfaces::TireModelBase
{
  friend class MF_Simple_Extended;
  struct Parameters
  {
    struct Coefficients
    {
      double B, C, D, E;
    };
    Coefficients lateral, longitudinal;
  } p_;
  static types::TireModelOutput evaluate_static(
    types::TireModelInput const & input, Parameters const & parameters);

public:
  types::TireModelOutput evaluate(types::TireModelInput const & input) override;
  void declare_parameters(
    tam::pmg::ParamReferenceManager * param_manager, std::string name_prefix) override;
  void register_log_signals(tam::tsl::ReferenceLogger *, std::string = "") const override {}
};
}  // namespace tam::sim::tire_models
