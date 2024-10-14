//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "compiler.hpp"

#include <string_view>

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
BALBINO_CONSTEXPR_SINCE_CXX17 auto operator|(const TEnumType a_Lhs, const TEnumType a_Rhs) noexcept -> TEnumType
    requires EnumFlagType<TEnumType>
{
    using UnderlyingType = std::underlying_type_t<TEnumType>;
    return static_cast<TEnumType>(static_cast<UnderlyingType>(a_Lhs) | static_cast<UnderlyingType>(a_Rhs));
}

template <EnumType TEnumType>
BALBINO_CONSTEXPR_SINCE_CXX17 auto operator&(const TEnumType a_Lhs, const TEnumType a_Rhs) noexcept -> TEnumType
    requires EnumFlagType<TEnumType>
{
    using UnderlyingType = std::underlying_type_t<TEnumType>;
    return static_cast<TEnumType>(static_cast<UnderlyingType>(a_Lhs) & static_cast<UnderlyingType>(a_Rhs));
}

template <EnumType TEnumType>
BALBINO_CONSTEXPR_SINCE_CXX17 auto operator^(const TEnumType a_Lhs, const TEnumType a_Rhs) noexcept -> TEnumType
    requires EnumFlagType<TEnumType>
{
    using UnderlyingType = std::underlying_type_t<TEnumType>;
    return static_cast<TEnumType>(static_cast<UnderlyingType>(a_Lhs) ^ static_cast<UnderlyingType>(a_Rhs));
}

#define ENABLE_ENUM_BITWISE_OPERATIONS(enumClassFlagType)                                                                                                                                                                                      \
    template <>                                                                                                                                                                                                                                \
    struct SIsEnumFlag<enumClassFlagType> : std::true_type                                                                                                                                                                                     \
    {                                                                                                                                                                                                                                          \
    };
