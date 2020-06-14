#include "BalbinoPCH.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "Transform.h"
#include "Enemy.h"
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
	if( m_Created ) return;
	this->Component::Create();
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
				transform->SetPosition( x * 8.f, (24-y) * 8.f, 0 );
				transform->SetParrent( m_pTransform );
				collider->Reset();
				if( x == 0 || x == 1 || x == 30 || x == 31 || y == 0 )
				{
					newTile->SetTag( "Wall" );
				}
				else
				{
					newTile->SetTag( "Platform" );
				}
			}
		}
		for( int i = 0; i < m_MaxEnemeis; i++ )
		{
			GameObject* enemyObject = SceneManager::AddGameObjectToScene();
			m_pEnemys[i] = enemyObject->AddComponent<Enemy>();
			enemyObject->Create();
			enemyObject->SetName( ( std::string( "Enemy " ) + std::to_string( i ) ).c_str() );
		}
	}
	else
	{
		for( int i = 0; i < m_MaxEnemeis; i++ )
		{
			GameObject* enemyObject = SceneManager::GetGameObjectByName( std::string( "Enemy " ) + std::to_string( i ) );
			m_pEnemys[i] = enemyObject->GetComponent<Enemy>();
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
	for( int i = 0; i < m_MaxEnemeis; i++ )
	{
		m_pEnemys[i]->GetGameObject()->SetActive( false );
	}
	std::ifstream level{ "../Data/LevelData/leveldata.dat", std::ios::in | std::ios::binary };
	level.seekg( 0, std::ios::beg );
	level.seekg( ( levelNr - 1 ) * 100, std::ios::beg );

	m_LevelNr = levelNr;
	int nrOfTiles = 100;
	unsigned char data{};

	const auto childeren = m_pTransform->GetChildren();
	for( int j = 0; j < nrOfTiles; ++j )
	{
		BinaryReadWrite::Read( level, data );
		for( int i = 0; i < 8; i++ )
		{
			if( ( data & ( 1 << ( 7 - i ) ) ) >> ( 7 - i ) )
			{
				childeren[i + ( j * 8 )]->GetGameObject()->SetActive( true );
				childeren[i + ( j * 8 )]->GetComponent<Sprite>()->SetSpriteIndex( levelNr - 1 );
			}
			else
			{
				childeren[i + ( j * 8 )]->GetGameObject()->SetActive( false );
			}
		}
	}
	level.seekg( 0, std::ios::beg );
	level.seekg( 0x271a, std::ios::beg );
	for( int i = 0; i < levelNr - 1; i++ )
	{
		do
		{
			BinaryReadWrite::Read( level, data );
		}
		while( data != 0b00000000 );
	}
	char enemyNr{ 0 };
	while( true )
	{
		BinaryReadWrite::Read( level, data );
		if( data == 0b00000000 ) break;
		const unsigned char enemyMask{ 0b00000111 };
		const unsigned char enemyType{ (unsigned char) ( data & enemyMask ) };
		const unsigned char collumMask{ 0b11111000 };
		const unsigned char collum{ (unsigned char) ( ( data & collumMask ) >> 3 ) };
		BinaryReadWrite::Read( level, data );
		const unsigned char rowMask{ 0b11111000 };
		const unsigned char row{ (unsigned char) ( ( data & rowMask ) >> 3 ) };
		/*
		Byte 2 Mask 0b00000100 = Unknown Bool 1
		Byte 2 Mask 0b00000010 = Unknown Bool 2
		Byte 2 Mask 0b00000001 = Unknown Bool 3
		*/
		BinaryReadWrite::Read( level, data );
		const unsigned char spawnDelayMask{ 0b00011111 };
		const unsigned char delay{ (unsigned char) ( ( data & spawnDelayMask ) << 1 ) };
		float delayTime = delay * 0.017f;
		/*
		Byte 3 Mask 0b10000000 = Unknown Bool 4
		Byte 3 Mask 0b01000000 = Unknown Bool 5
		Byte 3 Mask 0b00100000 = Unknown Bool 6
		*/
		m_pEnemys[enemyNr]->SetType( Enemy::EnemyType( enemyType ) );
		m_pEnemys[enemyNr]->SetPositon( collum, row );
		m_pEnemys[enemyNr]->SetAnimationOffset( delay );
		m_pEnemys[enemyNr]->SetSpawnDelay( delayTime );
		m_pEnemys[enemyNr]->GetGameObject()->SetActive( true );
		++enemyNr;
		if( enemyNr == 6 )
		{
			break;
		}
	}
}
#ifdef BALBINO_DEBUG
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
#endif // BALBINO_DEBUG

