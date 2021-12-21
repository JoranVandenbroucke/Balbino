#pragma once
#include <glm.hpp>

namespace Balbino
{
	class CCamera
	{
	public:
		CCamera();
		~CCamera() = default;
		CCamera(const CCamera&) = default;
		CCamera(CCamera&&) = default;
		CCamera& operator=(const CCamera&) = default;
		CCamera& operator=(CCamera&&) = default;

		bool operator==(const CCamera& other) const;

		void Initialize( uint32_t width, uint32_t height, float fov = glm::radians( 45.0f ));
		void SetDrawIndex( int index );

		const glm::mat4& GetView() const;
		const glm::mat4& GetProjection() const;
		int GetDrawIndex() const;
	private:
		glm::mat4 m_view;
		glm::mat4 m_projection;

		int m_index;
		uint32_t m_width;
		uint32_t m_height;
		float m_fov;
		float m_aspectRatio;
	};
}
