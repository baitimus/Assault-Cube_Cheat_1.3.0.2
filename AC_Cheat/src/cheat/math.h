#pragma once

#include "entity/entity.h" // Assuming the entity class is in entity.h

namespace myMath {
    // Structures for vectors and angles
    struct Vec3 {
        float x, y, z;
    };

    struct Vec2 {
        float x, y;
    };

    struct Vec4 {
        float x, y, z, w;
    };

    struct ViewAngles {
        float pitch;
        float yaw;
    };

    // Function declarations
    bool WorldToScreen(const entity& ent, Vec2* screen, float matrix[16], int windowWidth, int windowHeight);
}
