#include "geometry.h"
#include "pipeline.h"
#include "frame_buffer.h"

#include <DirectXMath.h>
#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>
#include <numeric>

constexpr auto kWindowWidth = 320 * 2;
constexpr auto kWindowHeight = 240 * 2;

struct Vertex
{
	DirectX::XMVECTOR position;
};

static void ZSort(std::vector<uint16_t> &out_indices, std::vector<Vertex> const &vertices, std::vector<uint16_t> const &indices)
{
	auto const num_triangles = indices.size() / 3;

	std::vector<std::pair<int, float>> triangle_z_depths(num_triangles);

	for (int i = 0; i < num_triangles; ++i)
	{
		auto const idx0 = indices[i * 3 + 0];
		auto const idx1 = indices[i * 3 + 1];
		auto const idx2 = indices[i * 3 + 2];

		auto const &kV0 = vertices[idx0];
		auto const &kV1 = vertices[idx1];
		auto const &kV2 = vertices[idx2];

		auto const kAverageZDepth = (kV0.position.m128_f32[2] + kV1.position.m128_f32[2] + kV2.position.m128_f32[2]) / 3.0f;

		triangle_z_depths[i] = {i, kAverageZDepth};
	}

	std::sort(triangle_z_depths.begin(), triangle_z_depths.end(), [](auto const &lhs, auto const &rhs)
			  { return lhs.second > rhs.second; });

	std::vector<uint16_t> sorted_indices;

	for (auto const &[triangle_index, _] : triangle_z_depths)
	{
		sorted_indices.push_back(indices[triangle_index * 3 + 0]);
		sorted_indices.push_back(indices[triangle_index * 3 + 1]);
		sorted_indices.push_back(indices[triangle_index * 3 + 2]);
	}

	out_indices = sorted_indices;
}

static void DrawIndexedTriangles(Pipeline::DrawPixelCallback on_draw_pixel, const std::vector<Vertex> &vertices, const std::vector<uint16_t> &indices)
{
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		const auto &kV0 = vertices[indices[i + 0]].position;
		const auto &kV1 = vertices[indices[i + 1]].position;
		const auto &kV2 = vertices[indices[i + 2]].position;
		Pipeline::DrawTriangle(on_draw_pixel, kV0, kV1, kV2);
	}
}

static void TestZSort()
{
	std::vector<Vertex> vertices = {
		{DirectX::XMVectorSet(-1.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(1.0f, 1.0f, 0.5f, 1.0f)},
		{DirectX::XMVectorSet(-1.0f, -1.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet(1.0f, -1.0f, 1.5f, 1.0f)}};

	std::vector<uint16_t> indices = {0, 1, 2, 1, 3, 2};

	std::vector<uint16_t> sorted_indices;

	ZSort(sorted_indices, vertices, indices);

	assert(sorted_indices[0] == 1 && sorted_indices[1] == 3 && sorted_indices[2] == 2 &&
		   sorted_indices[3] == 0 && sorted_indices[4] == 1 && sorted_indices[5] == 2);
}

int main(int argc, char **argv)
{
	TestZSort();

	std::vector<Vertex> vertices = {
		{{0.5f, 0.0f, 4.0f, 1.0f}},
		{{-0.5f, 0.0f, 4.0f, 1.0f}},
		{{0.0f, 0.5f, 4.0f, 1.0f}},
		{{0.5f, 0.0f, 2.0f, 1.0f}},
		{{-0.5f, 0.0f, 2.0f, 1.0f}},
		{{0.0f, 0.5f, 2.0f, 1.0f}}};

	std::vector<uint16_t> indices = {0, 1, 2, 3, 4, 5};

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kWindowWidth, kWindowHeight, 0);
	SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 320, 240, 5, SDL_PIXELFORMAT_RGB555);

	const auto kViewportMatrix = Geometry::MakeViewportMatrix({-1, 1, -1, 1}, {0, 320, 240, 0});

	static auto frame_buffer = std::make_unique<FrameBuffer<320, 240>>();
	frame_buffer->Clear();

	std::vector<std::uint16_t> sorted_indices = {};
	ZSort(sorted_indices, vertices, indices);
	assert(sorted_indices.size() == indices.size());

	for (auto &v : vertices)
	{
		v.position = DirectX::XMVector4Transform(v.position, DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 1.0f, 100.0f));
		v.position = DirectX::XMVectorDivide(v.position, DirectX::XMVectorSplatW(v.position));
	}

	// Apply the window to viewport transform to the vertices.
	for (auto &v : vertices)
	{
		v.position = DirectX::XMVector4Transform(v.position, kViewportMatrix);
	}

	DrawIndexedTriangles(
		[](int x, int y, float alpha, float beta, float gamma)
		{
			const auto color = CreateColor(
				static_cast<uint8_t>(255.0f * alpha),
				static_cast<uint8_t>(255.0f * beta),
				static_cast<uint8_t>(255.0f * gamma));
			frame_buffer->SetPixel(x, y, color);
		},
		vertices, sorted_indices);

	frame_buffer->WritePpm("screenshot.ppm");

	// Copy the contents of the frame buffer to the SDL surface
	std::memcpy(surface->pixels, frame_buffer->GetPixelData(), frame_buffer->GetSize());

	// Scale the surface to the window size using SDL_BlitScaled
	auto window_surface = SDL_GetWindowSurface(window);
	SDL_BlitScaled(surface, nullptr, window_surface, nullptr);

	// Update the window's surface
	SDL_UpdateWindowSurface(window);

	SDL_Delay(5000);

	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
