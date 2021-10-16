#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include <vector>

#include "capengine/gamestate.h"
#include "capengine/uiobject.h"

namespace Tanks
{

class PauseState : public CapEngine::GameState
{
  public:
    PauseState(uint32_t windowID);
    ~PauseState();
    virtual void render();
    virtual void update(double ms);
    virtual bool onLoad();
    virtual bool onDestroy();
    virtual bool onPause() override;
    virtual bool onResume() override;

    static void returnToMenuCallback(void *context);
    static void resumeCallback(void *context);

  private:
    uint32_t m_windowID;
    bool m_escapePressed;
    bool m_switchToMenuState;
    bool m_resumePlayState;
    std::vector<std::unique_ptr<CapEngine::UIObject>> m_uiObjects;
};

} // namespace Tanks

#endif // PAUSE_STATE_H
