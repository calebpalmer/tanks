#ifndef TANKS_TANKPHYSICSCOMPONENT_H
#define TANKS_TANKPHYSICSCOMPONENT_H

#include "capengine/components.h"
#include "capengine/componentfactory.h"

#include <jsoncons/json.hpp>

namespace Tanks
{

class TankPhysicsComponent final : public CapEngine::PhysicsComponent
{
public:
  TankPhysicsComponent();
  ~TankPhysicsComponent() override = default;

  std::unique_ptr<Component> clone() const override;

  static std::unique_ptr<TankPhysicsComponent>
	  makeComponent(const jsoncons::json &in_json);
  static void registerConstructor(CapEngine::ComponentFactory &in_factory);

  void update(CapEngine::GameObject &object, double timestep) override;

public:
  //! The component type.
  static constexpr inline char kType[] = "TankPhysicsComponent";
};

} // namespace Tanks

#endif // TANKS_TANKPHYSICSCOMPONENT_H
