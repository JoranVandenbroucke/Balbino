//
// Copyright (c) 2024.
// Author: Joran.
//

// todo: rename this file (no utils, no helpers ...)
#pragma once
#include "compiler.hpp"

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace Balbino
{
#if defined __GNUC__
#    define PRETTY_FUNCTION __PRETTY_FUNCTION__
constexpr char PRETTY_FUNCTION_PREFIX{'='};
constexpr char PRETTY_FUNCTION_SUFFIX{']'};
#    define PRETTY_FUNCTION_SUFFIX ']'
#elif defined __clang__
#    define PRETTY_FUNCTION __FUNCSIG__
constexpr char PRETTY_FUNCTION_PREFIX{'='};
constexpr char PRETTY_FUNCTION_SUFFIX{']'};
#elif defined _MSC_VER
#    define PRETTY_FUNCTION __FUNCSIG__
constexpr char PRETTY_FUNCTION_PREFIX{'<'};
constexpr char PRETTY_FUNCTION_SUFFIX{'>'};
#endif

template <typename >
BALBINO_CONSTEXPR_SINCE_CXX14 auto Name() BALBINO_NOEXCEPT_SINCE_CXX11->std::string_view
{
    BALBINO_CONSTEXPR std::string_view functionName{PRETTY_FUNCTION};
    BALBINO_CONSTEXPR uint64_t first{functionName.find_first_not_of(' ', functionName.find_first_of(PRETTY_FUNCTION_PREFIX) + 1)};
    return functionName.substr(first, functionName.find_last_of(PRETTY_FUNCTION_SUFFIX) - first);
}

template <typename T>
BALBINO_CONSTEXPR_SINCE_CXX14 auto Hash() BALBINO_NOEXCEPT_SINCE_CXX11->std::size_t
{
    BALBINO_CONSTEXPR std::string_view name{Name<T>()};
    BALBINO_CONSTEXPR std::size_t prime{1099511628211ULL};
    std::size_t value{};
    for (auto&& curr : name)
    {
        value = (value ^ static_cast<std::size_t>(curr)) * prime;
    }
    return value;
}

#if defined(__cpp_lib_embed) || (defined(BALBINO_EMBED_HAS_INCLUDE) && BALBINO_EMBED_HAS_INCLUDE(<embed>))
#    include <embed>
using embed = std::embed;
#elif defined(BALBINO_EMBED_HAS_BUILTIN_EMBED) && (BALBINO_EMBED_HAS_BUILTIN_EMBED != 0)

#    include <cstddef>
#    include <span>
#    include <string_view>
#    if defined(BALBINO_EMBED_CHECK_TYPE_SIZE) && (BALBINO_EMBED_CHECK_TYPE_SIZE != 0)
#        include <cassert>
#    endif
namespace __detail
{
template <typename _Ty, bool _Str, bool _Dynamic, std::size_t _NElements, typename _StrView, typename... _Args>
    requires std::is_trivially_copyable_v<_Ty> and (sizeof...(_Args) <= 1)
BALBINO_CONSTEXPR_SINCE_CXX14 std::span<const _Ty, _Dynamic ? std::dynamic_extent : _NElements> __embed(_StrView __resource_name, _Args&&... __args) noexcept
{
    const _Ty* __res = nullptr;
    // always returns # of bytes
    const std::size_t __res_len = __builtin_embed(__resource_name.size(), __resource_name.data(), _Str, std::forward<_Args>(__args)..., &__res);
    std::span<const _Ty, _Dynamic ? std::dynamic_extent : _NElements> __elements(__res, (__res_len / sizeof(_Ty)));
    if constexpr (!_Dynamic)
    {
        if (__elements.size() < (sizeof(_Ty) * _NElements))
        {
            throw "cannot have a fixed-size span that returns less than _NElements elements";
        }
    }
    return __elements;
}
} // namespace __detail
template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::wstring_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    ifdef __cpp_char8_t

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::u8string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    endif // char8_t shenanigans

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::u16string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::u32string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

// fixed size
template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::wstring_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    ifdef __cpp_char8_t

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::u8string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    endif // char8_t shenanigans

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::u16string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::u32string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, false, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

// string embed
// dynamic size
template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::wstring_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    ifdef __cpp_char8_t

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::u8string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    endif // char8_t shenanigans

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::u16string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty> embed(std::u32string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, true, 0>(move(__resource_name), std::forward<_Args>(__args)...);
}

// fixed size
template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::wstring_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    ifdef __cpp_char8_t

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::u8string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

#    endif // char8_t shenanigans

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::u16string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}

template <std::size_t _NElements, typename _Ty = std::byte, typename... _Args>
constexpr std::span<const _Ty, _NElements> embed(std::u32string_view __resource_name, _Args&&... __args) noexcept
{
    return __detail::__embed<_Ty, true, false, _NElements>(move(__resource_name), std::forward<_Args>(__args)...);
}
#endif
} // namespace Balbino