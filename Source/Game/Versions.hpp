//
// Copyright (c) 2024.
// Author: Joran
//

#pragma once
#ifndef VERSIONS_HPP
#define VERSIONS_HPP
#include <cstddef>
#include <locale>

#if defined(_WIN32)
#    define BALBINO_PLATFORM_WINDOWS
#elif defined(__linux__)
#    define BALBINO_PLATFORM_LINUX
#elif defined(__APPLE__)
#    define BALBINO_PLATFORM_MAC
#endif

#ifdef __cplusplus
#    if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#        define BALBINO_STL_LANG _MSVC_LANG
#    else // ^^^ language mode is _MSVC_LANG / language mode is __cplusplus vvv
#        define BALBINO_STL_LANG __cplusplus
#    endif // ^^^ language mode is larger of _MSVC_LANG and __cplusplus ^^^
#else      // ^^^ determine compiler's C++ mode / no C++ support vvv
#    define BALBINO_STL_LANG 0L
#endif // ^^^ no C++ support ^^^

#if BALBINO_STL_LANG <= 199711L
#    define BALBINO_STD_VERSION 98
#elif BALBINO_STL_LANG <= 201103L
#    define BALBINO_STD_VERSION 11
#elif BALBINO_STL_LANG <= 201402L
#    define BALBINO_STD_VERSION 14
#elif BALBINO_STL_LANG <= 201703L
#    define BALBINO_STD_VERSION 17
#elif BALBINO_STL_LANG <= 202002L
#    define BALBINO_STD_VERSION 20
#elif BALBINO_STL_LANG <= 202302L
#    define BALBINO_STD_VERSION 23
#else
// Expected release year of the next C++ standard
#    define BALBINO_STD_VERSION 26
#endif

#if BALBINO_STD_VERSION < 11
#    define BALBINO_NOEXCEPT_SINCE_CXX11
#    define BALBINO_CONSTEXPR_SINCE_CXX11
#    define BALBINO_CONSTEXPR_SINCE_CXX11
#    define BALBINO_CONSTEXPR const
#    define BALBINO_NULL NULL
#else
#    define BALBINO_NOEXCEPT_SINCE_CXX11 noexcept
#    define BALBINO_CONSTEXPR_SINCE_CXX11 constexpr
#    define BALBINO_EXPLICIT_SINCE_CXX11 explicit
#    define BALBINO_CONSTEXPR constexpr
#    define BALBINO_NULL nullptr
#endif

#if BALBINO_STD_VERSION < 14
#    define BALBINO_EXPLICIT_SINCE_CXX11
#else
#    define BALBINO_CONSTEXPR_SINCE_CXX14 constexpr
#endif

#if BALBINO_STD_VERSION < 17
#    define BALBINO_CONSTEXPR_SINCE_CXX17
#    define BALBINO_NODISCARD_SINCE_CXX17
#else
#    define BALBINO_CONSTEXPR_SINCE_CXX17 constexpr
#    define BALBINO_NODISCARD_SINCE_CXX17 [[nodiscard]]
#endif

#if BALBINO_STD_VERSION < 20
#    define BALBINO_CONSTEXPR_SINCE_CXX20
#else
#    define BALBINO_CONSTEXPR_SINCE_CXX20 constexpr
#endif

#if BALBINO_STD_VERSION < 20
#    define BALBINO_CONSTEXPR_SINCE_CXX23
#else
#    define BALBINO_CONSTEXPR_SINCE_CXX23 constexpr
#endif

#ifdef BALBINO_DEBUG
#    define BALBINO_ASSERT(expr)                                                                                                                                                                                                               \
        if (!(expr))                                                                                                                                                                                                                           \
        {                                                                                                                                                                                                                                      \
            std::cerr << "Assertion failed: " #expr << std::endl;                                                                                                                                                                              \
            std::abort();                                                                                                                                                                                                                      \
        }
#else
#    define BALBINO_ASSERT(expr) ((void)0)
#endif

#if defined(__GNUC__) || defined(__clang__)
#    define BALBINO_LIKELY(expr) __builtin_expect(!!(expr), 1)
#    define BALBINO_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#    define BALBINO_LIKELY(expr) (expr)
#    define BALBINO_UNLIKELY(expr) (expr)
#endif

#define BALBINO_ERROR(message)                                                                                                                                                                                                                 \
    do                                                                                                                                                                                                                                         \
    {                                                                                                                                                                                                                                          \
        std::cerr << "Error: " << message << std::endl;                                                                                                                                                                                        \
        std::abort();                                                                                                                                                                                                                          \
    } while (0)

#define QUOTES(x) #x
#define QUOTE(x) QUOTES(x)
#define FILE_LINE_QUOTE __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE(x) message(x)
#define FILE_LINE message(FILE_LINE_QUOTE)

#define TODO(x)                                                                                                                                                                                                                                \
    message(FILE_LINE_QUOTE  "\n"                                                                                                                                                                                                                    \
                       " ------------------------------------------------\n"                                                                                                                                                                   \
                       "|  TODO :   " #x "\n"                                                                                                                                                                                                  \
                       " -------------------------------------------------\n")
#define FIXME(x)                                                                                                                                                                                                                               \
    message(FILE_LINE_QUOTE  "\n"                                                                                                                                                                                                                    \
                       " ------------------------------------------------\n"                                                                                                                                                                   \
                       "|  FIXME :  " #x "\n"                                                                                                                                                                                                  \
                       " -------------------------------------------------\n")
#define todo(x) message(FILE_LINE_QUOTE " TODO :   " #x "\n")
#define fixme(x) message(FILE_LINE_QUOTE " FIXME:   " #x "\n")

namespace Balbino
{
#if defined __GNUC__
#    define PRETTY_FUNCTION __PRETTY_FUNCTION__
#    define PRETTY_FUNCTION_PREFIX '='
#    define PRETTY_FUNCTION_SUFFIX ']'
#elif defined __clang__
#    define PRETTY_FUNCTION __FUNCSIG__
#    define PRETTY_FUNCTION_PREFIX '='
#    define PRETTY_FUNCTION_SUFFIX ']'
#elif defined _MSC_VER
#    define PRETTY_FUNCTION __FUNCSIG__
#    define PRETTY_FUNCTION_PREFIX '<'
#    define PRETTY_FUNCTION_SUFFIX '>'
#endif

template <typename T>
BALBINO_CONSTEXPR_SINCE_CXX17 auto Name() BALBINO_NOEXCEPT_SINCE_CXX11 -> std::string_view
{
    BALBINO_CONSTEXPR std::string_view functionName{PRETTY_FUNCTION};
    BALBINO_CONSTEXPR uint64 first{functionName.find_first_not_of(' ', functionName.find_first_of(PRETTY_FUNCTION_PREFIX) + 1)};
    return functionName.substr(first, functionName.find_last_of(PRETTY_FUNCTION_SUFFIX) - first);
}

template <typename T>
BALBINO_CONSTEXPR_SINCE_CXX17 auto Hash() BALBINO_NOEXCEPT_SINCE_CXX11 -> std::size_t
{
    return std::hash<std::string_view>{}(Name<T>());
}

#define MAKE_VERSION(ma, mi, pa) uint32((uint8(ma) << 24) | uint8(mi) << 16 | uint16(pa))
} // namespace Balbino

#pragma region enum templates
template <typename>
struct SIsEnumFlag : std::false_type
{
    using EnumNonFlagType = void;
};

template <typename TEnumType>
concept EnumType = std::is_enum_v<TEnumType>;

template <typename TEnumType>
concept EnumFlagType = SIsEnumFlag<TEnumType>::value;

template <EnumType TEnumType>
BALBINO_CONSTEXPR_SINCE_CXX17 auto operator|(const TEnumType a_Lhs, const TEnumType a_Rhs) noexcept -> TEnumType requires EnumFlagType<TEnumType>
{
    using UnderlyingType = std::underlying_type_t<TEnumType>;
    return static_cast<const TEnumType>(static_cast<const UnderlyingType>(a_Lhs) | static_cast<const UnderlyingType>(a_Rhs));
}

template <EnumType TEnumType>
BALBINO_CONSTEXPR_SINCE_CXX17 auto operator&(const TEnumType a_Lhs, const TEnumType a_Rhs) noexcept -> TEnumType requires EnumFlagType<TEnumType>
{
    using UnderlyingType = std::underlying_type_t<TEnumType>;
    return static_cast<TEnumType>(static_cast<const UnderlyingType>(a_Lhs) & static_cast<const UnderlyingType>(a_Rhs));
}

template <EnumType TEnumType>
BALBINO_CONSTEXPR_SINCE_CXX17 auto operator^(const TEnumType a_Lhs, const TEnumType a_Rhs) noexcept -> TEnumType requires EnumFlagType<TEnumType>
{
    using UnderlyingType = std::underlying_type_t<TEnumType>;
    return static_cast<const TEnumType>(static_cast<const UnderlyingType>(a_Lhs) ^ static_cast<const UnderlyingType>(a_Rhs));
}

#define ENABLE_ENUM_BITWISE_OPERATIONS(enumClassFlagType)                                                                                                                                                                                      \
    template <>                                                                                                                                                                                                                                \
    struct SIsEnumFlag<enumClassFlagType> : std::true_type                                                                                                                                                                                     \
    {                                                                                                                                                                                                                                          \
    };

#pragma endregion
#endif
