#include "BalbinoPCH.h"
#include "Debug.h"

#ifdef BALBINO_DEBUG
#include "..\Renderer/Renderer.h"
#include "..\ResourceManager.h"
#include "..\Components/Text.h" 

#include <cassert>

Balbino::VertexBuffer* Balbino::Debug::m_pVertexBuff{ nullptr };
Balbino::Shader* Balbino::Debug::m_pShader{ nullptr };
Balbino::Debug::Debug()
{
	m_pVertexBuff = new VertexBuffer{ nullptr, 2 };
	m_pShader = new Shader{ "Shaders/Line.vert", "Shaders/Line.frag" };
}
void Balbino::Debug::Assert( bool argument )
{
	assert( argument );
}

void Balbino::Debug::DrawLine( const Vector3& start, const Vector3& end, const Color& color )
{
#ifdef BALBINO_DEBUG
	( void )color;
	vertex vert[2]{};
	vert[0].x = start.x;
	vert[0].y = start.y;
	vert[0].z = start.z;
	vert[1].x = end.x;
	vert[1].y = end.y;
	vert[1].z = end.z;
	m_pVertexBuff->Update( vert, 2 );
	m_pShader->Bind();
	glUniform4f( glGetUniformLocation( m_pShader->GetID(), "u_Color" ), color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f );
	m_pVertexBuff->Bind();
	glDrawArrays( GL_LINES, 0, 2 );
	m_pVertexBuff->Unbind();
#endif //BALBINO_DEBUG
}

void Balbino::Debug::DrawRay( const Vector3& start, const Vector3& direction, const Color& color )
{
#ifdef BALBINO_DEBUG
	DrawLine( start, start + direction, color );
#endif //BALBINO_DEBUG
}

void Balbino::Debug::Log( const std::string& log )
{
#ifdef BALBINO_DEBUG
	Get().ILog( log );
#endif //BALBINO_DEBUG
}

void Balbino::Debug::LogError( const std::string& error )
{
#ifdef BALBINO_DEBUG
	Get().ILogError( error );
#endif //BALBINO_DEBUG
}

void Balbino::Debug::LogException( const std::string& exeption )
{
#ifdef BALBINO_DEBUG
	Get().ILogException( exeption );
#endif //BALBINO_DEBUG
}

void Balbino::Debug::LogWarning( const std::string& warning )
{
#ifdef BALBINO_DEBUG
	Get().ILogWarning( warning );
#endif //BALBINO_DEBUG
}

void Balbino::Debug::Draw() const
{
	ImGui::Begin( "Log" );
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