#include "projection.h"

namespace nocturne::math {

ScreenPos Projection::world_to_screen(const Vec3& worldPos,
                                         int screenW, int screenH,
                                         const Matrix4x4& viewMatrix) {
    ScreenPos result{0, 0, 0, false, false};

    Vec4 clip(
        viewMatrix.m[0][0] * worldPos.x + viewMatrix.m[0][1] * worldPos.y + viewMatrix.m[0][2] * worldPos.z + viewMatrix.m[0][3],
        viewMatrix.m[1][0] * worldPos.x + viewMatrix.m[1][1] * worldPos.y + viewMatrix.m[1][2] * worldPos.z + viewMatrix.m[1][3],
        viewMatrix.m[2][0] * worldPos.x + viewMatrix.m[2][1] * worldPos.y + viewMatrix.m[2][2] * worldPos.z + viewMatrix.m[2][3],
        viewMatrix.m[3][0] * worldPos.x + viewMatrix.m[3][1] * worldPos.y + viewMatrix.m[3][2] * worldPos.z + viewMatrix.m[3][3]
    );

    if (clip.w < 0.1f) return result;

    Vec3 ndc(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);

    result.x = (screenW / 2.0f) * (ndc.x + 1.0f);
    result.y = screenH - (screenH / 2.0f) * (ndc.y + 1.0f);
    result.depth = clip.w;
    result.onScreen = result.x >= 0 && result.x <= screenW && result.y >= 0 && result.y <= screenH;
    result.valid = true;

    return result;
}

} // namespace nocturne::math
