#ifndef TANKS_GAME_H
#define TANKS_GAME_H

#include "capengine/gameevent.h"

#include <boost/signals2/connection.hpp>

namespace Tanks
{

enum struct GameState { startmenu, pause, scene, endlevel };

class Game
{
  public:
    int start();
    void handleGameEvent(const CapEngine::GameEvent &in_gameEvent);

  private:
    void startGame();
    void handlePausePressed();

    uint32_t m_windowId = 0;
    GameState m_state = GameState::startmenu;
    boost::signals2::scoped_connection m_gameEventsConnection;
};

} // namespace Tanks

#endif /* TANKS_GAME_H */
