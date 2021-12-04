#include "pch.h"
#include "ShaderManager.h"
#include <Shader.h>

Balbino::CShaderManager::CShaderManager() = default;

Balbino::CShaderManager::~CShaderManager()
{
	if (!m_shader.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

void Balbino::CShaderManager::Initialize(const BalVulkan::CDevice* pDevice)
{
	( void ) pDevice;
	//m_shader.emplace_back( DBG_NEW BalVulkan::CShader{pDevice} );
	//m_shader.emplace_back( DBG_NEW BalVulkan::CShader{pDevice} );
	std::cout << m_shader.size() << std::endl;
}

void Balbino::CShaderManager::Cleanup()
{
	m_shader.clear();
}

const std::vector<BalVulkan::CShader*>& Balbino::CShaderManager::GetShaders() const
{
	return m_shader;
}

void Balbino::CShaderManager::AddShader( BalVulkan::CShader* shader, const char* path, BalVulkan::EShaderStage shaderStage )
{
	const std::filesystem::path filePath{ std::filesystem::absolute( path ) };
	std::ifstream file{ filePath, std::ios::in | std::ios::binary };

	if ( !file.is_open() )
		return;
	const std::string shaderCode{ std::string( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() ) };
	shader->Initialize( static_cast<const void*>( shaderCode.c_str() ), shaderCode.size(), shaderStage, path );
	m_shader.push_back( shader );
}
