#pragma once
#include <Misc/Component.h>
namespace dae
{
	class AudioComponent;

	class SceneComponent :
    public Component
{
	public:
        SceneComponent(GameObject* gameObject);
		void Start() override;
		void End() override;
		private:
			AudioComponent* m_pBackgroundSound{};
};
}


