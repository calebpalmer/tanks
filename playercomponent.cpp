#include "playercomponent.h"

#include "capengine/CapEngineException.h"
#include "capengine/locator.h"
#include "hud.h"
#include "utils.h"

#include <any>
#include <optional>
#include <sstream>

namespace Tanks {

PlayerComponent::PlayerComponent(unsigned int in_health) : m_health(in_health) {}

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

    this->notifyHealth(object);
}

void PlayerComponent::notifyHealth(CapEngine::GameObject const &in_object) const {
    // get the player
    std::optional<int> playerNumber = getPlayerNumber(in_object);
    assert(playerNumber != std::nullopt);
    assert(*playerNumber == 1 || *playerNumber == 2);

    // get the hud
    std::any maybeHud = CapEngine::Locator::locate(Hud::locatorId);
    try {
        auto hud = std::any_cast<std::shared_ptr<Hud>>(maybeHud);
        Hud::Player player = *playerNumber == 1 ? Hud::Player::One : Hud::Player::Two;
        hud->updateHealth(player, m_health);
    } catch (const std::bad_any_cast &e) {
        std::cout << e.what() << '\n';
    }
}

} // namespace Tanks
