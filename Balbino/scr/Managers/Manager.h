#pragma once
#include "../Core.h"

namespace Balbino
{
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

		void SetInputHandler(CInputHandler* pInputHandler);
		void SetCameraManager(CCameraManager* pCameraManager);
		void SetMeshManager(CMeshManager* pMeshManager);
		void SetShaderManager(CShaderManager* pShaderManager);
		void SetTextureManager(CTextureManager* pTextureManager);

		static CInputHandler* GetInputHandler();
		static CCameraManager* GetCameraManager();
		static CMeshManager* GetMeshManager();
		static CShaderManager* GetShaderManager();
		static CTextureManager* GetTextureManager();
	private:
		static CInputHandler* m_inputHandler;
		static CCameraManager* m_cameraManager;
		static CMeshManager* m_meshManager;
		static CShaderManager* m_shaderManager;
		static CTextureManager* m_textureManager;
	};
}