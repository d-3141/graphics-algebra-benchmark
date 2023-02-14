#include "geometry.h"

Geometry::BoundingRect Geometry::ComputeBoundingRect(DirectX::XMVECTOR const& p0, DirectX::XMVECTOR const& p1, DirectX::XMVECTOR const& p2)
{
	auto const kMins = DirectX::XMVectorMin(DirectX::XMVectorMin(p0, p1), p2);
	auto const kMaxes = DirectX::XMVectorMax(DirectX::XMVectorMax(p0, p1), p2);

	float const kMinX = DirectX::XMVectorGetX(kMins);
	float const kMinY = DirectX::XMVectorGetY(kMins);
	float const kMaxX = DirectX::XMVectorGetX(kMaxes);
	float const kMaxY = DirectX::XMVectorGetY(kMaxes);

	return { kMinX, kMaxX, kMinY, kMaxY };
}

DirectX::XMMATRIX Geometry::MakeViewportMatrix(const BoundingRect& window, const BoundingRect& viewport)
{
	float const kSx = (viewport.max_x - viewport.min_x) / (window.max_x - window.min_x);
	float const kSy = (viewport.max_y - viewport.min_y) / (window.max_y - window.min_y);

	auto const kWindowTranslation = DirectX::XMMatrixTranslation(-window.min_x, -window.min_y, 0.0f);
	auto const kScaling = DirectX::XMMatrixScaling(kSx, kSy, 1.0f);
	auto const kViewportTranslation = DirectX::XMMatrixTranslation(viewport.min_x, viewport.min_y, 0.0f);

	/* Hint: T_window x S x T_viewport */
	return XMMatrixMultiply(XMMatrixMultiply(kWindowTranslation, kScaling), kViewportTranslation);
}
