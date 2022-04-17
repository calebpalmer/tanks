#include "gameevents.h"

namespace Tanks
{

// MenuSelectionEvent
MenuSelectionEvent::MenuSelectionEvent(MenuSelection in_menuSelection)
    : m_selection(in_menuSelection)
{
}

// PauseEvent
PauseEvent::PauseEvent(bool in_pause) : m_pause(in_pause) {}
} // namespace Tanks
