#include "pch.h"
#include "Manager.h"

#include "../Input/InputHandler.h"
#include "CameraManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"

Balbino::CInputHandler* Balbino::CManager::m_inputHandler{ nullptr };
Balbino::CCameraManager* Balbino::CManager::m_cameraManager{ nullptr };
Balbino::CMeshManager* Balbino::CManager::m_meshManager{ nullptr };
Balbino::CShaderManager* Balbino::CManager::m_shaderManager{ nullptr };
Balbino::CTextureManager* Balbino::CManager::m_textureManager{ nullptr };
Balbino::CManager::CManager()
= default;

Balbino::CManager::~CManager()
= default;

void Balbino::CManager::SetInputHandler(CInputHandler* pInputHandler)
{
	m_inputHandler = pInputHandler;
}

void Balbino::CManager::SetCameraManager(CCameraManager* pCameraManager)
{
	m_cameraManager = pCameraManager;
}

void Balbino::CManager::SetMeshManager(CMeshManager* pMeshManager)
{
	m_meshManager = pMeshManager;
}

void Balbino::CManager::SetShaderManager(CShaderManager* pShaderManager)
{
	m_shaderManager = pShaderManager;
}

void Balbino::CManager::SetTextureManager(CTextureManager* pTextureManager)
{
	m_textureManager = pTextureManager;
}

Balbino::CTextureManager* Balbino::CManager::GetTextureManager()
{
	return m_textureManager;
}

Balbino::CShaderManager* Balbino::CManager::GetShaderManager()
{
	return m_shaderManager;
}

Balbino::CMeshManager* Balbino::CManager::GetMeshManager()
{
	return m_meshManager;
}

Balbino::CCameraManager* Balbino::CManager::GetCameraManager()
{
	return m_cameraManager;
}

Balbino::CInputHandler* Balbino::CManager::GetInputHandler()
{
	return m_inputHandler;
}
