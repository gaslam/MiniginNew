#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <Component.h>
namespace dae {
    class GameObject;
    class TextComponent;
    class Texture2D;

    class FPSComponent :
        public Component
    {
    public:
        FPSComponent(GameObject* object);
        void Initialise();
        void Render() const override;
        void Update(float deltaTime) override;
    private:
        bool m_NeedsUpdate;
        int m_CountFPS{};
        float m_AccumulatedTime{};
        float sec{ 1.0f };
        std::string m_FPSValue{ "0" };
        TextComponent* m_TextRenderComp{nullptr};
    };
}

