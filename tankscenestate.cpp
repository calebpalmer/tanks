#include "tankscenestate.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "gameevents.h"
#include "scene2dstate.h"

#include "capengine/locator.h"

#include "jsoncons/json.hpp"

namespace Tanks
{

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
    std::cout << "TankSceneState::onPause()" << std::endl;
    return GameState::onPause();
}

bool TankSceneState::onResume()
{
    std::cout << "TankSceneState::onResume()" << std::endl;
    return GameState::onResume();
}

} // namespace Tanks
