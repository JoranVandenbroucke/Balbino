#pragma once
#include "Singleton.h"
#include <string>
#include <memory>

struct SDL_Texture;

namespace Balbino
{
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		static SDL_Texture* LoadTexture(const std::string& file);
		static std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size);
	private:
		SDL_Texture* ILoadTexture(const std::string& file) const;
		std::shared_ptr<Font> ILoadFont(const std::string& file, unsigned int size) const;
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}