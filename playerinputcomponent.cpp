#include "playerinputcomponent.h"

#include "CapEngineException.h"
#include "SDL_keycode.h"
#include "capengine/eventsubscriber.h"
#include "capengine/gameobject.h"
#include "capengine/keyboard.h"
#include "capengine/locator.h"
#include "capengine/placeholdergraphics.h"
#include "capengine/sdlutils.h"
#include "capengine/vector.h"
#include "scene2d.h"

#include <memory>
#include <mutex>

namespace Tanks
{

PlayerInputComponent::PlayerInputComponent()
{
    // subscribe to keyboard events
    CAP_THROW_ASSERT(CapEngine::Locator::eventSubscriber != nullptr,
                     "EventSubscribe not initialized.");
    this->connectKeyboardEvent();
}

PlayerInputComponent::PlayerInputComponent(const PlayerInputComponent &in_other)
{
    m_doFire = in_other.m_doFire;
    this->connectKeyboardEvent();
}

PlayerInputComponent::~PlayerInputComponent()
{
    m_keyboardInputConnection.disconnect();
}

void PlayerInputComponent::connectKeyboardEvent()
{
    m_keyboardInputConnection =
        CapEngine::Locator::eventSubscriber->m_keyboardEventSignal.connect(
            std::bind(&PlayerInputComponent::handleKeyboardInput, this,
                      std::placeholders::_1));
}

std::unique_ptr<CapEngine::Component> PlayerInputComponent::clone() const
{
    return std::make_unique<PlayerInputComponent>(*this);
}

/** @copydoc CapEngine::Component::update */
void PlayerInputComponent::update(CapEngine::GameObject &in_object,
                                  double in_timestep)
{

    using namespace CapEngine;

    Vector force;

    if (Locator::keyboard->key(Keyboard::CAP_KEYUP).state ==
        Keyboard::CAP_PRESSED)
        force.setY(force.getY() + 1);

    if (Locator::keyboard->key(Keyboard::CAP_KEYDOWN).state ==
        Keyboard::CAP_PRESSED)
        force.setY(force.getY() - 1);

    if (Locator::keyboard->key(Keyboard::CAP_KEYRIGHT).state ==
        Keyboard::CAP_PRESSED)
        force.setX(force.getX() + 1);

    if (Locator::keyboard->key(Keyboard::CAP_KEYLEFT).state ==
        Keyboard::CAP_PRESSED)
        force.setX(force.getX() - 1);

    in_object.setForce(force);

    if (m_doFire) {
        in_object.send(0, "Fire");
        m_doFire = false;
    }
}

PlayerInputComponent
    PlayerInputComponent::makeComponent(const jsoncons::json &in_json)
{
    return PlayerInputComponent();
}

std::unique_ptr<PlayerInputComponent>
    PlayerInputComponent::makeSharedComponent(const jsoncons::json &in_json)
{
    return std::make_unique<PlayerInputComponent>();
}

void PlayerInputComponent::registerConstructor(
    CapEngine::ComponentFactory &in_factory)
{
    in_factory.registerComponentType(
        CapEngine::ComponentUtils::componentTypeToString(
            CapEngine::ComponentType::Input),
        PlayerInputComponent::kComponentType,
        PlayerInputComponent::makeSharedComponent);
}

void PlayerInputComponent::handleKeyboardInput(SDL_KeyboardEvent in_event)
{
    if (in_event.state == SDL_PRESSED && in_event.keysym.sym == SDLK_SPACE) {
        std::cout << "setting m_doFire to true." << std::endl;
        m_doFire = true;
    }
}

} // namespace Tanks
