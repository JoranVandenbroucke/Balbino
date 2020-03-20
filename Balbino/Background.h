#pragma once
#include "Transform.h"
#include "SceneObject.h"

namespace Balbino
{
	class Texture2D;
	class Background : public SceneObject
	{
	public:
		void Create() override;
		void Update() override;
		void Draw() const override;

		Background() = default;
		virtual ~Background();
		Background(const Background& other) = delete;
		Background(Background&& other) = delete;
		Background& operator=(const Background& other) = delete;
		Background& operator=(Background&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture{};

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
	};
}
