#include "InputHandler.h"

Balbino::CInputHandler::CInputHandler()
        : m_inputsAxis{}
          , m_inputsAxisBinding{}
          , m_mousePosition{}
          , m_mouseRelativePosition{}
          , m_visible{ false }
{
}

bool Balbino::CInputHandler::Initialize()
{
    //ToggleCursor();
    return true;
}

bool Balbino::CInputHandler::Cleanup()
{
    for ( const auto& var : m_inputsAxis )
    {
        delete var.second;
    }
    for ( const auto& var : m_inputsAxisBinding )
    {
        delete var.second;
    }
    m_inputsAxis.clear();
    m_inputsAxisBinding.clear();
    return true;
}

void Balbino::CInputHandler::ProcessEvents( SDL_Event e )
{
    switch ( e.type )
    {
        case SDL_EVENT_KEY_DOWN:
            for ( const auto& axis : m_inputsAxis )
            {
                if ( axis.second->code == e.key.keysym.sym && e.key.repeat == 0 )
                {
                    auto       it{ m_inputsAxisBinding.equal_range( axis.first ) };
                    for ( auto iter{ it.first }; iter != it.second; ++iter )
                    {
                        iter->second->value += axis.second->weight;
                    }
                }
            }
            //if (e.key.keysym.sym == SDLK_ESCAPE)
            //	ToggleCursor();
            break;
        case SDL_EVENT_KEY_UP:
            for ( const auto& axis : m_inputsAxis )
            {
                if ( axis.second->code == e.key.keysym.sym )
                {
                    auto       it{ m_inputsAxisBinding.equal_range( axis.first ) };
                    for ( auto iter{ it.first }; iter != it.second; ++iter )
                    {
                        iter->second->value -= axis.second->weight;
                    }
                }
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            m_mousePosition.x         = static_cast<float>( e.motion.x );
            m_mousePosition.y         = static_cast<float>( e.motion.y );
            m_mouseRelativePosition.x = static_cast<float>( e.motion.xrel );
            m_mouseRelativePosition.y = static_cast<float>( e.motion.yrel );
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_WHEEL:
        case SDL_MOUSEWHEEL_FLIPPED:
        case SDL_MOUSEWHEEL_NORMAL:
        case SDL_EVENT_JOYSTICK_AXIS_MOTION:
        case SDL_EVENT_JOYSTICK_HAT_MOTION:
        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        case SDL_EVENT_JOYSTICK_BUTTON_UP:
        case SDL_EVENT_JOYSTICK_ADDED:
        case SDL_EVENT_JOYSTICK_REMOVED:
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMAPPED:
        case SDL_EVENT_GAMEPAD_REMOVED:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
        case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_UP:
        case SDL_EVENT_FINGER_MOTION:
        default:
            break;
    }
}

void Balbino::CInputHandler::Update()
{
    for ( const auto& axis : m_inputsAxis )
    {
        auto       it{ m_inputsAxisBinding.equal_range( axis.first ) };
        for ( auto iter{ it.first }; iter != it.second; ++iter )
        {
            iter->second->function( iter->second->value );
        }
    }
    m_mouseRelativePosition.x = m_mouseRelativePosition.y = 0;
}

void Balbino::CInputHandler::AddAxis( const char* pName, SDL_KeyCode code, float weight )
{
    m_inputsAxis.emplace(
            pName, new CInputAxis{
                    .name = SDL_GetKeyName( code ), .code = static_cast<uint16_t>( code ), .weight = weight,
            }
    );
}

void Balbino::CInputHandler::GetMousePosition( float& x, float& y ) const
{
    x = m_mousePosition.x;
    y = m_mousePosition.y;
}

void Balbino::CInputHandler::GetRelativeMousePosition( float& x, float& y ) const
{
    x = m_mouseRelativePosition.x;
    y = m_mouseRelativePosition.y;
}

void Balbino::CInputHandler::ToggleCursor()
{
    if ( SDL_SetRelativeMouseMode( m_visible ? SDL_TRUE : SDL_FALSE ) != 0 )
    {
        throw std::runtime_error( std::string( "could not enable Relative Mouse Mode: " ) + SDL_GetError());
    }
    m_visible = !m_visible;
}
