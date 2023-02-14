#pragma once

#include <DirectXMath.h>

namespace Geometry
{

	struct BoundingRect
	{
		float min_x = {};
		float max_x = {};
		float min_y = {};
		float max_y = {};
	};

	BoundingRect ComputeBoundingRect(DirectX::XMVECTOR const &p0, DirectX::XMVECTOR const &p1, DirectX::XMVECTOR const &p2);

	DirectX::XMMATRIX MakeViewportMatrix(const BoundingRect &window, const BoundingRect &viewport);

}
