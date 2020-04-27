#pragma once
#include "../Singleton.h"
#include "File.h"
#include <filesystem>

namespace Balbino
{
	class AssetBrouser final : public Singleton<AssetBrouser>
	{
	public:
		void Draw();

	private:
		friend class Singleton<AssetBrouser>;
		AssetBrouser();

		std::vector<file> GetFilesInPath( const std::filesystem::path& path );

		//std::string m_Current;
		const std::string m_GameStart = std::filesystem::current_path().generic_u8string();
		void MoveIn( const std::filesystem::path& path, std::vector<file>& selectedPath );

		const std::vector<std::string> m_SuportedImageFormat = { ".BMP", ".GIF", ".JPEG", ".JPG", ".LBM", ".PCX", ".PNG", ".PNM", ".SVG", ".TGA", ".TIFF", ".WEBP", ".XCF", ".XPM", ".XV" };
		const std::vector<std::string> m_SuportedAudioFormat = { ".OGG", ".MP3", ".WAV", ".MID", ".FLAC", ".MOD"/*, ".OPUS"*/ };
		const std::vector<std::string> m_SuportedFontFormat = { ".TTF", ".TTC", ".CFF", ".WOFF", ".OTF", ".OTC", ".PFB", ".PFA", ".CID", ".SFNT", ".PCF", ".FNT", ".BDF", ".PFR" };
		const GLuint m_BalbinoTexture, m_AudioTexture, m_CppChTexture, m_UnknownTexture, m_FolderTexture;
	};
}
