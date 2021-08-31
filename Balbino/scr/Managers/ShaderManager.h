#pragma once
#include "Singleton.h"
#include <vector>

namespace Balbino
{
	class CShader;
	class CShaderManager final: public CSingleton<CShaderManager>
	{
	public:
		static const std::vector<CShader*>& GetShaders();
		void AddShader(CShader* cam);
	private:
		friend CSingleton<CShaderManager>;
		CShaderManager();
		virtual ~CShaderManager();
		CShaderManager(const CShaderManager&) = delete;
		CShaderManager(CShaderManager&&) = delete;
		CShaderManager& operator=(const CShaderManager&) = delete;
		CShaderManager& operator=(CShaderManager&&) = delete;

		const std::vector<Balbino::CShader*>& IGetShaders() const;

		void SortList();

		std::vector<CShader*> m_Shaders;
	};
}

