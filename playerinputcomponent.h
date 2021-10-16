#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

#include "SDL_events.h"
#include "capengine/componentfactory.h"
#include "capengine/components.h"

#include <boost/signals2.hpp>
#include <jsoncons/json.hpp>
#include <vector>

namespace Tanks {

class PlayerInputComponent : public CapEngine::InputComponent
{
  public:
    PlayerInputComponent();
    ~PlayerInputComponent() override;

    PlayerInputComponent(const PlayerInputComponent &in_other);

    void update(CapEngine::GameObject &in_object, double in_timestep) override;
    std::unique_ptr<Component> clone() const override;

    static PlayerInputComponent makeComponent(const jsoncons::json &in_json);
    static std::unique_ptr<PlayerInputComponent>
        makeSharedComponent(const jsoncons::json &in_json);
    static void registerConstructor(CapEngine::ComponentFactory &in_factory);

    static constexpr inline char kComponentType[] = "PlayerInputComponent";

  private:
    void connectKeyboardEvent();
    void handleKeyboardInput(SDL_KeyboardEvent in_event);

    boost::signals2::connection
        m_keyboardInputConnection; //!< The connection to the keybaord input
                                   //!< signal.
    bool m_doFire = false;         //!< Fire command to be run.
    int m_player = 1;              //!< The player number
};

} // namespace Tanks

#endif // PLAYERINPUTCOMPONENT_H
