#pragma once

#include "common.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>

template <int Width, int Height>
class FrameBuffer
{
public:
    void Clear(uint16_t color = Color::kCerulean)
    {
        pixels_.fill(color);
    }

    uint16_t GetPixel(const int x = {}, const int y = {}) const
    {
        return pixels_[x + y * Width];
    }

    void SetPixel(const int x = {}, const int y = {}, const uint16_t color = Color::kRose)
    {
        const auto kPixelOutside = (x > Width || y > Height || x < 0 || y < 0);
        if (kPixelOutside)
        {
            return;
        }
        pixels_[x + y * Width] = color;
    }

    void WritePpm(const std::string &fileName)
    {
        constexpr auto kRedMask = 0x7C00U;
        constexpr auto kGreenMask = 0x3E0U;
        constexpr auto kBlueMask = 0x001FU;

        std::ofstream ofs(fileName);
        ofs << "P3\n"
            << Width << " " << Height << "\n255\n";
        for (auto &pixel : pixels_)
        {
            /* Reference: https://learn.microsoft.com/en-us/windows/win32/directshow/working-with-16-bit-rgb */
            const auto kRedValue = (pixel & kRedMask) >> 10U;
            const auto kGreenValue = (pixel & kGreenMask) >> 5U;
            const auto kBlueValue = (pixel & kBlueMask);
            const auto kRed = static_cast<uint8_t>(kRedValue << 3U);
            const auto kGreen = static_cast<uint8_t>(kGreenValue << 3U);
            const auto kBlue = static_cast<uint8_t>(kBlueValue << 3U);

            ofs << +kRed << " "
                << +kGreen << " "
                << +kBlue << '\n';
        }
    }

    const auto GetPixelData() const
    {
        return pixels_.data();
    }

    static constexpr auto GetWidth()
    {
        return Width;
    }

    static constexpr auto GetHeight()
    {
        return Height;
    }

    static constexpr auto GetResolution()
    {
        return Width * Height;
    }

    static constexpr auto GetSize()
    {
        return Width * Height * sizeof(std::uint16_t);
    }

private:
    std::array<uint16_t, Width * Height> pixels_;
};
