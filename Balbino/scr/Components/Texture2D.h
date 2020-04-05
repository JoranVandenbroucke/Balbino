#pragma once
#include "Component.h"
#include "../Core.h"
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

struct SDL_Texture;
namespace Balbino
{
	class GameObject;

	class BALBINO_API Texture2D final: public Component
	{
	public:
		explicit Texture2D(const std::weak_ptr<GameObject> origine );
		virtual ~Texture2D();

		void SetTexture( const std::string path );
		void SetTexture( SDL_Texture* m_Texture );

		virtual void Create();
		virtual void Update() override;
		virtual void Draw() const override;

		SDL_Texture* GetSDLTexture() const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_Texture;
	};
}
#pragma warning(pop)