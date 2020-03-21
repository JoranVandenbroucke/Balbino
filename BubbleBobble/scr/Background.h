#pragma once

namespace BubbleBobble
{
	class Balbino::Texture2D;
	class Background : public Balbino::SceneObject
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
		std::shared_ptr<Balbino::Texture2D> m_Texture{};

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
	};
}
