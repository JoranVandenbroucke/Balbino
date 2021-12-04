#pragma once
#include "../Core.h"

namespace Balbino
{
	class CMaterialManager;
	class CTextureManager;
	class CShaderManager;
	class CMeshManager;
	class CInputHandler;
	class CCameraManager;

	class BALBINO_API CManager
	{
	public:
		CManager();
		~CManager();
		CManager(const CManager&) = delete;
		CManager(CManager&&) = delete;
		CManager& operator=(const CManager&) = delete;
		CManager& operator=(CManager&&) = delete;

		void Cleanup();

		void SetInputHandler(CInputHandler* pInputHandler);
		void SetCameraManager(CCameraManager* pCameraManager);
		void SetMeshManager(CMeshManager* pMeshManager);
		void SetShaderManager(CShaderManager* pShaderManager);
		void SetTextureManager(CTextureManager* pTextureManager);
		void SetMaterialManager(CMaterialManager* pMaterialManager);

		static CInputHandler* GetInputHandler();
		static CCameraManager* GetCameraManager();
		static CMeshManager* GetMeshManager();
		static CShaderManager* GetShaderManager();
		static CTextureManager* GetTextureManager();
		static CMaterialManager* GetMaterialManager();
	private:
		static CInputHandler* s_inputHandler;
		static CCameraManager* s_cameraManager;
		static CMeshManager* s_meshManager;
		static CShaderManager* s_shaderManager;
		static CTextureManager* s_textureManager;
		static CMaterialManager* s_materialManager;
	};
}
