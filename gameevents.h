#ifndef TANKS_GAMEEVENTS_H
#define TANKS_GAMEEVENTS_H

#include "capengine/gameevent.h"

namespace Tanks
{
class MenuSelectionEvent : public CapEngine::GameEvent
{
  public:
    enum struct MenuSelection { StartGame, Quit };
    MenuSelectionEvent(MenuSelection selection);

    ~MenuSelectionEvent() override = default;
    std::string type() const override { return "TanksMenuSelection"; }

    MenuSelection m_selection;
};

class PauseEvent : public CapEngine::GameEvent
{
  public:
    PauseEvent(bool in_pause);

    ~PauseEvent() override = default;
    std::string type() const override { return "Pause"; }

    bool m_pause = false;
};

} // namespace Tanks

#endif /* TANKS_GAMESTATES_H */
