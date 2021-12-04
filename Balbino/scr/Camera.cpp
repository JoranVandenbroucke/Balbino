#include "pch.h"
#include "Camera.h"

#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>

#include "Input/InputHandler.h"
#include "Managers/Manager.h"

Balbino::CCamera::CCamera()
	: m_index{}
	, m_inputDir{}
	, m_view{}
	, m_projection{}
	, m_pitch{}
	, m_yaw{}
{
}

bool Balbino::CCamera::operator==(const CCamera& other) const
{
	return this->m_index == other.m_index &&
		this->m_yaw == other.m_yaw &&
		this->m_pitch == other.m_pitch;
}

void Balbino::CCamera::Initialize(float aspectRation, const glm::vec3& pos, const float xAngle, const float yAngle)
{
	CInputHandler* pInput = CManager::GetInputHandler();
	pInput->AddAxis("Vertical", SDLK_w, -1);
	pInput->AddAxis("Vertical", SDLK_s, 1);
	pInput->AddAxis("Horizontal", SDLK_a, -1);
	pInput->AddAxis("Horizontal", SDLK_d, 1);

	pInput->BindAxis("Horizontal", this, &CCamera::Horizontal);
	pInput->BindAxis("Vertical", this, &CCamera::Vertical);

	m_index = 0;
	m_yaw = yAngle;
	m_pitch = xAngle;
	m_inputDir = {};
	m_projection = glm::perspective(glm::radians(45.0f), aspectRation, 0.01f, 1000.0f);
	m_projection[1][1] *= -1;

	glm::vec3 forward{0, 0, 1};
	glm::vec3 right{1, 0, 0};
	forward = rotateX(forward, m_yaw);
	forward = rotateY(forward, m_pitch);
	right = rotateX(right, m_yaw);
	right = rotateY(right, m_pitch);
	const glm::vec3 up = cross(forward, right);
	m_view = lookAt(pos, pos + forward, up);
}

const glm::mat4& Balbino::CCamera::GetView() const
{
	return m_view;
}

const glm::mat4& Balbino::CCamera::GetProjection() const
{
	return m_projection;
}

void Balbino::CCamera::SetDrawIndex(const int index)
{
	m_index = index;
}

int Balbino::CCamera::GetDrawIndex() const
{
	return m_index;
}

void Balbino::CCamera::Update(float dt)
{
	glm::mat4 inverseInv{inverse(m_view)};
	glm::vec3 position{inverseInv[3].x, inverseInv[3].y, inverseInv[3].z};
	const glm::vec3 f{inverseInv[2].x, inverseInv[2].y, inverseInv[2].z};
	const glm::vec3 r{inverseInv[0].x, inverseInv[0].y, inverseInv[0].z};
	position += f * (m_inputDir.y * 4 * dt) + r * (m_inputDir.x * 4 * dt);

	float x, y;
	CInputHandler* pInput = CManager::GetInputHandler();
	pInput->GetRelativeMousePosition(x, y);
	m_yaw += y / 180.0f * 3.14159265359f;
	m_pitch -= x / 180.0f * 3.14159265359f;
	glm::vec3 forward{0, 0, 1};
	glm::vec3 right{1, 0, 0};
	forward = rotateX(forward, m_yaw);
	forward = rotateY(forward, m_pitch);
	right = rotateX(right, m_yaw);
	right = rotateY(right, m_pitch);
	const glm::vec3 up = cross(forward, right);
	m_view = lookAt(position, position + forward, up);
}

void Balbino::CCamera::Horizontal(float value)
{
	m_inputDir.x = value;
}

void Balbino::CCamera::Vertical(float value)
{
	m_inputDir.y = value;
}
