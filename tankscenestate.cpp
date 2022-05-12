#include "tankscenestate.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "gameevents.h"
#include "scene2dstate.h"

#include "capengine/locator.h"

#include "jsoncons/json.hpp"
#include <optional>

namespace Tanks
{

namespace
{

const int kSoundId = 1000;

} // namespace

void TankSceneState::startMusic()
{
    // play music
    assert(CapEngine::Locator::assetManager != nullptr);
    m_soundId = CapEngine::Locator::assetManager->playSound(kSoundId, true);
}

void TankSceneState::stopMusic()
{
    // stop music
    if (m_soundId != std::nullopt) {
        assert(CapEngine::Locator::assetManager != nullptr);
        CapEngine::Locator::assetManager->stopSound(*m_soundId);
    }
}

TankSceneState::TankSceneState(jsoncons::json in_sceneDescriptors,
                               std::string in_sceneId, uint32_t in_windowId)
    : CapEngine::Scene2dState(in_sceneDescriptors, in_sceneId, in_windowId)
{
    // subscribe to keyboard events to handle pauses
    assert(CapEngine::Locator::eventSubscriber != nullptr);
    m_keyboardEventConnection =
        CapEngine::Locator::eventSubscriber->m_keyboardEventSignal.connect(
            [this](SDL_KeyboardEvent in_event) {
                if (this->enabled()) {
                    if (in_event.keysym.sym == SDLK_ESCAPE &&
                        in_event.type == SDL_KEYUP) {

                        std::cout << "Pause game" << std::endl;

                        assert(CapEngine::Locator::eventSubscriber != nullptr);
                        CapEngine::Locator::eventSubscriber->m_gameEventSignal(
                            PauseEvent(true));
                    }
                }
            });
}

bool TankSceneState::onPause()
{
    GameState::onPause();
    this->stopMusic();
    return true;
}

bool TankSceneState::onResume()
{
    GameState::onResume();
    this->startMusic();
    return true;
}

bool TankSceneState::onLoad()
{
    GameState::onLoad();
    this->startMusic();
    return true;
}

bool TankSceneState::onDestroy()
{
    GameState::onDestroy();
    this->stopMusic();
    return true;
}

} // namespace Tanks
