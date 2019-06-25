/*
 * Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * GNU Lesser General Public License v2.1 or any later version.
 */

#include "RobotController.h"
#include "gympp/Log.h"
#include "gympp/Robot.h"
#include "gympp/gazebo/EnvironmentCallbacksSingleton.h"
#include "gympp/gazebo/IgnitionRobot.h"

#include <ignition/plugin/Register.hh>

#include <cassert>
#include <chrono>
#include <ostream>
#include <string>

using namespace gympp::gazebo;
using namespace gympp::plugins;

using ObservationDataType = int;
using ObservationSample = gympp::BufferContainer<ObservationDataType>::type;

class RobotController::Impl
{
public:
    std::shared_ptr<gympp::Robot> robot = nullptr;
};

RobotController::RobotController()
    : System()
    , pImpl{new Impl(), [](Impl* impl) { delete impl; }}
{}

RobotController::~RobotController()
{
    auto* ecSingleton = EnvironmentCallbacksSingleton::Instance();

    if (bool removed = ecSingleton->deleteEnvironmentCallback(pImpl->robot->name()); !removed) {
        gymppError << "Failed to unregister the environment callbacks";
        assert(removed);
    }
}

void RobotController::Configure(const ignition::gazebo::Entity& entity,
                                const std::shared_ptr<const sdf::Element>& sdf,
                                ignition::gazebo::EntityComponentManager& ecm,
                                ignition::gazebo::EventManager& /*eventMgr*/)
{
    // Create an IgnitionRobot object from the ecm
    auto ignRobot = std::make_shared<gympp::gazebo::IgnitionRobot>();
    if (!ignRobot->configureECM(entity, sdf, ecm)) {
        gymppError << "Failed to configure the Robot interface" << std::endl;
        return;
    }

    if (!ignRobot->valid()) {
        gymppError << "The Robot interface is not valid" << std::endl;
        return;
    }

    // Store a pointer to gympp::Robot
    pImpl->robot = ignRobot;

    // Auto-register the environment callbacks
    gymppDebug << "Registering environment callbacks for robot '" << ignRobot->name() << "'"
               << std::endl;
    auto ecSingleton = EnvironmentCallbacksSingleton::Instance();
    bool registered = ecSingleton->storeEnvironmentCallback(ignRobot->name(), this);
    assert(registered);
}

void RobotController::PreUpdate(const ignition::gazebo::UpdateInfo& info,
                                ignition::gazebo::EntityComponentManager& /*ecm*/)
{
    if (info.paused) {
        return;
    }

    if (!pImpl->robot->update(info.simTime)) {
        assert(false);
        gymppError << "Failed to update the robot controller" << std::endl;
        return;
    }
}

bool RobotController::isDone()
{
    return false;
}

bool RobotController::reset()
{
    return true;
}

bool RobotController::setAction(const EnvironmentCallbacks::Action& /*action*/)
{
    return true;
}

std::optional<gympp::gazebo::EnvironmentCallbacks::Reward> RobotController::computeReward()
{
    return EnvironmentCallbacks::Reward(0);
}

std::optional<gympp::gazebo::EnvironmentCallbacks::Observation> RobotController::getObservation()
{
    return ObservationSample{0};
}

IGNITION_ADD_PLUGIN(gympp::plugins::RobotController,
                    gympp::plugins::RobotController::System,
                    gympp::plugins::RobotController::ISystemConfigure,
                    gympp::plugins::RobotController::ISystemPreUpdate,
                    gympp::gazebo::EnvironmentCallbacks)
