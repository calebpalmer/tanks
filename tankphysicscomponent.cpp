#include "tankphysicscomponent.h"

#include "capengine/componentfactory.h"
#include "capengine/scene2dschema.h"
#include "capengine/physics.h"

#include <iostream>

namespace {
  const int kVelocity = 150;
}

namespace Tanks
{

//!  Constructor.
TankPhysicsComponent::TankPhysicsComponent() {}

//! \copydoc Component::clone
std::unique_ptr<CapEngine::Component> TankPhysicsComponent::clone() const
{
  return std::make_unique<TankPhysicsComponent>();
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
	assert(in_json[CapEngine::Schema::Components::kComponentSubType] == kType);
	return std::make_unique<TankPhysicsComponent>();
  } catch (const jsoncons::json_exception &e) {
	throw CapEngine::ComponentCreationException(
		CapEngine::ComponentType::Physics, kType, in_json,
		boost::diagnostic_information(e));
  }
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

} // namespace Tanks
