#pragma once
#include "FileParcer.h"
#include "Shader.h"

struct ISystem;

namespace Balbino
{
	class CMaterial;
}

namespace BalEditor
{
	class CMaterialEditor final
	{
	public:
		CMaterialEditor();
		~CMaterialEditor() = default;
		CMaterialEditor( const CMaterialEditor& ) = delete;
		CMaterialEditor( CMaterialEditor&& ) = delete;
		CMaterialEditor& operator=( const CMaterialEditor& ) = delete;
		CMaterialEditor& operator=( CMaterialEditor&& ) = delete;

		void Initialize( const ISystem* pSystem );
		void Draw();
		void Cleanup();
		void ShowWindow();
		void SetMaterial( const SFile& currentMaterial );
	private:
		bool m_isVisible;
		SFile m_currentMaterial;

		const ISystem* m_pSystem;
		std::vector<BalVulkan::SShaderResource> m_shaderResources;
	};
}