#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include <boost/signals2/connection.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
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
    bool m_enabled = false;
    uint32_t m_windowID;
    bool m_escapePressed = false;
    bool m_switchToMenuState = false;
    bool m_resumePlayState = false;
    std::vector<std::unique_ptr<CapEngine::UIObject>> m_uiObjects;
    boost::signals2::scoped_connection m_keyboardConnection;
};

} // namespace Tanks

#endif // PAUSE_STATE_H
