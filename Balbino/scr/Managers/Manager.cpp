#include "pch.h"
#include "Manager.h"

#include "CameraManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "../Input/InputHandler.h"

Balbino::CInputHandler* Balbino::CManager::s_inputHandler{nullptr};
Balbino::CCameraManager* Balbino::CManager::s_cameraManager{nullptr};
Balbino::CMeshManager* Balbino::CManager::s_meshManager{nullptr};
Balbino::CShaderManager* Balbino::CManager::s_shaderManager{nullptr};
Balbino::CTextureManager* Balbino::CManager::s_textureManager{nullptr};
Balbino::CMaterialManager* Balbino::CManager::s_materialManager{nullptr};
Balbino::CManager::CManager()
= default;

Balbino::CManager::~CManager()
= default;

void Balbino::CManager::Cleanup()
{
	s_inputHandler->Cleanup();
	s_meshManager->Cleanup();
	s_shaderManager->Cleanup();
	s_textureManager->Cleanup();
	s_materialManager->Cleanup();

	delete s_inputHandler;
	delete s_cameraManager;
	delete s_meshManager;
	delete s_shaderManager;
	delete s_textureManager;
	delete s_materialManager;
	
	s_inputHandler= nullptr;
	s_cameraManager= nullptr;
	s_meshManager= nullptr;
	s_shaderManager= nullptr;
	s_textureManager= nullptr;
	s_materialManager= nullptr;
}

void Balbino::CManager::SetInputHandler(CInputHandler* pInputHandler)
{
	s_inputHandler = pInputHandler;
}

void Balbino::CManager::SetCameraManager(CCameraManager* pCameraManager)
{
	s_cameraManager = pCameraManager;
}

void Balbino::CManager::SetMeshManager(CMeshManager* pMeshManager)
{
	s_meshManager = pMeshManager;
}

void Balbino::CManager::SetShaderManager(CShaderManager* pShaderManager)
{
	s_shaderManager = pShaderManager;
}

void Balbino::CManager::SetTextureManager(CTextureManager* pTextureManager)
{
	s_textureManager = pTextureManager;
}

void Balbino::CManager::SetMaterialManager(CMaterialManager* pMaterialManager)
{
	s_materialManager = pMaterialManager;
}

Balbino::CMaterialManager* Balbino::CManager::GetMaterialManager()
{
	return s_materialManager;
}

Balbino::CTextureManager* Balbino::CManager::GetTextureManager()
{
	return s_textureManager;
}

Balbino::CShaderManager* Balbino::CManager::GetShaderManager()
{
	return s_shaderManager;
}

Balbino::CMeshManager* Balbino::CManager::GetMeshManager()
{
	return s_meshManager;
}

Balbino::CCameraManager* Balbino::CManager::GetCameraManager()
{
	return s_cameraManager;
}

Balbino::CInputHandler* Balbino::CManager::GetInputHandler()
{
	return s_inputHandler;
}
