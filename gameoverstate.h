#ifndef TANKS_GAMEOVERSTATE_H
#define TANKS_GAMEOVERSTATE_H

#include "capengine/gamestate.h"
#include "capengine/label.h"

#include <boost/signals2/connection.hpp>
#include <chrono>
#include <cstdint>

namespace Tanks
{

class GameOverState final : public CapEngine::GameState
{
  public:
    GameOverState(uint32_t in_windowId, int in_winningPlayer);
    ~GameOverState() override = default;

    void render() override;          // GameState
    void update(double ms) override; // GameState
    bool onLoad() override;          // GameState

  private:
    uint32_t m_windowId = 0;
    int m_winningPlayer = -1;
    bool m_finished = false;
    std::shared_ptr<CapEngine::UI::Label> m_label;
    unsigned int m_maxTextWidth = 0;
    unsigned int m_maxTextHeight = 0;
    unsigned int m_currentTextWidth = 0;
    unsigned int m_currentTextHeight = 0;
    double m_animationLengthMs = 3000;
    double m_elapsedTime = 0.0;
    boost::signals2::scoped_connection m_keyboardSignalConnection;
};

} // namespace Tanks

#endif /* TANKS_GAMEOVERSTATE_H */
