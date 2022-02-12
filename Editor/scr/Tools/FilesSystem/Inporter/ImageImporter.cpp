#include "pch.h"
#include "ImageImporter.h"

#include "CalcTangents.h"

#include <Common.h>
#include <FileParcer.h>
#include <MeshMetadata.h>
#include <UUID.h>
#include <Vertex.h>

#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <gli/gli.hpp>

bool BalEditor::ImportImage( const std::filesystem::path& path )
{
	std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
	p.replace_extension( ".basset" );
	std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
	if ( !file.is_open() )
		return false;

	std::string extenstion = path.extension().string();
	std::ranges::for_each( extenstion, []( char& c )
	{
		c = static_cast< char >( toupper( c ) );
	} );

	if ( extenstion == ".KTX" || extenstion == ".DDS" )
	{
		gli::texture_cube texture{ gli::load( path.string().c_str() ) };
		if ( texture.empty() )
			return false;

		BinaryReadWrite::Write( file, ( uint64_t ) CUuid() );
		BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ) );
		BinaryReadWrite::Write( file, texture.extent().x );
		BinaryReadWrite::Write( file, texture.extent().y );
		BinaryReadWrite::Write( file, texture.levels() );
		BinaryReadWrite::Write( file, texture.faces() );
		BinaryReadWrite::Write( file, texture.layers() );
		BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageLayout::Undefined ) );
		BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageViewType::Cube ) );
		BinaryReadWrite::Write( file, texture.format() );
		BinaryReadWrite::Write( file, texture.size() );
		BinaryReadWrite::Write( file, static_cast< uint8_t* >( texture.data() ), texture.size() );
	}
	else
	{
		SDL_Surface* pSurface{ IMG_Load( path.string().c_str() ) };
		if ( pSurface == nullptr )
		{
			return false;
		}

		if ( pSurface->pitch / pSurface->w != 4 )
			pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA8888, 0 );

		BinaryReadWrite::Write( file, ( uint64_t ) CUuid() );
		BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ) );
		BinaryReadWrite::Write( file, pSurface->w );
		BinaryReadWrite::Write( file, pSurface->h );
		BinaryReadWrite::Write( file, 1 );
		BinaryReadWrite::Write( file, 1 );
		BinaryReadWrite::Write( file, 1 );
		BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageLayout::Undefined ) );
		BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageViewType::View2D ) );
		BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EFormat::R8G8B8A8Srgb ) );
		BinaryReadWrite::Write( file, static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
		BinaryReadWrite::Write( file, static_cast< uint8_t* >( pSurface->pixels ), static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
		SDL_FreeSurface( pSurface );
	}
	file.close();
	return true;
}

bool BalEditor::ImportFont( const std::filesystem::path& path )
{
	TTF_Font* pFont = TTF_OpenFont( path.string().c_str(), 12 );
	if ( pFont == nullptr )
		return false;

	std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
	p.replace_extension( ".basset" );
	std::ofstream file( p.filename(), std::ios::out | std::ios::binary );

	if ( !file.is_open() )
		return false;
	TTF_SetFontKerning( pFont, 0 );
	BinaryReadWrite::Write( file, CUuid() );
	BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Font ) );
	BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ) );
	BinaryReadWrite::Write( file, TTF_GetFontHinting( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontHeight( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontAscent( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontDescent( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ) );

	for ( uint16_t i{}; i < 256u; ++i )
	{
		const char c[1]{ static_cast< char >( i ) };
		SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, c, { 255, 255, 255, 255 } );
		if ( pSurface == nullptr )
			return false;
		BinaryReadWrite::Write( file, pSurface->w );
		BinaryReadWrite::Write( file, pSurface->h );
		BinaryReadWrite::Write( file, pSurface->h * pSurface->pitch );
		BinaryReadWrite::Write( file, static_cast< uint8_t* >( pSurface->pixels ), static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
		TTF_CloseFont( pFont );
	}
	file.close();
	return true;
}

bool BalEditor::ImportAudio( const std::filesystem::path& path )
{
	Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str() );
	if ( pMusic == nullptr )
		return false;
	Mix_FreeMusic( pMusic );
	return true;
}

bool BalEditor::ImportMesh( const std::filesystem::path& path )
{
	std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
	p.replace_extension( ".basset" );
	std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
	if ( !file.is_open() )
		return false;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path.string(), static_cast< unsigned >( aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_ValidateDataStructure ) );

	// If the import failed, report it
	if ( !scene )
	{
		std::cerr << importer.GetErrorString();
		file.close();
		return false;
	}
	// Now we can access the file's contents.
	if ( !scene->HasMeshes() )
		throw std::runtime_error( "No mesh inside file" );

	const uint32_t nrOfMeshes{ scene->mNumMeshes };
	std::vector<BalVulkan::SVertex> vertices;
	std::vector<Balbino::SMeshMetadata> metadata( nrOfMeshes );
	std::vector<uint32_t> indices;
	for ( uint32_t i{}; i < nrOfMeshes; ++i )
	{
		const aiMesh* pAiMesh{ scene->mMeshes[i] };
		if ( !pAiMesh->HasTextureCoords( 0 ) )
			importer.ApplyPostProcessing( aiProcess_GenUVCoords );
		if ( !pAiMesh->HasNormals() )
			importer.ApplyPostProcessing( aiProcess_GenNormals | aiProcess_FixInfacingNormals );
		if ( !pAiMesh->HasTangentsAndBitangents() )
			importer.ApplyPostProcessing( aiProcess_CalcTangentSpace );
		importer.ApplyPostProcessing( aiProcess_JoinIdenticalVertices );

		metadata[i].indexCount = pAiMesh->mNumFaces * 3;
		metadata[i].indexStart = static_cast< uint32_t >( indices.size() );
		//metadata[i].materialId = i;
		const uint32_t prevSize{ static_cast< uint32_t >( vertices.size() ) };
		vertices.resize( prevSize + pAiMesh->mNumVertices, BalVulkan::SVertex{ { 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } } );
		indices.resize( metadata[i].indexStart + metadata[i].indexCount, 0 );
		for ( uint32_t j{}; j < pAiMesh->mNumVertices; ++j )
		{
			vertices[prevSize + j].position.x = pAiMesh->mVertices[j].x;
			vertices[prevSize + j].position.y = pAiMesh->mVertices[j].y;
			vertices[prevSize + j].position.z = pAiMesh->mVertices[j].z;

			if ( pAiMesh->HasVertexColors( 0 ) && pAiMesh->mColors[0] != nullptr )
			{
				vertices[prevSize + j].color.r = pAiMesh->mColors[0][j].r;
				vertices[prevSize + j].color.g = pAiMesh->mColors[0][j].g;
				vertices[prevSize + j].color.b = pAiMesh->mColors[0][j].b;
				vertices[prevSize + j].color.a = pAiMesh->mColors[0][j].a;
			}
			if ( pAiMesh->HasTextureCoords( 0 ) && pAiMesh->mTextureCoords[0] != nullptr )
			{
				vertices[prevSize + j].uv.x = pAiMesh->mTextureCoords[0][j].x;
				vertices[prevSize + j].uv.y = pAiMesh->mTextureCoords[0][j].y;
			}
			if ( pAiMesh->HasNormals() && pAiMesh->mNormals != nullptr )
			{
				vertices[prevSize + j].normal.x = pAiMesh->mNormals[j].x;
				vertices[prevSize + j].normal.y = pAiMesh->mNormals[j].y;
				vertices[prevSize + j].normal.z = pAiMesh->mNormals[j].z;
			}
			if ( pAiMesh->HasTangentsAndBitangents() && pAiMesh->mTangents != nullptr && pAiMesh->mBitangents != nullptr )
			{
				vertices[prevSize + j].tangent.x = pAiMesh->mTangents[j].x;
				vertices[prevSize + j].tangent.y = pAiMesh->mTangents[j].y;
				vertices[prevSize + j].tangent.z = pAiMesh->mTangents[j].z;
				vertices[prevSize + j].tangent.w = 1;
			}
		}
		for ( uint32_t j{}; j < pAiMesh->mNumFaces; ++j )
		{
			indices[metadata[i].indexStart + j * 3 + 0] = ( prevSize + pAiMesh->mFaces[j].mIndices[0] );
			indices[metadata[i].indexStart + j * 3 + 1] = ( prevSize + pAiMesh->mFaces[j].mIndices[1] );
			indices[metadata[i].indexStart + j * 3 + 2] = ( prevSize + pAiMesh->mFaces[j].mIndices[2] );
		}
	}
	CCalcTangents calcTangents{};
	STempMesh tempMesh{ vertices, indices };
	calcTangents.Calc( &tempMesh );
	BinaryReadWrite::Write( file, CUuid() );
	BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Model ) );
	BinaryReadWrite::Write( file, indices.size() );
	BinaryReadWrite::Write( file, vertices.size() );
	BinaryReadWrite::Write( file, metadata.size() );
	BinaryReadWrite::Write( file, indices.data(), indices.size() * sizeof( uint32_t ) );
	BinaryReadWrite::Write( file, vertices.data(), vertices.size() * sizeof( BalVulkan::SVertex ) );
	BinaryReadWrite::Write( file, metadata.data(), metadata.size() * sizeof( Balbino::SMeshMetadata ) );

	file.close();
	return true;
}
