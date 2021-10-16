#ifndef START_MENU_STATE_H
#define START_MENU_STATE_H

#include "capengine/gamestate.h"
#include "capengine/uiobject.h"

#include <memory>
#include <string>
#include <vector>

namespace Tanks
{

class StartMenuState : public CapEngine::GameState
{
  public:
    StartMenuState(uint32_t in_windowId);
    ~StartMenuState();
    virtual void render() override;          // GameState
    virtual void update(double ms) override; // GameState
    virtual bool onLoad() override;          // GameState
    virtual bool onDestroy() override;       // GameState
    virtual bool onPause() override;         // GameState
    virtual bool onResume() override;        // GaneState

  protected:
    StartMenuState(const StartMenuState &);
    StartMenuState &operator=(const StartMenuState &);

    uint32_t m_windowId = 0;
    std::vector<std::unique_ptr<CapEngine::UIObject>> m_uiObjects;
    bool m_loaded = false;
    bool m_startNewGame = false;
    bool m_exitGame = false;
    long m_soundId = 0;
};

} // namespace Tanks

#endif // START_MENU_STATE
