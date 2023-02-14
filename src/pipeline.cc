#include "pipeline.h"

#include "geometry.h"

#include <cmath>
#include <limits>

void Pipeline::DrawTriangle(DrawPixelCallback on_draw_pixel,
							const DirectX::XMVECTOR &p0,
							const DirectX::XMVECTOR &p1,
							const DirectX::XMVECTOR &p2)
{
	const auto kEdge01 = DirectX::XMVectorSubtract(p1, p0);
	const auto kEdge02 = DirectX::XMVectorSubtract(p2, p0);
	const auto kDet = DirectX::XMVector2Cross(kEdge01, kEdge02);

	if (DirectX::XMVector2NearEqual(kDet, DirectX::XMVectorZero(), DirectX::XMVectorSplatEpsilon()))
	{
		return;
	}

	const auto kReciprocal = DirectX::XMVectorReciprocal(kDet);
	const auto kOffset = DirectX::XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f);
	const auto kEdge12 = DirectX::XMVectorSubtract(p2, p1);
	const auto kEdge20 = DirectX::XMVectorSubtract(p0, p2);

	const auto kBoundingRect = Geometry::ComputeBoundingRect(p0, p1, p2);
	const auto kMinY = static_cast<int>(kBoundingRect.min_y);
	const auto kMaxY = static_cast<int>(kBoundingRect.max_y);
	const auto kMinX = static_cast<int>(kBoundingRect.min_x);
	const auto kMaxX = static_cast<int>(kBoundingRect.max_x);

	for (auto y = kMinY; y <= kMaxY; ++y)
	{
		const auto kY = static_cast<float>(y);
		for (auto x = kMinX; x <= kMaxX; ++x)
		{
			const auto kX = static_cast<float>(x);
			const auto kP = DirectX::XMVectorSet(kX, kY, 0.0f, 0.0f);
			const auto kPCentered = DirectX::XMVectorSubtract(kP, kOffset);
			const auto kW0 = DirectX::XMVectorMultiply(DirectX::XMVector2Cross(kEdge12, DirectX::XMVectorSubtract(kPCentered, p1)), kReciprocal);
			const auto kW1 = DirectX::XMVectorMultiply(DirectX::XMVector2Cross(kEdge20, DirectX::XMVectorSubtract(kPCentered, p2)), kReciprocal);
			const auto kW2 = DirectX::XMVectorMultiply(DirectX::XMVector2Cross(kEdge01, DirectX::XMVectorSubtract(kPCentered, p0)), kReciprocal);

			auto const kW0GreatorOrEqualZero = DirectX::XMVector2Greater(kW0, DirectX::XMVectorZero());
			auto const kW1GreatorOrEqualZero = DirectX::XMVector2Greater(kW1, DirectX::XMVectorZero());
			auto const kW2GreatorOrEqualZero = DirectX::XMVector2Greater(kW2, DirectX::XMVectorZero());
			auto const kAllGreaterThanZero = (kW0GreatorOrEqualZero && kW1GreatorOrEqualZero && kW2GreatorOrEqualZero);

			if (kAllGreaterThanZero == false)
			{
				continue;
			}

			on_draw_pixel(x, y, DirectX::XMVectorGetX(kW0), DirectX::XMVectorGetX(kW1), DirectX::XMVectorGetX(kW2));
		}
	}
}
