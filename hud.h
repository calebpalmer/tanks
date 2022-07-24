#ifndef TANKS_HUD_H
#define TANKS_HUD_H

#include <cstdint>

namespace Tanks {
class Hud {
  public:
    enum class Player { One, Two };

    Hud(uint32_t in_windowId);

    void render();
    void updateHealth(Player in_player, int in_health);

    static constexpr char locatorId[] = "TanksHud";

  private:
    void drawHealthIndicators() const;
    uint32_t m_windowId = 0;
    int m_player1Health = 0;
    int m_player2Health = 0;
};
} // namespace Tanks

#endif /* TANKS_HUD_H */
