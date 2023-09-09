#include "InputHandler.h"
#include <stdexcept>

bool BalbinoControl::CInputHandler::Initialize()
{
#pragma todo("Read/Initialize input files") // todo: Read/Initialize input files
    return true;
}

bool BalbinoControl::CInputHandler::Cleanup()
{
    m_inputsAxis.clear();
    m_inputsAxisBinding.clear();
    return true;
}

void BalbinoControl::CInputHandler::ProcessEvents( const SDL_Event& e ) noexcept
{
    switch ( e.type )
    {
        case SDL_EVENT_KEY_DOWN:
            for ( const auto& [ name, axis ] : m_inputsAxis )
            {
                if ( axis.code != e.key.keysym.sym )
                {
                    continue;
                }
                const auto& [ firstBinding, lastBinding ] { m_inputsAxisBinding.equal_range( name ) };
                for ( auto binding { firstBinding }; binding != lastBinding; ++binding )
                {
                    binding->second.value += axis.weight;
                }
            }
            break;
        case SDL_EVENT_KEY_UP:
            for ( const auto& [ name, axis ] : m_inputsAxis )
            {
                if ( axis.code != e.key.keysym.sym )
                {
                    continue;
                }
                const auto& [ firstBinding, lastBinding ] { m_inputsAxisBinding.equal_range( name ) };
                for ( auto binding { firstBinding }; binding != lastBinding; ++binding )
                {
                    binding->second.value -= axis.weight;
                }
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            m_mousePosition.x         = e.motion.x;
            m_mousePosition.y         = e.motion.y;
            m_mouseRelativePosition.x = e.motion.xrel;
            m_mouseRelativePosition.y = e.motion.yrel;
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
        case SDL_EVENT_FINGER_MOTION: break;
    }
}

void BalbinoControl::CInputHandler::Update() noexcept
{
    for ( const auto& [ name, axis ] : m_inputsAxis )
    {
        const auto& [ firstBinding, lastBinding ] { m_inputsAxisBinding.equal_range( name ) };
        for ( auto binding { firstBinding }; binding != lastBinding; ++binding )
        {
            binding->second.function( binding->second.value );
        }
    }
    m_mouseRelativePosition.x = m_mouseRelativePosition.y = 0;
}

void BalbinoControl::CInputHandler::AddAxis( const char* pName, SDL_KeyCode code, float weight ) noexcept
{
    m_inputsAxis.emplace( pName,
                          SInputAxis {
                              .name   = SDL_GetKeyName( code ),
                              .code   = static_cast<uint16_t>( code ),
                              .weight = weight,
                          } );
}

bool BalbinoControl::CInputHandler::AddAxis( const char* axisName, input_event inputType, float weight ) noexcept
{
    m_inputsAxis.emplace( axisName, SInputAxis { axisName, inputType, weight } );
    return true;
}

void BalbinoControl::CInputHandler::GetMousePosition( float& x, float& y ) const noexcept
{
    x = m_mousePosition.x;
    y = m_mousePosition.y;
}

void BalbinoControl::CInputHandler::GetRelativeMousePosition( float& x, float& y ) const noexcept
{
    x = m_mouseRelativePosition.x;
    y = m_mouseRelativePosition.y;
}

void BalbinoControl::CInputHandler::ToggleCursor()
{
    if ( SDL_SetRelativeMouseMode( m_visible ? SDL_TRUE : SDL_FALSE ) != 0 )
    {
        throw std::invalid_argument( std::string( "could not enable Relative Mouse Mode: " ) + SDL_GetError() );
    }
    m_visible = !m_visible;
}

template<class UserClass>
bool BalbinoControl::CInputHandler::BindAxis( const char* axisName, UserClass* pObject, std::function<void( float value )> UserClass::*func ) noexcept
{
    if ( !pObject )
    {
        return false;
    }
    std::function<void( float )> funct = std::bind_front( std::move( func ), pObject, std::placeholders::_1 );
    m_inputsAxisBinding.emplace( axisName, SInputAxisBinding { .value = 0, .function = funct } );

    return true;
}
