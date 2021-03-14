#include "graphicscomponent.h"

#include "capengine/CapEngineException.h"
#include "capengine/componentutils.h"
#include "capengine/collision.h"
#include "capengine/scene2dutils.h"
#include "capengine/trigonometry.h"

#include <memory>
#include <cmath>

namespace {
  const int kAssetId = 1000;
}

namespace Tanks {

/**
 * @brief Constructor.
 * @param[in] in_spritePath The path to the sprite.
 */
GraphicsComponent::GraphicsComponent(int in_spriteAssetId)
  : m_spriteAssetId(in_spriteAssetId)
{}

/**
 * @brief Make the cmponent.
 * @param[in] in_json The json describing the component.
 * @return The Component.
 */
GraphicsComponent
	GraphicsComponent::makeComponent(const jsoncons::json &in_json)
{
  CapEngine::ComponentUtils::assertPropertyExists(in_json, GraphicsComponent::kSpritePathProperty);
  const auto spriteAssetId = in_json.at(GraphicsComponent::kSpritePathProperty).as_int();

  auto defaultOrientation = CapEngine::Vector(0.0, 1.0, 0.0);
  if(in_json.has_key(GraphicsComponent::kSpriteDefaultOrientation))
	defaultOrientation = CapEngine::JSONUtils::readVector(in_json[GraphicsComponent::kSpriteDefaultOrientation]);

  return GraphicsComponent(spriteAssetId);
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
void GraphicsComponent::update(CapEngine::GameObject&, double) {}

//! @copydoc CapEngine::GraphicsComponent::render
void GraphicsComponent::render(CapEngine::GameObject &in_object, const CapEngine::Camera2d &in_camera, uint32_t in_windowId)
{
  const auto objectRect = in_object.boundingPolygon();
  const auto doYFlip =
	in_object.getYAxisOrientation() == CapEngine::YAxisOrientation::BottomZero;

  CapEngine::Rectangle drawRect =
	  toScreenCoords(in_camera, objectRect, in_windowId, doYFlip);

  assert(CapEngine::Locator::videoManager != nullptr);

  auto *assetManager = CapEngine::Locator::assetManager;
  const auto srcRect = CapEngine::Rectangle(0, 0, assetManager->getImageHeight(m_spriteAssetId), assetManager->getImageHeight(m_spriteAssetId));

  // calculate any rotation of the sprite depending on the angle.
  const auto& orientation = in_object.getOrientation();
  const auto angle = (-1) * RADTODEG(atan2(orientation.getY(), orientation.getX()));

  CapEngine::Locator::assetManager->draw(in_windowId, m_spriteAssetId, srcRect, drawRect, angle);
}

} // namespace Tanks
