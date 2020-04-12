#include "BalbinoPCH.h"
#include "Debug.h"

#ifdef _DEBUG
#include "Renderer/Renderer.h"
#include "ResourceManager.h"
#include "Components/Text.h" 

#include "imgui-1.75/imgui.h"
#include "imgui-1.75/imgui_sdl.h"

#include <cassert>
#include <SDL.h>

void Balbino::Debug::Assert( bool argument )
{
	assert( argument );
}

void Balbino::Debug::DrawLine( const vec3& start, const vec3& end, const Color& color )
{
#ifdef _DEBUG
	SDL_SetRenderDrawColor( Renderer::Get().GetSDLRenderer(), color.r, color.g, color.b, color.a );
	SDL_RenderDrawLine( Renderer::Get().GetSDLRenderer(), int( start.x ), int( start.y ), int( end.x ), int( end.y ) );
#endif //_DEBUG
}

void Balbino::Debug::DrawRay( const vec3& start, const vec3& direction, const Color& color )
{
#ifdef _DEBUG
	DrawLine( start, start + direction, color );
#endif //_DEBUG
}

void Balbino::Debug::Log( const std::string& log )
{
#ifdef _DEBUG
	Get().ILog( log );
#endif //_DEBUG
}

void Balbino::Debug::LogError( const std::string& error )
{
#ifdef _DEBUG
	Get().ILogError( error );
#endif //_DEBUG
}

void Balbino::Debug::LogException( const std::string& exeption )
{
#ifdef _DEBUG
	Get().ILogException( exeption );
#endif //_DEBUG
}

void Balbino::Debug::LogWarning( const std::string& warning )
{
#ifdef _DEBUG
	Get().ILogWarning( warning );
#endif //_DEBUG
}

void Balbino::Debug::Draw() const
{
	ImGui::Begin( "Log", nullptr, ImGuiWindowFlags_AlwaysAutoResize );
	for( const std::pair<std::string, Color>& text : m_Log )
	{
		ImGui::TextColored( ImVec4{ text.second.r / 255.f, text.second.g / 255.f, text.second.b / 255.f, text.second.a / 255.f }, text.first.c_str() );
	}
	ImGui::End();
}

Balbino::Debug::~Debug()
{
	m_Log.clear();
}

void Balbino::Debug::ILog( const std::string& log )
{
	m_Log.push_back( std::make_pair( log, Color{ 0xCFCFCF } ) );
	if( m_Log.size() > 20 )
	{
		m_Log.pop_front();
	}
}

void Balbino::Debug::ILogError( const std::string& error )
{
	m_Log.push_back( std::make_pair( error, Color{ 0xCF000 } ) );
	if( m_Log.size() > 20 )
	{
		m_Log.pop_front();
	}
}

void Balbino::Debug::ILogException( const std::string& exeption )
{
	Get().m_Log.push_back( std::make_pair( exeption, Color{ 0xCF00CF } ) );
	if( Get().m_Log.size() > 20 )
	{
		Get().m_Log.pop_front();
	}
}

void Balbino::Debug::ILogWarning( const std::string& warning )
{
	m_Log.push_back( std::make_pair( warning, Color{ 0xCFCF00 } ) );
	if( m_Log.size() > 20 )
	{
		m_Log.pop_front();
	}
}
#endif