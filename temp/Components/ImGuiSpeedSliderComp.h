#pragma once
#include <Component.h>
#include <vector>
#include <Transform.h>
#include <string>

namespace dae {
    class ImGuiSpeedSliderComp :
        public Component
    {
        std::vector<Transform*> m_TransformComponents{};
        const std::string m_Title{};
        float m_MinSpeed{ 100.f };
        float m_MaxSpeed{ 400.f };
    public:
        ImGuiSpeedSliderComp(GameObject* owner, std::string& title) : Component(owner), m_Title{ title} {};
        void SetMinMaxSpeed(float min, float max);
        virtual void Update(float) override {};
        virtual void Render() const override;
        void AddTransform(Transform* transform);
    };
}


