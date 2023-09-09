#pragma once
#include <functional>

namespace BalbinoControl
{
    enum input_event : uint8_t
    {
        input_event_down        = 0,
        input_event_up          = 1,
        input_event_repeat      = 2,
        input_event_doubleclick = 3,
        input_event_axis        = 4,
    };

    struct SInputAxis final {
        const char* name;
        uint16_t code;
        float weight;
    };

    struct SInputAxisBinding final {
        float value;
        std::function<void( float )> function;
    };
}// namespace Balbino
