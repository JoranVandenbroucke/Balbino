#pragma once
#include "IManager.h"

#include "Core.h"

class CResourceManager;

namespace Balbino
{
	class CRenderer;
	class CInputHandler;
	class CAssetManager;

	class BALBINO_API CSystem final : public ISystem
	{
	public:
		CSystem();
		~CSystem() override = default;
		CSystem( const CSystem& ) = delete;
		CSystem( CSystem&& ) = delete;
		CSystem& operator=( const CSystem& ) = delete;
		CSystem& operator=( CSystem&& ) = delete;

		void Initialize();
		void Cleanup();

		void SetInputHandler( CInputHandler* pInputHandler );
		void SetCurrentScene( IScene* pScene );

		bool Update( bool isPause ) override;
		[[nodiscard]] IResourceManager* GetResourceManager() const override;
		[[nodiscard]] CInputHandler* GetInputHandler() const;
		[[nodiscard]] IScene* GetCurrentActiveScene() const override;
		
	private:
		CInputHandler* m_inputHandler;
		CResourceManager* m_resourceManager;
		IScene* m_pCurrentScene;
	};
}
