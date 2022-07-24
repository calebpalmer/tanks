#include "hud.h"
#include "capengine/VideoManager.h"
#include "capengine/collision.h"
#include "capengine/colour.h"
#include "capengine/locator.h"

#include <iostream>

namespace Tanks {

Hud::Hud(uint32_t in_windowId) : m_windowId(in_windowId) {}

void Hud::render() { this->drawHealthIndicators(); }

void Hud::updateHealth(Player in_player, int in_health) {
    if (in_player == Player::One) {
        m_player1Health = in_health;
    } else {
        m_player2Health = in_health;
    }
}

void Hud::drawHealthIndicators() const {
    assert(CapEngine::Locator::videoManager != nullptr);

    auto [width, height] = CapEngine::Locator::videoManager->getWindowLogicalResolution(m_windowId);
    int xBuffer = 25;
    int yBuffer = 10;
    int indicatorWidth = static_cast<double>(width) * 0.25;
    int indicatorHeight = static_cast<double>(indicatorWidth) * (1.0 / 8.0);

    CapEngine::Rect indicator1Rect{
        xBuffer,
        yBuffer,
        indicatorWidth,
        indicatorHeight,
    };

    CapEngine::Rect indicator2Rect{width - indicatorWidth - xBuffer, yBuffer, indicatorWidth, indicatorHeight};

    CapEngine::Locator::videoManager->drawRect(m_windowId, indicator1Rect, CapEngine::Colour{0, 0, 0});
    CapEngine::Locator::videoManager->drawRect(m_windowId, indicator2Rect, CapEngine::Colour{0, 0, 0});

    int healthBuffer = 1;

    CapEngine::Rect indicator1HealthBackroundRect{indicator1Rect.x + healthBuffer, indicator1Rect.y + healthBuffer,
                                                  indicator1Rect.w - (healthBuffer * 2),
                                                  indicator1Rect.h - (healthBuffer * 2)};
    CapEngine::Rect indicator2HealthBackgroundRect{indicator2Rect.x + healthBuffer, indicator2Rect.y + healthBuffer,
                                                   (indicator2Rect.w - (healthBuffer * 2)),
                                                   indicator2Rect.h - (healthBuffer * 2)};

    CapEngine::Colour healthBackgroundColour{255, 255, 255};
    CapEngine::Locator::videoManager->drawFillRect(m_windowId, indicator1HealthBackroundRect, healthBackgroundColour);
    CapEngine::Locator::videoManager->drawFillRect(m_windowId, indicator2HealthBackgroundRect, healthBackgroundColour);

    CapEngine::Rect indicator1HealthRect{indicator1Rect.x + healthBuffer, indicator1Rect.y + healthBuffer,
                                         static_cast<double>(indicator1Rect.w - (healthBuffer * 2)) *
                                             (static_cast<double>(m_player1Health) / 100.0),
                                         indicator1Rect.h - (healthBuffer * 2)};
    CapEngine::Rect indicator2HealthRect{indicator2Rect.x + healthBuffer, indicator2Rect.y + healthBuffer,
                                         static_cast<double>((indicator2Rect.w - (healthBuffer * 2))) *
                                             (static_cast<double>(m_player2Health) / 100.0),
                                         indicator2Rect.h - (healthBuffer * 2)};

    CapEngine::Colour healthColour{0, 255, 0};
    CapEngine::Locator::videoManager->drawFillRect(m_windowId, indicator1HealthRect, healthColour);
    CapEngine::Locator::videoManager->drawFillRect(m_windowId, indicator2HealthRect, healthColour);
}
} // namespace Tanks
