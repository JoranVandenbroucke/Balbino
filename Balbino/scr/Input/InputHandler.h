#pragma once
#include <map>

#include <functional>
//#pragma warning(push)
//#pragma warning(disable:4201)
//#pragma warning(disable:4324)
//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>
#include "InputButtons.h"

namespace Balbino
{
	struct CInputAxis;
	struct CInputAxisBinding;

	class CInputHandler final
	{
	public:
		CInputHandler();
		~CInputHandler() = default;
		CInputHandler( const CInputHandler& ) = delete;
		CInputHandler( CInputHandler&& ) = delete;
		CInputHandler& operator=( const CInputHandler& ) = delete;
		CInputHandler& operator=( CInputHandler&& ) = delete;

		bool Initialize();
		bool Cleanup();

		void ProcessEvents( SDL_Event e );
		void Update();
		void AddAxis( const char* pName, SDL_KeyCode code, float weight );

		void GetMousePosition( float& x, float& y ) const;
		void GetRelativeMousePosition( float& x, float& y ) const;

		template <class UserClass>
		bool BindAction( const char* axisName, EInputEvent inputType, UserClass* pObject, void (UserClass::* func)() );
		template <class UserClass>
		bool BindAxis( const char* axisName, UserClass* pObject, void (UserClass::* func)( float value ) );
		void ToggleCursor();
	private:
		std::multimap<std::string, CInputAxis*> m_inputsAxis;
		std::multimap<std::string, CInputAxisBinding*> m_inputsAxisBinding;
		glm::vec2 m_mousePosition;
		glm::vec2 m_mouseRelativePosition;
		bool m_visible;
	};
}

template <class UserClass>
bool Balbino::CInputHandler::BindAction( const char* axisName, EInputEvent inputType, UserClass* pObject,
                                         void (UserClass::* func)() )
{
	if ( !pObject )
		return false;
	std::function<void()> funct = std::bind( std::move( func ), pObject );

	return true;
}

template <class UserClass>
bool Balbino::CInputHandler::BindAxis( const char* axisName, UserClass* pObject, void (UserClass::*func)( float value ) )
{
	if ( !pObject )
		return false;
	std::function<void( float )> funct = std::bind( std::move( func ), pObject, std::placeholders::_1 );
	m_inputsAxisBinding.emplace( axisName, DBG_NEW CInputAxisBinding{ .value = 0, .function = funct } );

	return true;
}
//#pragma warning(pop)
