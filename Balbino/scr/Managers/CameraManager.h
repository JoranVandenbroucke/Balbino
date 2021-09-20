#pragma once
#include <vector>
#include <glm.hpp>
#include "../Camera.h"

namespace Balbino
{
	class CRenderer;

	class CCameraManager final
	{
	public:
		CCameraManager();
		virtual ~CCameraManager();
		CCameraManager( const CCameraManager& ) = delete;
		CCameraManager( CCameraManager&& ) = delete;
		CCameraManager& operator=( const CCameraManager& ) = delete;
		CCameraManager& operator=( CCameraManager&& ) = delete;

		void Update(float deltaTime) const;
		CCamera* AddCamera(const glm::vec3& pos = {0.0f,0.0f,0.0f}, float xAngle = 0, float yAngle = 0);
		void SetRenderer(const CRenderer* renderer);
		void Cleanup();

		const CCamera* GetMainCamera() const;
		const std::vector<CCamera>& GetCameras() const;
	private:

		void SortList();

		CCamera* m_pMainCamera;
		const CRenderer* m_pRenderer;
		std::vector<CCamera> m_cameras;
	};
}
