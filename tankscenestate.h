#ifndef TANKS_TANKSCENESTATE_H
#define TANKS_TANKSCENESTATE_H

#include "capengine/scene2dstate.h"

#include "jsoncons/json.hpp"
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>

namespace Tanks
{

class TankSceneState final : public CapEngine::Scene2dState
{
  public:
    TankSceneState(jsoncons::json in_sceneDescriptors, std::string in_sceneId,
                   uint32_t in_windowId);
    ~TankSceneState() override = default;

    bool onPause() override;
    bool onResume() override;

  private:
    boost::signals2::scoped_connection m_keyboardEventConnection;
};

} // namespace Tanks

#endif /* TANKS_TANKSCENESTATE_H */
