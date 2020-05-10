#pragma once
#include "Component.h"
#include "../Struct.h"

#include <memory>

#pragma warning(push)
#pragma warning(disable:4127)
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)
namespace Balbino
{
	class GameObject;

	class Camera final : public Component
	{
		typedef unsigned int GLuint;
	public:
		explicit Camera( const GameObject* const origine, float aspectRatio, float w );


		Camera( const Camera& ) = delete;
		Camera( Camera&& ) = delete;
		Camera& operator= ( const Camera& ) = delete;
		Camera& operator= ( const Camera&& ) = delete;

		virtual ~Camera() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;

		void SetClearColor( const Color& newClearColor );
		void SetDepth( int newDepth );
		const Color& GetClearColor()const;
		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;
		int GetCameraIndex()const;
		int GetDepth()const;
		GLuint GetTargetTexture()const;
		static Camera* GetMainCamera();

		static std::vector<Camera*>& GetAllCameras();


#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG
	private:
		int m_Depth;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		GLuint m_CameraIndex;
		GLuint m_RenderTexutre;

		Color m_ClearColor;
		bool m_IsMain;
		static Camera* m_pMainCamera;

		static std::vector<Camera*> m_pAllCameras;

	};
}