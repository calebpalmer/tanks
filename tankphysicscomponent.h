#ifndef TANKS_TANKPHYSICSCOMPONENT_H
#define TANKS_TANKPHYSICSCOMPONENT_H

#include "capengine/componentfactory.h"
#include "capengine/components.h"
#include "capengine/gameobject.h"

#include <jsoncons/json.hpp>

namespace Tanks
{

class TankPhysicsComponent final : public CapEngine::PhysicsComponent
{
  public:
    TankPhysicsComponent(
        std::optional<CapEngine::Vector> in_projectileOffset = std::nullopt);
    ~TankPhysicsComponent() override = default;

    std::unique_ptr<Component> clone() const override;

    static std::unique_ptr<TankPhysicsComponent>
        makeComponent(const jsoncons::json &in_json);
    static void registerConstructor(CapEngine::ComponentFactory &in_factory);

    void update(CapEngine::GameObject &object, double timestep) override;
    void receive(CapEngine::GameObject &in_object, int in_messageId,
                 const std::string &in_message) override;

  public:
    //! The component type.
    static constexpr inline char kType[] = "TankPhysicsComponent";

    //! offset from center of object for projectiles
    CapEngine::Vector m_projectileOffset = {0.0, 0.0, 0.0};

  private:
    static constexpr int m_soundId = 2000;
};

} // namespace Tanks

#endif // TANKS_TANKPHYSICSCOMPONENT_H
