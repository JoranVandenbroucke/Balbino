#pragma once
//#include <Balbino.h>

namespace BubbleBobble
{
	//class Balbino::GameObject;

	class Background : public Balbino::GameObject
	{
	public:
		void Create() override;

		Background() = default;
		virtual ~Background();
		Background(const Background& other) = delete;
		Background(Background&& other) = delete;
		Background& operator=(const Background& other) = delete;
		Background& operator=(Background&& other) = delete;

	private:
	};
}
