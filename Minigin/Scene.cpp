#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

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

void dae::Scene::AddPlayer(std::shared_ptr<GameObject> object)
{
	m_Players.emplace_back(std::move(object));
}

void dae::Scene::RemovePlayer(std::shared_ptr<GameObject> object)
{
	m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), object), m_Players.end());
}

void Scene::RemoveAllPlayers()
{
	m_Players.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	for (auto& player : m_Players)
	{
		player->Update(deltaTime);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}

	for (auto& player : m_Players)
	{
		player->Render();
	}
}

std::vector<GameObject*> dae::Scene::GetPlayers()
{
	auto players = std::vector<GameObject*>();
	for (auto& player : m_Players)
	{
		players.push_back(player.get());
	}
	return players;
}

