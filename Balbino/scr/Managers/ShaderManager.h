#pragma once
#include "Common.h"

namespace BalVulkan
{
	class CShader;
	class CDevice;
}

namespace Balbino
{
	class CShaderManager final
	{
	public:
		CShaderManager();
		~CShaderManager();
		CShaderManager( const CShaderManager& ) = delete;
		CShaderManager( CShaderManager&& ) = delete;
		CShaderManager& operator=( const CShaderManager& ) = delete;
		CShaderManager& operator=( CShaderManager&& ) = delete;

		void Initialize( const BalVulkan::CDevice* pDevice );
		void Cleanup();

		void AddShader( BalVulkan::CShader* shader, const char* path, BalVulkan::EShaderStage shaderStage );
		const std::vector<BalVulkan::CShader*>& GetShaders() const;
	private:
		std::vector<BalVulkan::CShader*> m_shader;
	};
}
