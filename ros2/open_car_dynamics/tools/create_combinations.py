import argparse

# Define your model class names as strings
drivetrains = [
    ("DrivetrainWheelTorqueModel", "DrivetrainWheelTorqueCommunicationHandler"),
    # Add more (Drivetrain, DrivetrainCommHandler) pairs here if needed
]

steering_actuators = [
    ("PT1SteeringActuator", "SteeringActuatorPT1CommunicationHandler"),
    # Add more (SteeringActuator, SteeringActuatorCommHandler) pairs here if needed
]

vehicle_dynamics_models = [
    "VehicleDynamicsDoubleTrackModel",  # Only one as per your description
]

aerodynamics_models = [
    "DefaultAerodynamicsModel",
    # Add more aerodynamics models here if needed
]

tire_models = [
    "MF52",
    # Add more tire models here if needed
]


def main():
    parser = argparse.ArgumentParser(description="Generate component combinations.")
    parser.add_argument(
        "--names-only", action="store_true", help="Print only the component names"
    )
    args = parser.parse_args()

    for drivetrain, drivetrain_comm in drivetrains:
        for steering, steering_comm in steering_actuators:
            for vdm in vehicle_dynamics_models:
                for aero in aerodynamics_models:
                    for tire in tire_models:
                        name = f"{drivetrain}__{steering}__DoubleTrack__{tire}__{aero}"
                        if args.names_only:
                            print(name)
                        else:
                            print(f"OCD_NODE_COMPONENT(")
                            print(f"  {name},")
                            print(f"  tam::sim::drivetrain::{drivetrain},")
                            print(
                                f"  tam::sim::communication_handlers::{drivetrain_comm},"
                            )
                            print(f"  tam::sim::steering_actuator_pt1::{steering},")
                            print(
                                f"  tam::sim::communication_handlers::{steering_comm},"
                            )
                            print(f"  tam::sim::vd_double_track::{vdm},")
                            print(
                                f"  tam::sim::aerodynamics::{aero}, tam::sim::tire_models::{tire})\n"
                            )


if __name__ == "__main__":
    main()
