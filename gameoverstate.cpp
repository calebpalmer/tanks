#include "gameoverstate.h"
#include "collision.h"
#include "startmenustate.h"

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "capengine/filesystem.h"
#include "capengine/game_management.h"
#include "capengine/locator.h"

namespace Tanks
{

GameOverState::GameOverState(uint32_t in_windowId, int in_winningPlayer)
    : m_windowId(in_windowId), m_winningPlayer(in_winningPlayer)
{
}

bool GameOverState::onLoad()
{
    std::stringstream labelText;
    labelText << "Player " << m_winningPlayer << " Wins!";

    auto [windowLogicalWidth, windowLogicalHeight] =
        CapEngine::Locator::videoManager->getWindowLogicalResolution(
            m_windowId);
    m_maxTextWidth = windowLogicalWidth * 0.75;
    m_maxTextHeight = windowLogicalHeight * 0.25;

    std::string currentDir =
        CapEngine::stripPath(CapEngine::getCurrentExecutablePath());
    std::stringstream fontDir;
    fontDir << currentDir << "/res/fonts/tahoma.ttf";

    m_label =
        CapEngine::UI::Label::create(labelText.str(), fontDir.str(), 36,
                                     CapEngine::Colour(0x85, 0xFD, 0x00, 0xFF));
    m_label->setWindowId(m_windowId);

    m_keyboardSignalConnection =
        CapEngine::Locator::eventSubscriber->m_keyboardEventSignal.connect(
            [this](SDL_KeyboardEvent in_event) {
                if (m_enabled && m_finished && in_event.type == SDL_KEYUP &&
                    in_event.keysym.sym == SDLK_RETURN) {

                    CapEngine::switchState(
                        std::make_unique<Tanks::StartMenuState>(m_windowId));
                }
            });

    return true;
}

void GameOverState::render()
{
    if (!m_enabled)
        return;

    m_label->render();
}

void GameOverState::update(double in_ms)
{
    if (!m_enabled)
        return;

    if (!m_finished) {
        m_elapsedTime += in_ms;
        if (m_elapsedTime >= m_animationLengthMs) {
            m_finished = true;
        }

        // get the aspect ratio of the label texture
        assert(m_label != nullptr);
        auto [textureWidth, textureHeight] = m_label->getTextureSize();
        const double textureRatio = static_cast<double>(textureHeight) /
                                    static_cast<double>(textureWidth);

        // set the current size
        m_currentTextWidth =
            m_maxTextWidth * (m_elapsedTime / m_animationLengthMs);
        m_currentTextHeight = m_currentTextWidth * textureRatio;
        m_label->setSize(m_currentTextWidth, m_currentTextHeight);

        // set the position of the label
        auto [windowLogicalWidth, windowLogicalHeight] =
            CapEngine::Locator::videoManager->getWindowLogicalResolution(
                m_windowId);
        m_label->setPosition(
            (windowLogicalWidth / 2.0) - (m_currentTextWidth / 2.0),
            (windowLogicalHeight / 2.0) - (m_currentTextHeight / 2.0));

        std::cout << CapEngine::Rectangle(m_label->getPosition()) << std::endl;
    }

    m_label->update(in_ms);
}

} // namespace Tanks
