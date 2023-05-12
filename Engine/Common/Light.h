//
// Created by joran on 17/06/2022.
//

#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H

enum light_type : uint8_t
{
    light_type_directional,
    light_type_point,
    light_type_spot,
    light_type_area,
    light_type_max
};

#endif //GAME_LIGHT_H
