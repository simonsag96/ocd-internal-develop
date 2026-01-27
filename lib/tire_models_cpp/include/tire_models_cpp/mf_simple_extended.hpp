// Copyright 2026 Simon Sagmeister
#pragma once
#include <math.h>

#include <string>
#include <tum_types_cpp/common.hpp>

#include "param_management_cpp/param_reference_manager.hpp"
#include "tire_model_base_cpp/base_class.hpp"
#include "tire_model_base_cpp/concept.hpp"
#include "tire_models_cpp/mf_simple.hpp"
namespace tam::sim::tire_models
{
//
// MF Simple Extended
// =========================================================
class MF_Simple_Extended : public tam::interfaces::TireModelBase
{
  struct Parameters
  {
    double FNOMIN;
    double PDY2;
  } p_;

public:
  types::TireModelOutput evaluate(types::TireModelInput const & input) override;
  void declare_parameters(
    tam::pmg::ParamReferenceManager * param_manager, std::string name_prefix) override;
  void register_log_signals(tam::tsl::ReferenceLogger *, std::string = "") const override {}

private:
  MF_Simple mf_simple_;
};
// Check that the class fulfills the concept
// This checks if the base class was properly implemented without having to create an instance
static_assert(
  tam::interfaces::concepts::TireModel<MF_Simple_Extended>,
  "MF_Simple_Extended does not fulfill the TireModel concept");
}  // namespace tam::sim::tire_models
