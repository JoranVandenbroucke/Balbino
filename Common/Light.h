//
// Created by joran on 17/06/2022.
//

#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H

namespace Balbino
{
    enum class ELightType : uint8_t
    {
        Directional,
        Point,
        Spot,
        Area,
        Max
    };
}

#endif //GAME_LIGHT_H
