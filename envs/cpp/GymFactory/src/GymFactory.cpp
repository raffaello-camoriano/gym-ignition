#include "gympp/GymFactory.h"
#include "gympp/Log.h"
#include "gympp/gazebo/IgnitionEnvironment.h"

gympp::EnvironmentPtr gympp::GymFactory::make(const std::__cxx11::string& envName)
{
    if (envName == "CartPole") {
        using OSpace = gympp::spaces::Box;
        using ASpace = gympp::spaces::Discrete;

        // Create the gym environment
        auto ignGym = std::make_shared<gympp::gazebo::IgnitionEnvironment>(
            std::make_shared<ASpace>(3),
            std::make_shared<OSpace>(OSpace::Limit{-90, -1}, OSpace::Limit{90, 1}),
            /*updateRate=*/1000,
            /*iterations=*/1);

        // Setup the CartPolePlugin
        if (!ignGym->setupIgnitionPlugin("CartPolePlugin", "gympp::plugins::CartPole")) {
            gymppError << "Failed to setup the ignition plugin" << std::endl;
            return nullptr;
        }

        // Setup the SDF file
        if (!ignGym->setupSdf("CartPole.world", {"cartpole_xacro"})) {
            gymppError << "Failed to setup SDF file";
            return nullptr;
        }

        return ignGym->env();
    }

    return nullptr;
}
