#pragma once
#include "Singleton.h"

#include <vector>

namespace Balbino
{
	class CCamera;

	class CCameraManager final : public Singleton<CCameraManager>
	{
	public:
		static const CCamera* GetMainCamera();
		static const std::vector<CCamera*>& GetCameras();
		void AddCamera( CCamera* cam );
	private:
		friend Singleton<CCameraManager>;
		CCameraManager();
		virtual ~CCameraManager();
		CCameraManager( const CCameraManager& ) = delete;
		CCameraManager( CCameraManager&& ) = delete;
		CCameraManager& operator=( const CCameraManager& ) = delete;
		CCameraManager& operator=( CCameraManager&& ) = delete;


		const CCamera* IGetMainCamera() const;
		const std::vector<CCamera*>& IGetCameras() const;

		void SortList();

		CCamera* m_pMainCamera;
		std::vector<CCamera*> m_Cameras;
	};
}
