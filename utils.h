#ifndef TANKS_UTILS_H
#define TANKS_UTILS_H

#include "capengine/gameobject.h"

#include <optional>

namespace Tanks {
std::optional<int> getPlayerNumber(CapEngine::GameObject const &in_object);
}

#endif /* TANKS_UTILS_H */
