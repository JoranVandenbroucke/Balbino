#include "BalbinoPCH.h"
#include "GameObject.h"
#include "../Components/All.h"
#include "../BinaryReaderWrider.h"
Balbino::GameObject::GameObject()
	: SceneObject{}
	, m_IsDestroyed{ false }
{
}

Balbino::GameObject::~GameObject()
{
	m_IsDestroyed = true;
	m_Name = "";
	m_Components.clear();
}

void Balbino::GameObject::Create()
{
	if( !GetComponent<Transform>() )
	{
		AddComponent<Transform>();
		if( m_Components.size() > 1 )
		{
			for( int i = int( m_Components.size() - 1 ); i > 0; --i )
			{
				std::swap( m_Components[i], m_Components[i - 1] );
			}
		}
	}

	for( Component* comp : m_Components )
	{
		comp->Create();
	}
}

void Balbino::GameObject::FixedUpdate()
{
	for( Component* comp : m_Components )
	{
		comp->FixedUpdate();
	}
}

void Balbino::GameObject::Update()
{
	for( Component* comp : m_Components )
	{
		comp->Update();
	}
}

void Balbino::GameObject::LateUpdate()
{
	for( Component* comp : m_Components )
	{
		comp->LateUpdate();
	}
}

void Balbino::GameObject::Draw() const
{
	for( Component* comp : m_Components )
	{
		comp->Draw();
	}
}

#ifdef _DEBUG
void Balbino::GameObject::DrawInspector()
{
	for( Component* comp : m_Components )
	{
		comp->DrawInpector();
	}
}
#endif // _DEBUG

void Balbino::GameObject::Destroy()
{
	m_Components.clear();
	m_IsDestroyed = true; 
}

bool Balbino::GameObject::IsDestroy() const
{
	return m_IsDestroyed;
}

void Balbino::GameObject::LoadComponents()
{
	for( Component* comp : m_Components )
	{
		comp->Create();
	}
}

void Balbino::GameObject::SetName( const char* newName )
{
	m_Name = newName;
}

const char* Balbino::GameObject::GetName() const
{
	return m_Name.c_str();
}

void Balbino::GameObject::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, m_Name );
	int size{ int( m_Components.size() ) };
	BinaryReadWrite::Write( file, size );
	for( int i = 0; i < size; i++ )
	{
		auto comp = m_Components[i];
		comp->Save( file );
	}
}

void Balbino::GameObject::Load( std::istream& file )
{
	int size{};
	BinaryReadWrite::Read( file, m_Name );
	BinaryReadWrite::Read( file, size );
	for( int i = 0; i < size; i++ )
	{
		int type{};
		BinaryReadWrite::Read( file, type );
		switch( ComponentList( type ) )
		{
		case Balbino::ComponentList::Audio:
			AddComponent<ConsoleAudio>()->Load( file );
			break;
		case Balbino::ComponentList::LoggedAudio:
			AddComponent<LoggedAudio>()->Load( file );
			break;
		case Balbino::ComponentList::Avatar:
			AddComponent<Avatar>()->Load( file );
			break;
		case Balbino::ComponentList::Camera:
			AddComponent<Camera>( .75f, 640.f );
			break;
		case Balbino::ComponentList::FPSScript:
			AddComponent<FPSScript>()->Load( file );
			break;
		case Balbino::ComponentList::Text:
			AddComponent<Text>()->Load( file );
			break;
		case Balbino::ComponentList::Sprite:
			AddComponent<Sprite>()->Load( file );
			break;
		case Balbino::ComponentList::Transform:
			AddComponent<Transform>()->Load( file );
			break;
		default:
			break;
		}
	}
	//std::reverse( m_Components.begin(), m_Components.end() );
}
