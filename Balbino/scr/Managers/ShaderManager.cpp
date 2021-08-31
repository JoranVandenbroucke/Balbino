#include "pch.h"
#include "ShaderManager.h"

const std::vector<Balbino::CShader*>& Balbino::CShaderManager::GetShaders()
{
	return GetInstance().IGetShaders();
}

void Balbino::CShaderManager::AddShader( CShader* cam )
{
	(void) cam;
}

Balbino::CShaderManager::CShaderManager()
{
}

Balbino::CShaderManager::~CShaderManager()
{
}

const std::vector<Balbino::CShader*>& Balbino::CShaderManager::IGetShaders() const
{
	return m_Shaders;
}

void Balbino::CShaderManager::SortList()
{
}
