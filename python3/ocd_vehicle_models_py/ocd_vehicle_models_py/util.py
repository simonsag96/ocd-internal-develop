# Copyright 2026 Simon Sagmeister
from .submodel_choices import (
    DrivetrainType,
    SteeringActuatorType,
    VehicleDynamicsType,
    TireType,
    AerodynamicsType,
)


def get_cpp_class_name(
    drivetrain: DrivetrainType,
    steering: SteeringActuatorType,
    vdm: VehicleDynamicsType,
    tire: TireType,
    aero: AerodynamicsType,
):
    return f"OCD_Vehicle_{drivetrain.value}__{steering.value}__{vdm.value}__{tire.value}__{aero.value}"
