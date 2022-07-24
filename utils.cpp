#include "utils.h"

namespace Tanks {

std::optional<int> getPlayerNumber(CapEngine::GameObject const &in_object) {
    // get the metadata and see if it's a player object
    CapEngine::GameObject::Metadata const &metadata = in_object.metadata();

    if (metadata.find("player") == metadata.end()) {
        return std::nullopt; // not a player, who cares.
    }

    CapEngine::MetadataType player = metadata.at("player");
    const int playerNumber = std::get<int>(player);
    return playerNumber;
}

} // namespace Tanks
