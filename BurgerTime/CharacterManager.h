#pragma once
#include <Singleton.h>
#include <memory>
#include <vector>

namespace dae
{
    class CharacterComponent;
    class GameObject;
    class CharacterManager  final :
        public Singleton<CharacterManager>
    {
    public:
        CharacterManager() = default;
        std::shared_ptr<GameObject> InitPlayer();
        std::vector<GameObject*> GetCharacters();
        GameObject* GetPlayer();
    private:
        GameObject* m_Player{};
        std::vector<GameObject*> m_Enemies{};
    };
}

