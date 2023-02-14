#pragma once

#include <cstdint>

constexpr std::uint16_t CreateColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    /* Reference: https://learn.microsoft.com/en-us/windows/win32/directshow/working-with-16-bit-rgb */
    return (r >> 3) << 10 | (g >> 3) << 5 | (b >> 3);
}

enum Color : uint16_t
{
    kMint = CreateColor(165, 241, 233),
    kTeal = CreateColor(127, 233, 222),
    kYellow = CreateColor(255, 246, 191),
    kBeige = CreateColor(255, 235, 173),
    kThistle = CreateColor(227, 192, 211),
    kFulvous = CreateColor(226, 132, 19),
    kCerulean = CreateColor(36, 123, 160),
    kCeladon = CreateColor(150, 230, 179),
    kRose = CreateColor(244, 0, 118)
};
