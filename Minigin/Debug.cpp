#include "MiniginPCH.h"
#include "Debug.h"
#include <cassert>
#include <SDL.h>
#include "Renderer.h"
#include "ResourceManager.h"
#include "Text.h"

void Balbino::Debug::Assert( bool argument )
{
	assert( argument );
}

void Balbino::Debug::DrawLine( const glm::vec3& start, const glm::vec3& end, const Color& color )
{
#ifdef _DEBUG
	SDL_SetRenderDrawColor( Renderer::GetInstance().GetSDLRenderer(), color.r, color.g, color.b, color.a );
	SDL_RenderDrawLine( Renderer::GetInstance().GetSDLRenderer(), int( start.x ), int( start.y ), int( end.x ), int( end.y ) );
#endif //_DEBUG
}

void Balbino::Debug::DrawRay( const glm::vec3& start, const glm::vec3& direction, const Color& color )
{
#ifdef _DEBUG
	DrawLine( start, start + direction, color );
#endif //_DEBUG
}

void Balbino::Debug::Log( const std::string& log )
{
#ifdef _DEBUG
	GetInstance().ILog( log );
#endif //_DEBUG
}

void Balbino::Debug::LogError( const std::string& error )
{
#ifdef _DEBUG
	GetInstance().ILogError( error );
#endif //_DEBUG
}

void Balbino::Debug::LogException( const std::string& exeption )
{
#ifdef _DEBUG
	GetInstance().ILogException( exeption );
#endif //_DEBUG
}

void Balbino::Debug::LogWarning( const std::string& warning )
{
#ifdef _DEBUG
	GetInstance().ILogWarning( warning );
#endif //_DEBUG
}

void Balbino::Debug::ILog( const std::string& log )
{
	auto font = ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 );
	std::shared_ptr<Text> text = std::make_shared<Text>( log, font );
	text->SetColor( Color{ 0xCFCFCF } );
	GetInstance().m_Log.push_back( text );
	if( GetInstance().m_Log.size() < 20 )
	{
		GetInstance().m_Log.pop_front();
	}
}

void Balbino::Debug::ILogError( const std::string& error )
{
	auto font = ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 );
	std::shared_ptr<Text> text = std::make_shared<Text>( error, font );
	text->SetColor( Color{ 0xCF000 } );
	GetInstance().m_Log.push_back( text );
	if( GetInstance().m_Log.size() < 20 )
	{
		GetInstance().m_Log.pop_front();
	}
}

void Balbino::Debug::ILogException( const std::string& exeption )
{
	auto font = ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 );
	std::shared_ptr<Text> text = std::make_shared<Text>( exeption, font );
	text->SetColor( Color{ 0xCF00CF } );
	GetInstance().m_Log.push_back( text );
	if( GetInstance().m_Log.size() < 20 )
	{
		GetInstance().m_Log.pop_front();
	}
}

void Balbino::Debug::ILogWarning( const std::string& warning )
{
	auto font = ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 );
	std::shared_ptr<Text> text = std::make_shared<Text>( warning, font );
	text->SetColor( Color{ 0xCFCF00 } );
	GetInstance().m_Log.push_back( text );
	if( GetInstance().m_Log.size() < 20 )
	{
		GetInstance().m_Log.pop_front();
	}
}