#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(glm::mat3(
            1.0f, 0.0f, translateX,
            0.0f, 1.0f, translateY,
            0.0f, 0.0f, 1.0f));
    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(glm::mat3(
            scaleX, 0.0f,   0.0f,
            0.0f,   scaleY, 0.0f,
            0.0f,   0.0f,   1.0f));
    }

    // Uniform scale matrix
    inline glm::mat3 Scale(float scaleX)
    {
        return Scale(scaleX, scaleX);
    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        return glm::transpose(glm::mat3(
            cos(radians),  -sin(radians), 0.0f,
            sin(radians),  cos(radians),  0.0f,
            0.0f,          0.0f,          1.0f));
    }
}   // namespace transform2D
