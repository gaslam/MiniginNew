#pragma once
#include "Component.h"
namespace dae {
	class ImGuiTutorial : public Component
	{
	public:
		ImGuiTutorial(GameObject* pObject) : Component(pObject) {};
		void Render() const override;
		void Update(float) override {};
	};
}

