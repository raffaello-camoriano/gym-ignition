/*
 * Copyright (C) 2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This project is dual licensed under LGPL v2.1+ or Apache License.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This software may be modified and distributed under the terms of the
 * GNU Lesser General Public License v2.1 or any later version.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SCENARIO_CONTROLLERS_CONTROLLER_H
#define SCENARIO_CONTROLLERS_CONTROLLER_H

#include "scenario/controllers/References.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace scenario {
    namespace controllers {
        class Controller;
        class UseScenarioModel;
        class SetBaseReferences;
        class SetJointReferences;
        using ControllerPtr = std::shared_ptr<Controller>;
        constexpr std::array<double, 3> g = {0, 0, -9.80665};
    } // namespace controllers
    namespace gazebo {
        class Model;
        using ModelPtr = std::shared_ptr<Model>;
    } // namespace gazebo
} // namespace scenario

class scenario::controllers::Controller
    : public std::enable_shared_from_this<scenario::controllers::Controller>
{
public:
    using StepSize = std::chrono::duration<double>;

    Controller() = default;
    virtual ~Controller() = default;

    virtual bool initialize() = 0;
    virtual bool step(const StepSize& dt) = 0;
    virtual bool terminate() = 0;
};

class scenario::controllers::UseScenarioModel
{
public:
    UseScenarioModel() = default;
    virtual ~UseScenarioModel() = default;

    virtual bool updateStateFromModel() = 0;

protected:
    gazebo::ModelPtr m_model;
};

class scenario::controllers::SetBaseReferences
{
public:
    SetBaseReferences() = default;
    virtual ~SetBaseReferences() = default;

    virtual bool setBaseReferences(const BaseReferences& jointReferences) = 0;
};

class scenario::controllers::SetJointReferences
{
public:
    SetJointReferences() = default;
    virtual ~SetJointReferences() = default;

    virtual const std::vector<std::string>& controlledJoints() = 0;
    virtual bool setJointReferences(const JointReferences& jointReferences) = 0;

protected:
    std::vector<std::string> m_controlledJoints;
};

#endif // SCENARIO_CONTROLLERS_CONTROLLER_H
