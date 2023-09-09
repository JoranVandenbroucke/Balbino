#pragma once
#include <Core.h>
#include <SDL3/SDL.h>
#include <functional>
#include <map>
#include <string>

#include "BambiMath.h"
#include "InputButtons.h"

namespace BalbinoControl
{
    struct SInputAxis;
    struct SInputAxisBinding;

    class CInputHandler final
    {
    public:
        CInputHandler()                                  = default;
        ~CInputHandler()                                 = default;
        CInputHandler( const CInputHandler& )            = delete;
        CInputHandler( CInputHandler&& )                 = delete;
        CInputHandler& operator=( const CInputHandler& ) = delete;
        CInputHandler& operator=( CInputHandler&& )      = delete;

        bool Initialize();
        bool Cleanup();

        void ProcessEvents( const SDL_Event& e ) noexcept;
        void Update() noexcept;
        void AddAxis( const char* pName, SDL_KeyCode code, float weight ) noexcept;

        void GetMousePosition( float& x, float& y ) const noexcept;
        void GetRelativeMousePosition( float& x, float& y ) const noexcept;

        bool AddAxis( const char* axisName, input_event inputType, float weight ) noexcept;
        template<class UserClass>
        bool BindAxis( const char* axisName, UserClass* pObject, std::function<void( float value )> UserClass::*func ) noexcept;
        void ToggleCursor();

    private:
        std::multimap<std::string, SInputAxis, std::less<>> m_inputsAxis;
        std::multimap<std::string, SInputAxisBinding, std::less<>> m_inputsAxisBinding;

        BambiMath::Vector2 m_mousePosition {};
        BambiMath::Vector2 m_mouseRelativePosition {};

        bool m_visible {};
    };
}// namespace BalbinoControl

