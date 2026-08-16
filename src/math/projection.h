#pragma once
#include "vec3.h"
#include "matrix.h"

namespace nocturne::math {

struct ScreenPos {
    float x, y;
    float depth;
    bool onScreen;
    bool valid;
};

class Projection {
public:
    static ScreenPos world_to_screen(const Vec3& worldPos,
                                      int screenW, int screenH,
                                      const Matrix4x4& viewMatrix);
};

} // namespace nocturne::math
