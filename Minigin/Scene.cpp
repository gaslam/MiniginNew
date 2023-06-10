#include "Scene.h"
#include "GameObject.h"
#include "algorithm"

using namespace dae;

Scene::Scene(const std::string& name) : m_name(name) {}

void Scene::Start() const
{
	for(auto& object: m_objects)
	{
		object->Start();
	}
}

void Scene::End() const
{
	for (auto& object : m_objects)
	{
		object->End();
	}
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::RenderImGUI() const
{
	for (const auto& object : m_objects)
	{
		object->RenderImGUI();
	}
}

