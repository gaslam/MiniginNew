#include "Scene.h"
#include "GameObject.h"
#include "algorithm"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

std::vector<GameObject*> dae::Scene::GetSceneCharacters() const
{
	std::vector<std::shared_ptr<GameObject>> objects = m_objects;
	std::vector<GameObject*> characters{};
	auto new_end = std::copy_if(objects.begin(), objects.end(), objects.begin(), [](std::shared_ptr<GameObject> object)
	{
		return object->GetComponent<CharacterComponent>() != nullptr;
	});

	objects.resize(std::distance(objects.begin(), new_end));
	for(auto object: objects)
	{
		auto pObject = object.get();
		characters.emplace_back(pObject);
	}
	return characters;
}

Scene::Scene(const std::string& name) : m_name(name) {}

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

