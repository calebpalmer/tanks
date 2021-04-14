#include "startmenustate.h"
#include "assets.h"

#include "capengine/button.h"
#include "capengine/button_group.h"
#include "capengine/capcommon.h"
#include "capengine/controller.h"
#include "capengine/filesystem.h"
#include "capengine/game_management.h"
#include "capengine/locator.h"
#include "capengine/scene2dstate.h"
#include "capengine/textbutton.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Tanks
{

StartMenuState::StartMenuState(uint32_t in_windowId)
    : m_windowId(in_windowId), m_loaded(false), m_startNewGame(false),
      m_exitGame(false)
{
}

StartMenuState::~StartMenuState() {}

bool StartMenuState::onLoad()
{
    // Locator::assetManager->getSound(SOUND_MUSIC_NES_OVERWORLD);

    std::string currentDir =
        CapEngine::stripPath(CapEngine::getCurrentExecutablePath());

    std::stringstream fontDir;
    fontDir << currentDir << "/res/fonts/tahoma.ttf";

    CapEngine::Colour buttonInactiveColour(0x85, 0xFD, 0x00, 0xFF);
    CapEngine::Colour buttonActiveColour(0xBA, 0xBA, 0xBA, 0x00);

    uint32_t windowLogicalWidth, windowLogicalHeight = 0.0;
    std::tie(windowLogicalWidth, windowLogicalHeight) =
        CapEngine::Locator::videoManager->getWindowLogicalResolution(
            m_windowId);

    // buttons
    std::unique_ptr<CapEngine::ButtonGroup> pButtonGroup(
        new CapEngine::ButtonGroup);

    //  button
    std::unique_ptr<CapEngine::TextButton> pPlayButton(
        new CapEngine::TextButton(m_windowId, "Play", fontDir.str(), 30,
                                  CapEngine::Vector{}, buttonInactiveColour,
                                  buttonActiveColour));
    pPlayButton->setPosition(CapEngine::Vector(
        (windowLogicalWidth / 2) - (pPlayButton->getWidth() / 2), 200, 0.0));
    pPlayButton->setIndicator("res/textures/turtle_shell.png");
    // pPlayButton->registerCallback(&playButtonCallback, this);
    pPlayButton->registerCallback([this]() { this->m_startNewGame = true; });

    // quit button
    std::unique_ptr<CapEngine::Button> pQuitButton(new CapEngine::TextButton(
        m_windowId, "Quit", fontDir.str(), 30, CapEngine::Vector{},
        buttonInactiveColour, buttonActiveColour));

    pQuitButton->setPosition(CapEngine::Vector(
        (windowLogicalWidth / 2) - (pQuitButton->getWidth() / 2),
        pPlayButton->getPosition().getY() + pPlayButton->getHeight() + 20,
        0.0));

    pQuitButton->setIndicator("res/textures/turtle_shell.png");
    pQuitButton->registerCallback([this]() { this->m_exitGame = true; });

    // add the buttons to the button group
    pButtonGroup->addButton(std::move(pPlayButton));
    pButtonGroup->addButton(std::move(pQuitButton));

    // check for controller to listen to
    auto controllers = CapEngine::Controller::getConnectedControllers();
    if (controllers.size() > 0) {
        pButtonGroup->listenController(controllers[0]);
    }

    m_uiObjects.push_back(std::move(pButtonGroup));

    return true;
}

bool StartMenuState::onDestroy()
{
    // CapEngine::Locator::assetManager->stopSound(m_soundId);
    return true;
}

void StartMenuState::render()
{
    if (!m_loaded) {
        // m_soundId = CapEngine::Locator::assetManager->playSound(
        //     SOUND_MUSIC_NES_OVERWORLD, true);
        m_loaded = true;
    }

    // int textureWidth =
    //     CapEngine::Locator::assetManager->getImageWidth(TEXTURE_MENU_LOGO);
    // int textureHeight =
    //     CapEngine::Locator::assetManager->getImageHeight(TEXTURE_MENU_LOGO);
    // CapEngine::Rectangle srcRect(0, 0, textureWidth, textureHeight);
    // CapEngine::Rectangle dstRect(100, 35, 1000, 361);
    // CapEngine::Locator::assetManager->draw(m_windowId, TEXTURE_MENU_LOGO,
    //                                        srcRect, dstRect);

    // render ui
    for (auto &i : m_uiObjects) {
        i->render();
    }
}

void StartMenuState::update(double ms)
{
    // update ui objects
    for (auto &i : m_uiObjects) {
        i->update();
    }
    if (m_exitGame) {
        CapEngine::end();
    }
    if (m_startNewGame) {
        // load the scenes
        const std::string sceneFile = "res/scenes.json";
        std::ifstream stream(sceneFile);
        jsoncons::json scenesJson = jsoncons::json::parse(stream);

        const std::string sceneId = "demo";

        auto pScene2dState = std::make_unique<CapEngine::Scene2dState>(
            scenesJson, sceneId, m_windowId);

        pScene2dState->setEndSceneCB([]() { CapEngine::popState(); });

        CapEngine::pushState(std::move(pScene2dState));
    }
}

} // namespace Tanks
