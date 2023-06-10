#pragma once
#include "SceneManager.h"
#include "Shape.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Start() const;
		void End() const;

		void Update(float deltaTime);
		void Render() const;
		void RenderImGUI() const;

		~Scene();
		explicit Scene(const std::string& name);
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};
	};

}
