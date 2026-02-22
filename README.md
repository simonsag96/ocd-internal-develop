<div align="center">
    <img src="doc/ocd_banner_gpt52_dark.png" alt="ocd_logo" width="100%" style="margin-bottom: 30px;">
</div>

<div align="center" style="margin-bottom: 30px;">
<p>
Open Car Dynamics provides a comprehensive, modular, and highly efficient implementation of a vehicle's dynamic behavior. 
Following the philosophy of modeling vehicle behavior "in as much detail as necessary, but as simply as possible," the library drastically simplifies parametrization and adaptation to custom requirements. 
Written in modern <b>C++</b> for maximum performance, the library offers seamless integrations for <b>Python</b> and <b>ROS 2</b>. 
Furthermore, vehicle dynamics has been rigorously validated against data recorded with the AV21 autonomous racecar used in the [Indy Autonomous Challenge](https://www.indyautonomouschallenge.com/) to ensure simulation accuracy and reliability.
</p>
</div>

<div align="center" style="margin-bottom: 30px;">

[![C++](https://img.shields.io/badge/C%2B%2B-20-1e6293?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.18-1e6293?logo=cmake&logoColor=white)](https://cmake.org/)
[![Docker](https://img.shields.io/badge/Container-docker-1e6293.svg?&logo=docker&logoColor=white)](https://www.docker.com/)
[![Apache 2.0](https://img.shields.io/badge/License-Apache%202.0-1e6293.svg?&logo=apache&logoColor=white)](https://www.apache.org/licenses/LICENSE-2.0)
[![DOI](https://img.shields.io/badge/DOI-10.1109%2FIV55156.2024.10588858-1e6293.svg?&logo=doi&logoColor=white)](https://doi.org/10.1109/IV55156.2024.10588858)
<br>

[![Python](https://img.shields.io/badge/Python-3.12%20%7C%203.10-e3781f?logo=python&logoColor=white)](https://www.python.org/)
[![ROS 2](https://img.shields.io/badge/ROS_2-Humble%20%7C%20Jazzy-e3781f.svg?logo=ros&logoColor=white)](https://docs.ros.org/en/humble/)
![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04%20%7C%2022.04-e3781f?logo=ubuntu&logoColor=white)
</div>


--- 

### How it works

The model is designed to be an ordinary differential equation in state-space formulation. 
This state space model is solved using the Dormand Prince Scheme with a constant integration step size to enable real-time execution.

To achieve modularity, each vehicle model consists of 3 different submodels:
- Vehicle Dynamics
- Drivetrain
- Steering Actuator

The interfaces connecting the different models, are shown in the following figure:

<div align="center">
    <img src="doc/Model_Composition.drawio.svg" alt="ocd_logo", width="80%" style="margin-bottom: 30px;">
</div>


Furthermore, the vehicle dynamics models can incorporte submodels for modeling aerodynamcis and tire behavior.
However, these models are stateless which differs them from the 3 submodels mentioned above.

All of these 5 submodels components can be freely combined with each other enabling a vast number of different vehicle implementations.
With this design, our libary is able to model a lot of different vehicles without modifyfing the source code, also enabling rapid extension and collaboration. New variations of any submodel can be created by just inheriting and implementing the corresponding base class.

The submodels are automatically combined at compile time (hence the heavy templating in this library) by concatenating their
state vectors to form one big model.
Since combination happens at compile time, the compiler is able to heavily optimize the model makint it hihgly efficient. 
Even the most complex model currently inside the repository achieves 
a full simulation time step (using Dormand Prince~ode4 integration) with an execution 
time below 10us on our benchmark system (AMD Ryzen 9 7950X)


## Compiling and Running the Model

### 1. Clone Repository and Install Dependencies

First clone the repository using the command:

```bash
git clone --recursive https://github.com/TUMFTM/Open-Car-Dynamics.git
```

Then install the required dependencies:
```bash
sudo apt install libboost-dev libeigen3-dev build-essential cmake
```

### 2a. Compile Using CMake

Our build system is build on colcon and ament, the build tools of ROS2.
However, for building the open car dynamics libary, without having `ros2`/`colcon`/`ament` installed, we provide an extra CMakeLists.txt in the 
folder [cmake_build](./cmake_build/).

To build the project using cmake, just paste the following commands one after another into your terminal.

```bash
cd cmake_build
```
```bash
mkdir build && cd build
```
```bash
cmake ..
```
```bash
cmake --build .
```

For installing the libary run the following comamnd inside the the `<RepoRoot>/cmake_build/build` folder after building.
```bash
cmake --install . 
```

This create a folder under `<RepoRoot>/cmake_build/install`. This folder acts as an overlay.
To use the libary, just source the `<RepoRoot>/cmake_build/install/setup.sh` script in your shell.
After sourcing the library can be linked correctly.

To install the libary correctly, just source the file in your `.bashrc` file by running this command the **Root of your Repository**:
```bash
echo "source $PWD/cmake_build/install/setup.sh" >> ~/.bashrc
```

### 2b. Compile and Run the ROS 2 Nodes

For using the model in a ROS 2 environemnt, we provide a generic wrapper node which wraps 
certain vehicle model into as ROS 2 node. 

The ROS 2 packages can be compiled by first installing the required dependencies for building with ROS 2 installed:
```bash
sudo apt install libboost-dev ros-${ROS_DISTRO}-can-msgs ros-${ROS_DISTRO}-ros2-socketcan ros-${ROS_DISTRO}-geographic-msgs
```

Afterwards, you can just compile the correct packages using:
```bash
colcon build --packages-up-to ocd_vehicle_nodes_cpp --cmake-args -DCMAKE_BUILD_TYPE=Release
```

Afterwards just source your compiled with install folder and check the available `rclcpp components` by using the command
```bash
ros2 component types ocd_vehicle_nodes_cpp
```
 
Alternatively, you can directly run the nodes by starting their executable. 
Finding the executables can be done using the command
```bash
ros2 pkg executables ocd_vehicle_nodes_cpp
```

### 2c. Compile the python Bindings




### 2d. Compile the Python bindings without ros2 installed.

Make sure you have docker installed and working.
For the following commands, it is assumed you can run docker commands without sudo (your user should be in the docker group)
If not prepend sudo for any docker command.

First compile the bindings in the docker container using the following command in the root of this repository.

```bash
docker run  \
    --rm \
    -v "$PWD":"$PWD" \
    -w "$PWD" \
    -it \
    ros:$(if [ "$(lsb_release -rs)" = "22.04" ]; then echo humble; elif [ "$(lsb_release -rs)" = "24.04" ]; then echo jazzy; fi) \
    bash -c "   \
        source /opt/ros/${ROS_DISTRO}/setup.bash && \
        apt update && \
        apt install -y libboost-dev ros-${ROS_DISTRO}-can-msgs ros-${ROS_DISTRO}-ros2-socketcan ros-${ROS_DISTRO}-geographic-msgs && \
        colcon build --merge-install --packages-up-to ocd_vehicle_models_py"
```

This create a new folder `<Repository Root>/install`.

To use the compiled packages, just source the file `<Repository Root>/install/local_setup.sh` inside your `.bashrc` file.
You can do this quickly by running the following command in the root of this repo.

```bash
echo "source $PWD/install/local_setup.sh" >> ~/.bashrc
```

Be sure to create a new shell after modifying your `.bashrc` file.



## Parameters

All vehicle parameters can be adapted via the config file `./config/example_config.yml`.
Furthermore, most of the parameters are adaptable at runtime via the `ros2 param set` command.

Unfortunately, significant parts of the parametrization resembling the AV21 racecar are confidential.
Therefore, we can only provide a parametrization that resembles a generic single-seater race car equipped with a conventional on-road tire.

For tire parameters, we provide a set of MF52 parameters taken from https://github.com/TUMFTM/sim_vehicle_dynamics.
This parameter set resembles a sport focused road tire.


## Related Projects

When building this vehicle model, we initially took inspiration from the [CommonRoad Vehicle Models](https://gitlab.lrz.de/tum-cps/commonroad-vehicle-models) Project. 
However, we needed a real-time capable, modularized model for an autonomous race-car which is why this project was started.


## References

If you use Open Car Dynamics in your work please consider citing our paper [Analyzing the Impact of Simulation Fidelity on the Evaluation of Autonomous Driving Motion Control](https://ieeexplore.ieee.org/document/10588858/).
```
@INPROCEEDINGS{10588858,
  author={Sagmeister, Simon and Kounatidis, Panagiotis and Goblirsch, Sven and Lienkamp, Markus},
  booktitle={2024 IEEE Intelligent Vehicles Symposium (IV)}, 
  title={Analyzing the Impact of Simulation Fidelity on the Evaluation of Autonomous Driving Motion Control}, 
  year={2024},
  volume={},
  number={},
  pages={230-237},
  keywords={Measurement;Analytical models;Heuristic algorithms;Software algorithms;Approximation algorithms;Data models;Vehicle dynamics},
  doi={10.1109/IV55156.2024.10588858}}

```

### Core Developers
 - [Simon Sagmeister](mailto:simon.sagmeister@tum.de)
 - Simon Hoffmann | Implementation of parts of the ROS2 and some of the dependency functions
 - Georg Jank | Implementation of parts of the template structure of this repo.

### Acknowledgments

Several students contributed to the success of the project during their Bachelor's, Master's or Project Thesis.
 - Panagiotis Kounatidis | *Development of an initial version of this model as well as implementation of the tire model.*


Special thanks to my colleagues for the regular technical feedback and talks during the development phase of this model:
- Sven Goblirsch
- Frederik Werner


We gratefully acknowledge financial support by:
 - Deutsche Forschungsgemeinschaft (DFG, German Research Foundation) | Project Number - 469341384

