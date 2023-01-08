#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "glm/glm.hpp"

namespace Asura::Math {
    bool decomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation,
                            glm::vec3& outRotation, glm::vec3& outScale);
}

#endif //GAME_MATH_H
