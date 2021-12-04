#pragma once
#include <string>

class CMainScreen
{
public:
	void Draw();
private:
	std::string m_SavePosition;
	bool m_Saved = false;
};
