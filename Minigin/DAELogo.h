#pragma once
#include "GameObject.h"
#include <string>
namespace Balbino
{
	class DAELogo: public SceneObject
	{
	public:
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		DAELogo();
		virtual ~DAELogo() = default;
		DAELogo( const DAELogo& other ) = delete;
		DAELogo( DAELogo&& other ) = delete;
		DAELogo& operator=( const DAELogo& other ) = delete;
		DAELogo& operator=( DAELogo&& other ) = delete;
	protected:
		std::shared_ptr<Texture2D> m_Texture{};

		void SetTexture( const std::string& filename );
		void SetPosition( float x, float y );
	};
}