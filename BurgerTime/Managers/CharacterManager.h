#pragma once
#include <memory>
#include <vector>
#include <glm/fwd.hpp>

#include <Misc/Singleton.h>

namespace dae
{
	class Scene;
	class CharacterComponent;
    class GameObject;
    class CharacterManager  final :
        public Singleton<CharacterManager>
    {
    public:
        CharacterManager() = default;
        std::shared_ptr<GameObject> InitPlayer();
        void InitEnemies(Scene* scene,GameObject* player, glm::vec2& gridStartPos, int backgroundWidth, int backgroundHeight, float) const;
        std::vector<GameObject*> GetCharacters();
        GameObject* GetPlayer() const { return m_Player; };
    private:
        GameObject* m_Player{};
        std::vector<GameObject*> m_Enemies{};
    };
}

