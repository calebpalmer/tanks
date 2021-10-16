#include "capengine/CapEngine.h"

#include "graphicscomponent.h"
#include "playerinputcomponent.h"
#include "projectilephysicscomponent.h"
#include "startmenustate.h"
#include "tankphysicscomponent.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <jsoncons/json.hpp>
#include <memory>
#include <sstream>
#include <string>

int main()
{
    try {
        std::cout << "Tanks!" << std::endl;

        const bool isFullScreen = false;

        CapEngine::WindowParams windowParams{
            "tanks", 1200, 800, 32, isFullScreen, false, false, false, "main"};

        uint32_t windowId = CapEngine::init(windowParams);
        CapEngine::Locator::videoManager->setWindowLogicalResolution(windowId,
                                                                     600, 400);

        const std::string assetFile = "res/assets.xml";
        CapEngine::loadAssetFile(assetFile);

        CapEngine::setDefaultQuitEvents();

        // register custom components
        CapEngine::ComponentFactory &componentFactory =
            CapEngine::ComponentFactory::getInstance();

        Tanks::GraphicsComponent::registerConstructor(componentFactory);
        Tanks::PlayerInputComponent::registerConstructor(componentFactory);
        Tanks::TankPhysicsComponent::registerConstructor(componentFactory);
        Tanks::ProjectilePhysicsComponent::registerConstructor(
            componentFactory);

        auto pStartMenuState =
            std::make_unique<Tanks::StartMenuState>(windowId);
        CapEngine::startLoop(std::move(pStartMenuState));

        return EXIT_SUCCESS;
    }

    catch (const boost::exception &e) {
        std::cout << boost::diagnostic_information(e) << std::endl;
        return EXIT_FAILURE;
    }

    catch (const std::exception &e) {
        std::cout << boost::diagnostic_information(e) << std::endl;
        return EXIT_FAILURE;
    }
}
