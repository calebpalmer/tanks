#ifndef TANKS_PROJECTILEPHYSICSCOMPONENT_H
#define TANKS_PROJECTILEPHYSICSCOMPONENT_H

#include "capengine/componentfactory.h"
#include "capengine/components.h"
#include "capengine/vector.h"

#include <jsoncons/json.hpp>

namespace Tanks
{

class ProjectilePhysicsComponent final : public CapEngine::PhysicsComponent
{
  public:
    ProjectilePhysicsComponent(int in_width, int in_height,
                               CapEngine::Vector in_velocity);
    ~ProjectilePhysicsComponent() = default;

    std::unique_ptr<Component> clone() const override;

    static std::unique_ptr<ProjectilePhysicsComponent>
        makeComponent(const jsoncons::json &in_json);

    static void registerConstructor(CapEngine::ComponentFactory &in_factory);

    void update(CapEngine::GameObject &object, double timestep) override;

    std::optional<CapEngine::Rectangle>
        boundingPolygon(const CapEngine::GameObject &in_object) const override;

    bool handleCollision(CapEngine::CollisionType in_collisionType,
                         CapEngine::CollisionClass in_collisionClass,
                         CapEngine::GameObject &in_Object,
                         std::optional<CapEngine::GameObject *> in_OtherObject,
                         const CapEngine::Vector &collisionLocation) override;

  public:
    //! The component type.
    static constexpr char kType[] = "ProjectilePhysicsComponent";

    //! The property name for the height.
    static constexpr char kWidthPropertyName[] = "width";
    //! The property name for the height.
    static constexpr char kHeightPropertyName[] = "height";
    //! The property name for the velocity.
    static constexpr char kVelocityPropertyName[] = "velocity";

    int m_width;                  //< The width of the projectile.
    int m_height;                 //< The height of the projectile.
    CapEngine::Vector m_velocity; //< The velocity of the projectile.
};
} // namespace Tanks

#endif /* TANKS_PROJECTILEPHYSICSCOMPONENT_H */
