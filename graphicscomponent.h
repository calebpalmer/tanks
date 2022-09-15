#ifndef TANK_GRAPHICSCOMPONENT_H
#define TANK_GRAPHICSCOMPONENT_H

#include "capengine/CapEngine.h"
#include "capengine/animatorv2.h"

namespace Tanks {
class GraphicsComponent final : public CapEngine::GraphicsComponent {
  public:
    GraphicsComponent(int in_spritePath, int in_explosionAssetId);
    ~GraphicsComponent() override = default;

    std::unique_ptr<Component> clone() const override;

    static GraphicsComponent makeComponent(const jsoncons::json &in_json);
    static std::unique_ptr<GraphicsComponent> makeSharedComponent(const jsoncons::json &in_json);
    static void registerConstructor(CapEngine::ComponentFactory &in_factory);

    void update(CapEngine::GameObject &in_object, double timestep) override;
    void render(CapEngine::GameObject &in_object, const CapEngine::Camera2d &in_camera, uint32_t in_windowId) override;

  private:
    //! Component subtype.
    static constexpr char kComponentType[] = "TanksPlayerGraphicsComponent";

    //! property name of sprite asset id
    static constexpr char kSpritePathProperty[] = "spriteAssetId";
    //! property name of the default orientation
    static constexpr char kSpriteDefaultOrientation[] = "defaultOrientation";

    int m_spriteAssetId;                                   //<! asset id of sprite.
    CapEngine::Vector m_defaultOrientation;                //<! default orientation of the sprite.
    std::optional<CapEngine::AnimatorV2> m_deathAnimation; //<! The death animation
};

} // namespace Tanks

#endif // TANK_GRAPHICSCOMPONENT_H
