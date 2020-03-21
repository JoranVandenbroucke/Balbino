#pragma once
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)
struct SDL_Texture;
namespace Balbino
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class BALBINO_API Texture2D
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();

		void Draw() const;
		void Draw(float x, float y) const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_Texture;
	};
}
#pragma warning(pop)