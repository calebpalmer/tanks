#include "pausestate.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "gameevent.h"
#include "gameevents.h"
#include "startmenustate.h"

#include "capengine/button_group.h"
#include "capengine/game_management.h"
#include "capengine/keyboard.h"
#include "capengine/locator.h"
#include "capengine/textbutton.h"
#include "capengine/vector.h"

#include <memory>

using namespace std;

namespace Tanks
{

PauseState::PauseState(uint32_t windowID)
    : m_windowID(windowID), m_escapePressed(false), m_switchToMenuState(false),
      m_resumePlayState(false)
{
}

PauseState::~PauseState() {}

void PauseState::render()
{
    if (m_uiObjects.size() > 0) {
        for (auto &i : m_uiObjects) {
            i->render();
        }
    }
}

void PauseState::update(double /*ms*/)
{
    for (auto &&i : m_uiObjects) {
        i->update();
    }
}

bool PauseState::onLoad()
{
    // add ui
    unique_ptr<CapEngine::TextButton> pResumeButton(
        new CapEngine::TextButton(m_windowID, "Resume", "res/fonts/tahoma.ttf",
                                  40, CapEngine::Vector(0, 0)));
    unique_ptr<CapEngine::TextButton> pMainMenuButton(new CapEngine::TextButton(
        m_windowID, "Main Menu", "res/fonts/tahoma.ttf", 40,
        CapEngine::Vector(0, 0)));
    int numItems = 2;
    int xres, yres;
    CapEngine::Locator::videoManager->getWindowResolution(m_windowID, &xres,
                                                          &yres);
    int maxwidth = pMainMenuButton->getWidth();
    int xStart = (xres / 2.0) - (maxwidth / 2.0);
    int menuItemSpacing = 10;
    int maxheight = pResumeButton->getHeight() + pMainMenuButton->getHeight() +
                    ((numItems - 1) * menuItemSpacing);
    int yStart = (yres / 2.0) - (maxheight / 2.0);

    int itemHeight = pResumeButton->getHeight();
    pResumeButton->setPosition(CapEngine::Vector(xStart, yStart));
    pMainMenuButton->setPosition(CapEngine::Vector(
        xStart, yStart + ((numItems - 1) * (itemHeight + menuItemSpacing))));

    pResumeButton->registerCallback(this->resumeCallback, this);
    pMainMenuButton->registerCallback(this->returnToMenuCallback, this);

    unique_ptr<CapEngine::ButtonGroup> pButtonGroup(
        new CapEngine::ButtonGroup());
    pButtonGroup->addButton(std::move(pResumeButton));
    pButtonGroup->addButton(std::move(pMainMenuButton));

    m_uiObjects.push_back(std::move(pButtonGroup));

    m_keyboardConnection =
        CapEngine::Locator::eventSubscriber->m_keyboardEventSignal.connect(
            [this](SDL_KeyboardEvent in_keyboardEvent) {
                if (m_enabled) {
                    if (in_keyboardEvent.keysym.sym == SDLK_ESCAPE &&
                        in_keyboardEvent.type == SDL_KEYUP) {

                        std::cout << "Unpause" << std::endl;
                        assert(CapEngine::Locator::eventSubscriber != nullptr);
                        CapEngine::Locator::eventSubscriber->m_gameEventSignal(
                            PauseEvent(false));
                    }
                }
            });

    m_enabled = true;
    return true;
}

bool PauseState::onDestroy() { return true; }

// \copydoc GameState::onPause
bool PauseState::onPause()
{
    m_enabled = false;
    for (auto &&i : m_uiObjects)
        i->setEnabled(false);

    return true;
}

// \copydoc GameState::onResume
bool PauseState::onResume()
{
    m_enabled = true;
    for (auto &&i : m_uiObjects)
        i->setEnabled(true);

    return true;
}

void PauseState::returnToMenuCallback(void *context)
{
    reinterpret_cast<PauseState *>(context)->m_switchToMenuState = true;
}

void PauseState::resumeCallback(void *context)
{
    reinterpret_cast<PauseState *>(context)->m_resumePlayState = true;
}

} // namespace Tanks
