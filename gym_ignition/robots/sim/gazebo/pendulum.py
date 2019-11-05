# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT). All rights reserved.
# This software may be modified and distributed under the terms of the
# GNU Lesser General Public License v2.1 or any later version.

import numpy as np
from gym_ignition.robots import gazebo_robot


class PendulumGazeboRobot(gazebo_robot.GazeboRobot):
    def __init__(self, model_file: str, gazebo, **kwargs):
        # Initialize base class
        super().__init__(model_file=model_file,
                         gazebo=gazebo,
                         controller_rate=kwargs.get("controller_rate"))

        # Configure the pendulum as fixed-base robot
        floating = False if "floating" not in kwargs else kwargs["floating"]
        ok_floating = self.set_as_floating_base(floating)
        assert ok_floating, "Failed to set the robot as floating base"

        # Initial base position
        base_position = np.array([0., 0., 0.]) \
            if "base_position" not in kwargs else kwargs["base_position"]

        # Initial base orientation
        base_orientation = np.array([1., 0., 0., 0.]) \
            if "base_orientation" not in kwargs else kwargs["base_orientation"]

        # Set the base pose
        ok_base_pose = self.set_initial_base_pose(base_position, base_orientation)
        assert ok_base_pose, "Failed to set base pose"

        # Insert the model in the simulation
        _ = self.gympp_robot
