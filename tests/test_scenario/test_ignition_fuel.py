# Copyright (C) 2020 Istituto Italiano di Tecnologia (IIT). All rights reserved.
# This software may be modified and distributed under the terms of the
# GNU Lesser General Public License v2.1 or any later version.

import pytest
pytestmark = pytest.mark.scenario

from ..common import utils
from ..common.utils import gazebo_fixture as gazebo
from gym_ignition import scenario_bindings as bindings

# Set the verbosity
bindings.set_verbosity(4)


@pytest.mark.parametrize("gazebo",
                         [(0.001, 1.0, 1)],
                         indirect=True,
                         ids=utils.id_gazebo_fn)
def test_download_model_from_fuel(gazebo: bindings.GazeboSimulator):

    assert gazebo.initialize()

    # Get the default world
    world = gazebo.get_world()

    # Download a model from Fuel (testing a name with spaces)
    model_name = "Electrical Box"
    model_sdf = bindings.get_model_file_from_fuel(
        f"https://fuel.ignitionrobotics.org/openrobotics/models/{model_name}", False)
    assert model_sdf

    assert world.insert_model(model_sdf, bindings.Pose_identity())
    assert model_name in world.model_names()

    # Insert another model changing its name
    other_model_name = "my_box"
    other_model_pose = bindings.Pose([3.0, 0.0, 0.0], [1.0, 0, 0, 0])
    assert world.insert_model(model_sdf, other_model_pose, other_model_name)
    assert other_model_name in world.model_names()

    assert gazebo.run()
