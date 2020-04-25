#include "BalbinoPCH.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "../BinaryReaderWrider.h"
#include "../GameObject/GameObject.h"
Balbino::LevelLoader::LevelLoader( std::weak_ptr<GameObject> origine )
	:Component{ origine }
	, m_LevelNr{}
{
}

void Balbino::LevelLoader::Create()
{
	this->Component::Create();
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
	m_LevelNr = levelNr;
	//int nrOfTiles = 32 * 25 * ( 1 + levelNr );
	bool tyle{};
	for( int y = 0; y < 25; y++ )
	{
		for( int x = 0; x < 32; x++ )
		{
			BinaryReadWrite::Read( level, tyle );
			if( tyle )
			{
				std::shared_ptr<GameObject> newTile = std::make_shared<GameObject>();
				auto image = newTile->AddComponent<Sprite>();
				//image->SetSprite()
			}
		}
	}
}
