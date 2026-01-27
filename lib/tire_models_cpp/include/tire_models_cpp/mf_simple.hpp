// Copyright 2026 Simon Sagmeister
#pragma once
#include <math.h>

#include <string>
#include <tum_types_cpp/common.hpp>

#include "param_management_cpp/param_reference_manager.hpp"
#include "tire_model_base_cpp/base_class.hpp"
#include "tire_model_base_cpp/concept.hpp"
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
// Check that the class fulfills the concept
// This checks if the base class was properly implemented without having to create an instance
static_assert(
  tam::interfaces::concepts::TireModel<MF_Simple>,
  "MF_Simple does not fulfill the TireModel concept");
}  // namespace tam::sim::tire_models
