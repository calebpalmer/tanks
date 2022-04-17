#include "game.h"

#include "boost/bind/placeholders.hpp"
#include "capengine/CapEngine.h"
#include "capengine/game_management.h"
#include "capengine/gameobject.h"
#include "gameevents.h"
#include "graphicscomponent.h"
#include "pausestate.h"
#include "playercomponent.h"
#include "playerinputcomponent.h"
#include "projectilephysicscomponent.h"
#include "startmenustate.h"
#include "tankphysicscomponent.h"
#include "tankscenestate.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <jsoncons/json.hpp>
#include <memory>
#include <sstream>
#include <string>

namespace Tanks
{

int Game::start()
{
    try {
        std::cout << "Tanks!" << std::endl;

        // set up the window
        const bool isFullScreen = false;

        CapEngine::WindowParams windowParams{
            "tanks", 1200, 800, 32, isFullScreen, false, false, false, "main"};

        m_windowId = CapEngine::init(windowParams);
        CapEngine::Locator::videoManager->setWindowLogicalResolution(m_windowId,
                                                                     600, 400);

        // load assets
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
        Tanks::PlayerComponent::registerConstructor(componentFactory);

        // register for events
        m_gameEventsConnection =
            CapEngine::Locator::eventSubscriber->m_gameEventSignal.connect(
                [this](const CapEngine::GameEvent &in_gameEvent) {
                    this->handleGameEvent(in_gameEvent);
                });

        // add initial state and start loop
        auto pStartMenuState =
            std::make_unique<Tanks::StartMenuState>(m_windowId);
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

void Game::handleGameEvent(const CapEngine::GameEvent &in_gameEvent)
{
    // start menu selection
    if (auto menuSelectionEvent =
            dynamic_cast<const MenuSelectionEvent *>(&in_gameEvent);
        menuSelectionEvent != nullptr) {
        std::cout << "MenuSelectionEvent received" << std::endl;

        // start game
        if (menuSelectionEvent->m_selection ==
            MenuSelectionEvent::MenuSelection::StartGame) {
            startGame();
        }
        // quit game
        else {
            CapEngine::end();
        }

    }

    // pause
    else if (auto pauseEvent = dynamic_cast<const PauseEvent *>(&in_gameEvent);
             pauseEvent != nullptr) {
        std::cout << "Pause Event received" << std::endl;

        if (pauseEvent->m_pause) {
            m_state = GameState::pause;
            pushState(std::make_shared<PauseState>(m_windowId));
        }

        else {
            m_state = GameState::scene;
            CapEngine::popState();
        }
    }

    // game state event changed
    else if (auto gameStateChangedEvent =
                 dynamic_cast<const CapEngine::GameObjectStateChangedEvent *>(
                     &in_gameEvent);
             gameStateChangedEvent != nullptr) {
        // get the metadata and see if it's a player object
        CapEngine::GameObject::Metadata const &metadata =
            gameStateChangedEvent->m_object->metadata();

        if (metadata.find("player") == metadata.end()) {
            return; // not a player, who cares.
        }

        CapEngine::MetadataType player = metadata.at("player");
        const int playerNumber = std::get<int>(player);
        m_state = GameState::startmenu;
        CapEngine::popState();
    }

    else {
        std::cout << "Some other event received" << std::endl;
    }
}

void Game::startGame()
{
    assert(this->m_state == GameState::startmenu);

    // load the scenes
    const std::string sceneFile = "res/scenes.json";
    std::ifstream stream(sceneFile);
    jsoncons::json scenesJson = jsoncons::json::parse(stream);

    const std::string sceneId = "demo";

    auto tankSceneState =
        std::make_unique<TankSceneState>(scenesJson, sceneId, m_windowId);

    // When the state is done, pop it and return to this state
    tankSceneState->setEndSceneCB([]() { CapEngine::popState(); });

    // start the new game scene state
    CapEngine::pushState(std::move(tankSceneState));
}

} // namespace Tanks

int main()
{
    Tanks::Game game;
    return game.start();
}
