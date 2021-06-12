#include "pausestate.h"
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

    if (!m_escapePressed &&
        CapEngine::Locator::keyboard->key(CapEngine::Keyboard::CAP_ESCAPE)
                .state == CapEngine::Keyboard::CAP_PRESSED) {
        m_escapePressed = true;
    }

    if (m_escapePressed &&
        CapEngine::Locator::keyboard->key(CapEngine::Keyboard::CAP_ESCAPE)
                .state == CapEngine::Keyboard::CAP_UNPRESSED) {
        CapEngine::popState();
        return;
    }

    if (m_switchToMenuState) {
        m_switchToMenuState = false;
        unique_ptr<StartMenuState> p_startMenuState(
            new StartMenuState(m_windowID));
        CapEngine::switchState(std::move(p_startMenuState));
        return;
    }

    if (m_resumePlayState) {
        m_resumePlayState = false;
        CapEngine::popState();
        return;
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

    // m_uiObjects.push_back(std::move(pResumeButton));
    // m_uiObjects.push_back(std::move(pMainMenuButton));
    m_uiObjects.push_back(std::move(pButtonGroup));

    m_escapePressed = false;
    return true;
}

bool PauseState::onDestroy() { return true; }

void PauseState::returnToMenuCallback(void *context)
{
    reinterpret_cast<PauseState *>(context)->m_switchToMenuState = true;
}

void PauseState::resumeCallback(void *context)
{
    reinterpret_cast<PauseState *>(context)->m_resumePlayState = true;
}

} // namespace Tanks
