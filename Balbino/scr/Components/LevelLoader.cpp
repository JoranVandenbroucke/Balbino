#include "BalbinoPCH.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "Transform.h"
#include "../BinaryReaderWrider.h"
#include "../GameObject/GameObject.h"
#include "../SceneManager.h"
#include "BoxCollider2D.h"

Balbino::LevelLoader::LevelLoader( const GameObject* const origine )

	:Component{ origine }
	, m_LevelNr{ 1 }
{
}

void Balbino::LevelLoader::Create()
{
	this->Component::Create();
	SetLevelNr( m_LevelNr );
	m_pTransform->SetScale( 2.f, 2.f, 1 );
}

void Balbino::LevelLoader::Update()
{
}

void Balbino::LevelLoader::Draw() const
{
}

void Balbino::LevelLoader::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, m_LevelNr );
}

void Balbino::LevelLoader::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_LevelNr );
	SetLevelNr( m_LevelNr );
}

void Balbino::LevelLoader::SetLevelNr( int levelNr )
{
	std::ifstream level{ "../Data/LevelData/leveldata.dat", std::ios::in | std::ios::binary };
	level.seekg( 0, std::ios::beg );
	level.seekg( ( levelNr - 1 ) * 100, std::ios::beg );

	m_LevelNr = levelNr;
	int nrOfTiles = 100;
	unsigned char tyle{};

	float x{}, y{};
	m_pTransform->DestroyChilderen();
	for( int j = 0; j < nrOfTiles; ++j )
	{
		BinaryReadWrite::Read( level, tyle );
		for( int i = 0; i < 8; i++ )
		{
			if( ( tyle & ( 1 << ( 7 - i ) ) ) >> ( 7 - i ) )
			{
				GameObject* newTile = SceneManager::AddGameObjectToScene();
				auto image = newTile->AddComponent<Sprite>();
				auto collider = newTile->AddComponent<BoxCollider2D>();
				newTile->Create();
				newTile->SetName( ( std::string( "Tile " ) + std::to_string( i + ( j * 8 ) ) ).c_str() );
				image->SetTexture( "Sprites/blocks.png" );
				image->LoadUV();
				image->SetSpriteIndex( levelNr - 1 );

				auto transform = newTile->GetComponent<Transform>();

				{
					x = 8.f * ( ( i + ( j * 8 ) ) % 32 );
					y = 8.f * ( ( i + ( j * 8 ) ) / 32 );
				}
				transform->SetPosition( x, y, 0 );
				transform->SetParrent( m_pTransform );
				collider->Reset();
			}
		}
	}
}

void Balbino::LevelLoader::DrawInpector()
{
	int level = m_LevelNr;
	bool valueChainged{};
	ImGui::BeginChild( "Level Loader Component", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Level Nr." );
	ImGui::SameLine();
	valueChainged = ImGui::SliderInt( "##LevelSelector", &level, 1, 100 );
	if( valueChainged )
	{
		SetLevelNr( level );
	}
	ImGui::EndChild();
}
