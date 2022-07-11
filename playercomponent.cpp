#include "playercomponent.h"

#include "capengine/CapEngineException.h"

#include <sstream>

namespace Tanks
{

PlayerComponent::PlayerComponent(unsigned int in_health) : m_health(in_health)
{
}

PlayerComponent PlayerComponent::makeComponent(const jsoncons::json &in_json) {
    if (!in_json.has_key(PlayerComponent::kHealthProperty)) {
        std::stringstream error;
        error << "Missing required property \"" << PlayerComponent::kHealthProperty;
        CAP_THROW(CapEngine::CapEngineException(error.str()));
    }

    return PlayerComponent(in_json[PlayerComponent::kHealthProperty].as_uint());
}

std::unique_ptr<PlayerComponent> PlayerComponent::makeSharedComponent(const jsoncons::json &in_json) {
    return std::make_unique<PlayerComponent>(makeComponent(in_json));
}

void PlayerComponent::registerConstructor(CapEngine::ComponentFactory &in_factory) {
    in_factory.registerComponentType(CapEngine::ComponentUtils::componentTypeToString(CapEngine::ComponentType::Custom),
                                     PlayerComponent::kComponentType, PlayerComponent::makeSharedComponent);
}

void PlayerComponent::damage(int damage) {
    m_health -= damage;
    std::cout << "Achh I been damaged.  My health is now " << m_health << "\n1";
}

void PlayerComponent::update(CapEngine::GameObject &object, double timestep) {
    const auto state = object.getObjectState();
    using namespace CapEngine;
    if (state == GameObject::ObjectState::Active) {
        if (m_health <= 0) {
            object.setObjectState(GameObject::ObjectState::Dying);
        }
    }
    if (state == GameObject::ObjectState::Dying) {
        std::cout << "egad...I dead\n";
        object.setObjectState(GameObject::ObjectState::Dead);
    }
}

} // namespace Tanks
