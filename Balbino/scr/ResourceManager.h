#pragma once
#include "Singleton.h"
#include "Core.h"
#include <string>
#include <memory>
#pragma warning(push)
#pragma warning(disable:4251)
//#pragma warning(disable:2039)

namespace Balbino
{
	class Texture2D;
	class Font;
	class BALBINO_API ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}
#pragma warning(pop)