#pragma once
#include <string>

class MainScreen
{
public:
	void Draw();
private:
	std::string m_SavePosition;
	bool m_Saved;
};

