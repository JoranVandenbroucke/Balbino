//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once

#define BALBINO_COMPILER_MSVC 1
#define BALBINO_COMPILER_CLANG 2
#define BALBINO_COMPILER_GNU 3

#if __GNUC__
#define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_GNU
#elif __clang__
#define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_CLANG
#elif __EMSCRIPTEN__
#elif __MINGW32__
#elif __MINGW32__
#elif __MINGW64__
#elif _MSC_VER
#define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_MSVC
#endif

#ifdef __cplusplus
#    if BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_MSVC && _MSVC_LANG > __cplusplus
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

#if BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_CLANG || BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_GNU
#    define BALBINO_LIKELY(expr) __builtin_expect(!!(expr), 1)
#    define BALBINO_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#    define BALBINO_LIKELY(expr) (expr)
#    define BALBINO_UNLIKELY(expr) (expr)
#endif

#if defined(__has_include)
#if !defined(BALBINO_EMBED_HAS_INCLUDE)
#define BALBINO_EMBED_HAS_INCLUDE(HEADER_TOKEN) __has_include(HEADER_TOKEN)
#endif // undefined BALBINO_EMBED_HAS_INCLUDE
#endif // __has_include ability

#if defined(BALBINO_EMBED_HAS_INCLUDE) && (BALBINO_EMBED_HAS_INCLUDE(<version>))
#include <version>
#endif

#if !defined(BALBINO_EMBED_HAS_BUILTIN_EMBED)
#if defined(__has_builtin)
#if __has_builtin(__builtin_embed)
#define  BALBINO_EMBED_HAS_BUILTIN_EMBED 1
#else
#define  BALBINO_EMBED_HAS_BUILTIN_EMBED 0
#endif
#else
#define  BALBINO_EMBED_HAS_BUILTIN_EMBED 0
#endif // __has_builtin test
#endif // undefined BALBINO_EMBED_HAS_BUILTIN_EMBED