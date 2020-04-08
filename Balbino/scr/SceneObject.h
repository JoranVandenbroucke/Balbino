#pragma once

namespace Balbino
{
	class SceneObject
	{
	public:
		virtual void Create() = 0;
		virtual void Update() = 0;
		virtual void Draw() const = 0;
		virtual void Destroy() = 0;

		SceneObject();
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;
	protected:
	};
}