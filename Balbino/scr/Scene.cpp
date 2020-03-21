#include "BalbinoPCH.h"
#include "Scene.h"
#include "SceneObject.h"

using namespace Balbino;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name.c_str()) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Draw() const
{
	for (const auto& object : m_Objects)
	{
		object->Draw();
	}
}

