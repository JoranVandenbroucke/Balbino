#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <memory>
struct SDL_Texture;

namespace Balbino
{
	class Font;
	struct vertex;

	class ResourceManager final: public Singleton<ResourceManager>
	{
	public:
		void Init( const std::string& data );
		static GLuint LoadTexture( const std::string& file, vertex* vert = nullptr );
		static GLuint LoadTexture( const Font& font, const std::string& text, vertex* vert );
		static GLuint CreateFromSurface( SDL_Surface* pSurface );
		static Font* LoadFont( const std::string& file, unsigned int size );
		static Mix_Chunk* LoadAudio( const std::string& file );
		static void Cleanup();
		virtual ~ResourceManager();
	private:
		GLuint ILoadTexture( const std::string& file, vertex* vert );
		GLuint ILoadTexture( const Font& font, const std::string& text, vertex* vert );
		Font* ILoadFont( const std::string& file, unsigned int size );

		Mix_Chunk* ILoadAudio( const std::string& file );

		GLuint ICreateFromSurface( SDL_Surface* pSurface );

		void ICleanup();

		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::string m_DataPath;
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
		std::unordered_map<std::string, GLuint> m_Textures;
		std::unordered_map<std::pair<std::string, int>, Font*, comp> m_Fonts;

		std::unordered_map<std::string, Mix_Chunk*> m_Audio;
	};
}