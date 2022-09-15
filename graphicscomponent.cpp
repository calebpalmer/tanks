#include "graphicscomponent.h"

#include "animatorv2.h"
#include "capengine/CapEngineException.h"
#include "capengine/asset_manager.h"
#include "capengine/collision.h"
#include "capengine/componentutils.h"
#include "capengine/locator.h"
#include "capengine/scene2dutils.h"
#include "capengine/trigonometry.h"
#include "gameobject.h"

#include <cmath>
#include <memory>
#include <pthread.h>

namespace {
const int kAssetId = 1000;
const int kPlayer1DeathAnimationId = 1002;
const int kPlayer2DeathAnimationId = 1003;
} // namespace

namespace Tanks {

/**
 * @brief Constructor.
 * @param[in] in_spritePath The path to the sprite.
 */
GraphicsComponent::GraphicsComponent(int in_spriteAssetId, int in_explosionAssetId)
    : m_spriteAssetId(in_spriteAssetId) {
    assert(CapEngine::Locator::assetManager != nullptr);
    const std::optional<CapEngine::AnimatedImage> maybeImage =
        CapEngine::Locator::assetManager->getAnimatedImage(in_explosionAssetId);
    assert(maybeImage != std::nullopt);

    m_deathAnimation = CapEngine::AnimatorV2(*maybeImage);
}

/**
 * @brief Make the cmponent.
 * @param[in] in_json The json describing the component.
 * @return The Component.
 */
GraphicsComponent GraphicsComponent::makeComponent(const jsoncons::json &in_json) {
    CapEngine::ComponentUtils::assertPropertyExists(in_json, GraphicsComponent::kSpritePathProperty);
    const auto spriteAssetId = in_json.at(GraphicsComponent::kSpritePathProperty).as_int();

    auto defaultOrientation = CapEngine::Vector(0.0, 1.0, 0.0);
    if (in_json.has_key(GraphicsComponent::kSpriteDefaultOrientation))
        defaultOrientation = CapEngine::JSONUtils::readVector(in_json[GraphicsComponent::kSpriteDefaultOrientation]);

    // get the player number to determine which explosion to use
    int playerNum = 1;
    if (in_json.has_key("metadata")) {
        const auto metadata = in_json["metadata"];
        if (metadata.has_key("player")) {
            playerNum = metadata["player"].as_int();
        }
    }

    int animationAssetId = playerNum == 2 ? kPlayer2DeathAnimationId : kPlayer1DeathAnimationId;
    return GraphicsComponent(spriteAssetId, animationAssetId);
}

/**
 * @brief make the shared component from json
 * @param[in] in_json Description

 {
   "type" : "GraphicsComponent",
   "subtype": "TankPlayerGraphicsComponent",
   "spritePath": "tank.png"
 }

 * @return The Graphics Component.
 */
std::unique_ptr<GraphicsComponent> GraphicsComponent::makeSharedComponent(const jsoncons::json &in_json)
{
  return std::make_unique<GraphicsComponent>(makeComponent(in_json));
}

/**
 * @brief Clone the component.
 * @return The cloned component.
 */
std::unique_ptr<CapEngine::Component> GraphicsComponent::clone() const
{
  return std::make_unique<GraphicsComponent>(*this);
}

/**
 * @brief register the constructor for the component.
 * @param[in] in_factory The factory to register it with.
 */
void GraphicsComponent::registerConstructor(
	CapEngine::ComponentFactory &in_factory)
{
  in_factory.registerComponentType(
	  CapEngine::ComponentUtils::componentTypeToString(
		  CapEngine::ComponentType::Graphics),
	  GraphicsComponent::kComponentType,
	  GraphicsComponent::makeSharedComponent);
}

//!  @copydoc CapEngine::Component::update
void GraphicsComponent::update(CapEngine::GameObject &in_object, double in_ms) {
    if (in_object.getObjectState() == CapEngine::GameObject::Dying && !m_deathAnimation->started()) {
        m_deathAnimation->start();
    }

    if (m_deathAnimation->started() && !m_deathAnimation->finished()) {
        m_deathAnimation->update(in_ms);
    }

    if (m_deathAnimation->finished()) {
        in_object.setObjectState(CapEngine::GameObject::Dead);
    }
}

//! @copydoc CapEngine::GraphicsComponent::render
void GraphicsComponent::render(CapEngine::GameObject &in_object, const CapEngine::Camera2d &in_camera,
                               uint32_t in_windowId) {
    const auto objectRect = in_object.boundingPolygon();
    const auto doYFlip = in_object.getYAxisOrientation() == CapEngine::YAxisOrientation::BottomZero;

    CapEngine::Rectangle drawRect = toScreenCoords(in_camera, objectRect, in_windowId, doYFlip);

    assert(CapEngine::Locator::videoManager != nullptr);

    auto *assetManager = CapEngine::Locator::assetManager;
    const auto srcRect = CapEngine::Rectangle(0, 0, assetManager->getImageHeight(m_spriteAssetId),
                                              assetManager->getImageHeight(m_spriteAssetId));

    // calculate any rotation of the sprite depending on the angle.
    const auto &orientation = in_object.getOrientation();
    const auto angle = (-1) * RADTODEG(orientation.angle2d());

    if (in_object.getObjectState() == CapEngine::GameObject::Active) {
        CapEngine::Locator::assetManager->draw(in_windowId, m_spriteAssetId, srcRect, drawRect, angle);
    }

    else if (in_object.getObjectState() == CapEngine::GameObject::Dying && m_deathAnimation->started()) {
        m_deathAnimation->render(in_windowId, drawRect.toRect(), angle);
    }
}

} // namespace Tanks
