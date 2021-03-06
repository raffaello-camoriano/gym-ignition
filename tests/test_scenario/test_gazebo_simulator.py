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
                         [
                             (0.001, 1.0, 1),
                             (0.1, 5.0, 5),
                             (0.001, 0.0, 1),
                             (0.001, -1.0, 1),
                             (0.001, 1.0, 0),
                             (0, 1.0, 1),
                         ], indirect=True, ids=utils.id_gazebo_fn)
def test_initialization(gazebo: bindings.GazeboSimulator):

    ok = gazebo.initialize()

    rtf = gazebo.real_time_factor()
    step_size = gazebo.step_size()
    iterations = gazebo.steps_per_run()

    if step_size <= 0:
        assert not ok
        assert not gazebo.initialized()
        assert not gazebo.run()

    if rtf <= 0:
        assert not ok
        assert not gazebo.initialized()
        assert not gazebo.run()

    if iterations <= 0:
        assert not ok
        assert not gazebo.initialized()
        assert not gazebo.run()

    if rtf > 0 and iterations > 0 and step_size > 0:
        assert ok
        assert gazebo.initialized()


@pytest.mark.parametrize("gazebo",
                         [(0.001, 1.0, 1)],
                         indirect=True,
                         ids=utils.id_gazebo_fn)
def test_run(gazebo: bindings.GazeboSimulator):

    assert gazebo.initialize()
    assert gazebo.run(paused=True)
    assert gazebo.run()


@pytest.mark.parametrize("gazebo",
                         [(0.001, 1.0, 1)],
                         indirect=True,
                         ids=utils.id_gazebo_fn)
def test_pause(gazebo: bindings.GazeboSimulator):

    assert gazebo.initialize()
    assert not gazebo.running()
    assert gazebo.pause()

    assert gazebo.initialize()

    assert not gazebo.running()
    assert gazebo.pause()

    assert gazebo.run(paused=True)
    assert not gazebo.running()
    assert gazebo.pause()

    assert gazebo.run(paused=False)
    assert not gazebo.running()
    assert gazebo.pause()


@pytest.mark.parametrize("gazebo",
                         [(0.001, 1.0, 1)],
                         indirect=True,
                         ids=utils.id_gazebo_fn)
def test_load_default_world(gazebo: bindings.GazeboSimulator):

    assert gazebo.initialize()
    assert gazebo.world_names()
    assert len(gazebo.world_names()) == 1

    world1 = gazebo.get_world()
    assert world1
    assert world1.name() in gazebo.world_names()

    world2 = gazebo.get_world(gazebo.world_names()[0])
    assert world2

    assert world1.id() == world2.id()
    assert world1.name() == world2.name()

    # TODO: understand how to compare shared ptr returned by swig with nullptr
    # world3 = gazebo.get_world("foo")
    # assert world3
