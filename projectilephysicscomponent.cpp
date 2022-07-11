#include "projectilephysicscomponent.h"

#include "capengine/componentutils.h"
#include "capengine/gameobject.h"
#include "capengine/scene2dschema.h"
#include "capengine/scene2dutils.h"
#include "damageable.h"
#include "physics.h"
#include <memory>
#include <optional>

namespace Tanks {

//!  Constructor.
ProjectilePhysicsComponent::ProjectilePhysicsComponent(int in_width, int in_height, CapEngine::Vector in_velocity)
    : m_width(in_width), m_height(in_height), m_velocity(std::move(in_velocity)) {}

//! \copydoc Component::clone
std::unique_ptr<CapEngine::Component> ProjectilePhysicsComponent::clone() const {
    return std::make_unique<ProjectilePhysicsComponent>(m_width, m_height, m_velocity);
}

//! \copydoc Component::update
void ProjectilePhysicsComponent::update(CapEngine::GameObject &in_object, double ms) {
    //   // set default orientation
    const auto &orientation = in_object.getOrientation();
    if (orientation.getX() == 0.0 && orientation.getY() == 0.0)
        in_object.setOrientation(m_velocity.normalize());

    in_object.setVelocity(m_velocity);

    in_object.setPosition(CapEngine::applyDisplacement(in_object.getVelocity(), in_object.getPosition(), ms));
}

//! Creates the component from json.
/**
 \param in_json
   The json
 \return
   The component.
*/
std::unique_ptr<ProjectilePhysicsComponent> ProjectilePhysicsComponent::makeComponent(const jsoncons::json &in_json) {
    try {
        assert(in_json[CapEngine::Schema::Components::kComponentSubType] == kType);

        // get the width
        CapEngine::ComponentUtils::assertPropertyExists(in_json, ProjectilePhysicsComponent::kWidthPropertyName);
        const auto width = in_json[ProjectilePhysicsComponent::kWidthPropertyName].as_int();
        // get the height
        CapEngine::ComponentUtils::assertPropertyExists(in_json, ProjectilePhysicsComponent::kHeightPropertyName);
        const auto height = in_json[ProjectilePhysicsComponent::kHeightPropertyName].as_int();

        // get the velocity
        CapEngine::ComponentUtils::assertPropertyExists(in_json, ProjectilePhysicsComponent::kVelocityPropertyName);
        const auto velocity =
            CapEngine::JSONUtils::readVector(in_json[ProjectilePhysicsComponent::kVelocityPropertyName]);

        return std::make_unique<ProjectilePhysicsComponent>(width, height, velocity);
    }

    catch (const jsoncons::json_exception &e) {
        throw CapEngine::ComponentCreationException(CapEngine::ComponentType::Physics, kType, in_json,
                                                    boost::diagnostic_information(e));
    }
}

//! Registers the component with the ComponentFactory
/**
 \param in_factory
   The factory.
*/
void ProjectilePhysicsComponent::registerConstructor(CapEngine::ComponentFactory &in_factory) {
    in_factory.registerComponentType(
        CapEngine::ComponentUtils::componentTypeToString(CapEngine::ComponentType::Physics), kType, makeComponent);
}

std::optional<CapEngine::Rectangle>
ProjectilePhysicsComponent::boundingPolygon(const CapEngine::GameObject &in_object) const {
    const auto position = in_object.getPosition();
    const auto rectangle = CapEngine::Rectangle{static_cast<int>(position.getX() - (m_width / 2.0)),
                                                static_cast<int>(position.getY() - (m_height / 2)), m_width, m_height};

    return rectangle;
}

bool ProjectilePhysicsComponent::handleCollision(CapEngine::CollisionType in_collisionType,
                                                 CapEngine::CollisionClass in_collisionClass,
                                                 CapEngine::GameObject &in_object,
                                                 std::optional<CapEngine::GameObject *> in_otherObject,
                                                 const CapEngine::Vector &collisionLocation) {
    // if the other object is not the parent
    if (in_otherObject != std::nullopt && (*in_otherObject)->getObjectID() != in_object.getParentObjectID()) {
        for (auto &&component : (*in_otherObject)->getComponents()) {
            auto damageable = std::dynamic_pointer_cast<Damageable>(component);
            if (damageable) {
                damageable->damage(25);
                in_object.setObjectState(CapEngine::GameObject::Dead);
            }
        }
    }
    return true;
}

} // namespace Tanks
