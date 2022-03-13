#include "tankphysicscomponent.h"
#include "projectilephysicscomponent.h"

#include "capengine/componentfactory.h"
#include "capengine/componentutils.h"
#include "capengine/locator.h"
#include "capengine/matrix.h"
#include "capengine/objectmanager.h"
#include "capengine/physics.h"
#include "capengine/placeholdergraphics.h"
#include "capengine/scene2dschema.h"
#include "capengine/trigonometry.h"
#include "scene2dutils.h"

#include <iostream>

namespace
{
const int kVelocity = 150;

const char kProjectileOffsetProperty[] = "projectileOffset";
} // namespace

namespace Tanks
{

namespace
{

std::unique_ptr<CapEngine::GameObject>
    makeProjectile(CapEngine::ObjectID in_parentObjectID,
                   CapEngine::Vector in_position, CapEngine::Vector in_velocity)
{
    auto gameObject = std::make_unique<CapEngine::GameObject>();

    gameObject->setParentObjectID(in_parentObjectID);
    gameObject->setPosition(in_position);
    gameObject->setVelocity(in_velocity);

    gameObject->addComponent(
        std::make_unique<ProjectilePhysicsComponent>(2, 2, in_velocity));

    gameObject->addComponent(std::make_unique<CapEngine::PlaceHolderGraphics>(
        2, 2, CapEngine::Colour{255, 0, 0, 255}));

    return gameObject;
}

} // namespace

//!  Constructor.
TankPhysicsComponent::TankPhysicsComponent(
    std::optional<CapEngine::Vector> in_projectileOffset)
{
    if (in_projectileOffset)
        m_projectileOffset = *in_projectileOffset;
}

//! Creates the component from json.
/**
 \param in_json
   The json
 \return
   The component.
*/
std::unique_ptr<TankPhysicsComponent>
    TankPhysicsComponent::makeComponent(const jsoncons::json &in_json)
{
    try {
        assert(in_json[CapEngine::Schema::Components::kComponentSubType] ==
               kType);

        auto projectileOffset = CapEngine::Vector{0.0, 0.0, 0.0};
        if (in_json.has_key(kProjectileOffsetProperty))
            projectileOffset = CapEngine::JSONUtils::readVector(
                in_json[kProjectileOffsetProperty]);

        return std::make_unique<TankPhysicsComponent>(projectileOffset);

    } catch (const jsoncons::json_exception &e) {
        throw CapEngine::ComponentCreationException(
            CapEngine::ComponentType::Physics, kType, in_json,
            boost::diagnostic_information(e));
    }
}

//! \copydoc Component::clone
std::unique_ptr<CapEngine::Component> TankPhysicsComponent::clone() const
{
    return std::make_unique<TankPhysicsComponent>(*this);
}

//! \copydoc Component::update
void TankPhysicsComponent::update(CapEngine::GameObject &object,
								  double timestep)
{
  // set default orientation
  const auto& orientation = object.getOrientation();
  if(orientation.getX() == 0.0 && orientation.getY() == 0.0)
	object.setOrientation(CapEngine::Vector(0.0, 1.0)); // default is pointing up

  // apply forces here.
  const auto& force = object.getForce();
  object.setVelocity({
					  force.getX() * kVelocity,
					  force.getY() * kVelocity
	});
  object.setPosition(CapEngine::applyDisplacement(object.getVelocity(), object.getPosition(), timestep));

  if(force.getX() != 0.0 || force.getY() != 0)
	object.setOrientation(CapEngine::Vector{force.getX() * 1, force.getY() * 1, force.getZ() * 1, 0.0});
}

//! Registers the component with the ComponentFactory
/**
 \param in_factory
   The factory.
*/
void TankPhysicsComponent::registerConstructor(
    CapEngine::ComponentFactory &in_factory)
{
    in_factory.registerComponentType(
        CapEngine::ComponentUtils::componentTypeToString(
            CapEngine::ComponentType::Physics),
        kType, makeComponent);
}

//! \copydoc Component::receive
void TankPhysicsComponent::receive(CapEngine::GameObject &in_object,
                                   int /*in_messageId*/,
                                   const std::string &in_message)
{
    if (in_message == "Fire") {
        auto maybeObjectManager = CapEngine::Locator::locate(
            CapEngine::ObjectManager::kObjectManagerLocatorId);
        CAP_THROW_ASSERT(maybeObjectManager.has_value(),
                         "ObjectManager not found.");

        auto *objectManager =
            std::any_cast<std::shared_ptr<CapEngine::ObjectManager>>(
                &maybeObjectManager);
        CAP_THROW_ASSERT(objectManager != nullptr, "ObjectManager not found.");

        const auto speed = double{200};

        const auto &objectPosition = in_object.getPosition();

        // subsract 90 because default orientation is up (90)
        const auto angle = RADTODEG(in_object.getOrientation().angle2d()) - 90;
        const auto translationMatrix =
            CapEngine::Matrix::createTranslationMatrix(objectPosition.getX(),
                                                       objectPosition.getY(),
                                                       objectPosition.getZ());
        const auto rotationMatrix =
            CapEngine::Matrix::createZRotationMatrix(angle);
        const auto position =
            translationMatrix * rotationMatrix * m_projectileOffset;

        (*objectManager)
            ->addObject(
                makeProjectile(in_object.getObjectID(), position,
                               in_object.getOrientation().normalize() * speed));
    }
}

} // namespace Tanks
