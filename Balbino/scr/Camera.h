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

		void Initialize(float aspectRatio, const glm::vec3& pos, float xAngle, float yAngle);

		const glm::mat4& GetView() const;
		const glm::mat4& GetProjection() const;
		void SetDrawIndex(int index);
		void Update(float dt);

		int GetDrawIndex() const;
		void Horizontal(float value);
		void Vertical(float value);
	private:
		int m_index;
		glm::vec2 m_inputDir;
		glm::mat4 m_view;
		glm::mat4 m_projection;

		float m_pitch;
		float m_yaw;
	};
}
