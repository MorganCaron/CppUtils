#pragma once

#include <string_view>

namespace CppUtils::Hash
{
    constexpr inline std::size_t constexprHash(const char* str, const size_t size) noexcept
    {
        return size == 0 ? 0xcbf29ce484222325u : (static_cast<std::size_t>(str[0]) ^ constexprHash(str + 1, size - 1)) * 0x100000001b3u;
    }

    constexpr inline std::size_t constexprHash(std::string_view str) noexcept
    {
        return constexprHash(str.data(), str.size());
    }
}
