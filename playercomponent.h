#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "capengine/componentfactory.h"
#include "capengine/components.h"
#include "capengine/gameobject.h"

#include "jsoncons/json.hpp"

namespace Tanks
{
class PlayerComponent : public CapEngine::CustomComponent
{
  public:
    PlayerComponent(unsigned int in_health);
    ~PlayerComponent() = default;

    void update(CapEngine::GameObject &object, double timestep){};
    std::unique_ptr<Component> clone() const override
    {
        return std::make_unique<PlayerComponent>(*this);
    }

    // component registration
    static PlayerComponent makeComponent(const jsoncons::json &in_json);
    static std::unique_ptr<PlayerComponent>
        makeSharedComponent(const jsoncons::json &in_json);
    static void registerConstructor(CapEngine::ComponentFactory &in_factory);

    static constexpr inline char kComponentType[] = "PlayerComponent";
    static constexpr inline char kHealthProperty[] = "health";

  private:
    unsigned int m_health = 1; //< Player health
};
} // namespace Tanks

#endif /* PLAYERCOMPONENT_H */
