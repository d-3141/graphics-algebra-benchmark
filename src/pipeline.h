#pragma once

#include <functional>

#include <DirectXMath.h>

namespace Pipeline
{

    using DrawPixelCallback = std::function<void(int, int, float, float, float)>;

    void DrawTriangle(DrawPixelCallback on_draw_pixel,
                      const DirectX::XMVECTOR &p0, const DirectX::XMVECTOR &p1, const DirectX::XMVECTOR &p2);
}
