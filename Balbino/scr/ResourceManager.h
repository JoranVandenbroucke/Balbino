#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <memory>

struct SDL_Texture;

namespace Balbino
{
	class Font;
	struct comp
	{
		template<typename T1, typename T2>
		bool operator()( const std::pair<T1, T2>& p ) const
		{
			auto h1 = std::hash<T1>{}( p.first );
			auto h2 = std::hash<T2>{}( p.second );
			return h1 ^ h2;
		}
	};

	class ResourceManager final: public Singleton<ResourceManager>
	{
	public:
		void Init( const std::string& data );
		static SDL_Texture* LoadTexture( const std::string& file );
		static std::shared_ptr<Font> LoadFont( const std::string& file, unsigned int size );
		static std::shared_ptr<Mix_Chunk> LoadAudio( const std::string& file );
	private:
		SDL_Texture* ILoadTexture( const std::string& file );
		std::shared_ptr<Font> ILoadFont( const std::string& file, unsigned int size );
		friend class Singleton<ResourceManager>;
		std::shared_ptr<Mix_Chunk> ILoadAudio( const std::string& file );

		ResourceManager() = default;
		std::string m_DataPath;

		std::unordered_map<std::string, SDL_Texture*> m_Textures;
		std::unordered_map<std::pair<std::string, int>, std::shared_ptr<Font>, comp> m_Fonts;
		std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> m_Audio;
	};
}