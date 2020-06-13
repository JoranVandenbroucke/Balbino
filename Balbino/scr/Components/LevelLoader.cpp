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
	, m_OverSave{ false }
{
}

void Balbino::LevelLoader::Create()
{
	if( m_Created ) return; this->Component::Create();
	if( !m_OverSave )
	{
		for( int y = 0; y < 25; y++ )
		{
			for( int x = 0; x < 32; x++ )
			{
				GameObject* newTile = SceneManager::AddGameObjectToScene();
				auto image = newTile->AddComponent<Sprite>();
				auto collider = newTile->AddComponent<BoxCollider2D>();
				newTile->Create();
				newTile->SetName( ( std::string( "Tile " ) + std::to_string( x + ( y * 32 ) ) ).c_str() );
				image->SetTexture( "Sprites/blocks.png" );
				image->LoadUV();
				image->SetSpriteIndex( 0 );
				auto transform = newTile->GetComponent<Transform>();
				transform->SetPosition( x * 8.f, y * 8.f, 0 );
				transform->SetParrent( m_pTransform );
				collider->Reset();
			}
		}
	}
	SetLevelNr( m_LevelNr );
}

void Balbino::LevelLoader::Update()
{
}

void Balbino::LevelLoader::Draw() const
{
}

void Balbino::LevelLoader::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::LevelLoader ) );
	BinaryReadWrite::Write( file, m_LevelNr );
}

void Balbino::LevelLoader::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_LevelNr );
	m_OverSave = true;
	//SetLevelNr( m_LevelNr );
}

void Balbino::LevelLoader::SetLevelNr( int levelNr )
{
	std::ifstream level{ "../Data/LevelData/leveldata.dat", std::ios::in | std::ios::binary };
	level.seekg( 0, std::ios::beg );
	level.seekg( ( levelNr - 1 ) * 100, std::ios::beg );

	m_LevelNr = levelNr;
	int nrOfTiles = 100;
	unsigned char tyle{};

	//m_pTransform->DestroyChilderen();
	const auto childeren = m_pTransform->GetChildren();
	for( int j = 0; j < nrOfTiles; ++j )
	{
		BinaryReadWrite::Read( level, tyle );
		for( int i = 0; i < 8; i++ )
		{

			if( ( tyle & ( 1 << ( 7 - i ) ) ) >> ( 7 - i ) )
			{
				childeren[ i + ( j * 8 )]->GetGameObject()->SetActive(true);
				childeren[ i + ( j * 8 )]->GetComponent<Sprite>()->SetSpriteIndex( levelNr - 1 );
			}
			else
			{
				childeren[i + ( j * 8 )]->GetGameObject()->SetActive( false );
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
