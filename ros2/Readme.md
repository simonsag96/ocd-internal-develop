# Using OCD in ROS2

- Just colcon build with using `--packages-up-to ocd_vehicle_nodes_cpp`
- Source your install folder 
- Just look up which components to exist. `ros2 component types ocd_vehicle_nodes_cpp`
- Run the component you want. This launches the vehicle in a ros2 node.
- Runs in realtime
- Tip: You can dump all parameters using the command `ros2 param dump <NodeName>`
- Settings parameters: Just load them from a parameter file (on startup) or use `ros2 param set`. All parameters are runtime settable.