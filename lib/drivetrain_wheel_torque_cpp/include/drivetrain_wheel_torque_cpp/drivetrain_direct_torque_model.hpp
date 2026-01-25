// Copyright 2026 Simon Sagmeister
#pragma once
#include <math.h>

#include <algorithm>
#include <array>
#include <eigen3/Eigen/Dense>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "drivetrain_model_base_cpp/base_class.hpp"
#include "drivetrain_wheel_torque_cpp/drivetrain_direct_torque_eqns.hpp"
#include "drivetrain_wheel_torque_cpp/states.hpp"
#include "param_management_cpp/base.hpp"
#include "param_management_cpp/param_reference_manager.hpp"
#include "tsl_logger_cpp/reference_logger.hpp"
#include "tum_types_cpp/common.hpp"
namespace tam::sim::drivetrain
{
// Wrapping class around the equations
class DrivetrainWheelTorqueModel
: public tam::interfaces::DrivetrainModelBase<
    drivetrain::DrivetrainEquationsDirectTorque::DriverInput,
    drivetrain::DrivetrainEquationsDirectTorque::Feedback, drivetrain::x::CNT_LENGTH_STATE_VECTOR,
    drivetrain::StateNamesTorque>
{
  using double_per_wheel_t = tam::types::common::DataPerWheel<double>;

public:
  DrivetrainWheelTorqueModel();
  // setters
  void set_x_vec(const StateVectorType & x_vec) override;
  void set_driver_input(const DriverInputType & input) override;
  void set_load(const double_per_wheel_t & drivetrain_load_torque_per_wheel_Nm) override;
  // model step function
  void evaluate();

  // getters
  double_per_wheel_t get_wheel_speeds() const override;
  FeedbackType get_feedback() const override;
  StateVectorType get_x_vec() const override;
  StateVectorType get_x_dot_vec() const override;
  tam::tsl::LoggerAccessInterface::SharedPtr get_logger() const override;
  tam::pmg::MgmtInterface::SharedPtr get_param_manager() const override;

private:
  DrivetrainEquationsDirectTorque eqns_;
  tam::tsl::ReferenceLogger::SharedPtr logger_ = std::make_shared<tam::tsl::ReferenceLogger>();
  pmg::ParamReferenceManager::SharedPtr param_manager_ =
    std::make_shared<pmg::ParamReferenceManager>();
  void declare_parameters();
  void register_log_signals();
};
}  // namespace tam::sim::drivetrain
