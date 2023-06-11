#pragma once
#include <Command/Command.h>

#include <Managers/SceneManager.h>

namespace dae
{
    class SceneSwitchCommand :
        public Command
    {
    public:
        SceneSwitchCommand(bool moveToNext) : m_GoToNext{moveToNext} {}
        void Execute(float) override
        {
        	auto sceneManager{ &SceneManager::GetInstance() };
            if(m_GoToNext)
            {
                sceneManager->GoToNextScene();
            }
            else
            {
                sceneManager->GoToPreviousScene();
            }
        }
    private:
        bool m_GoToNext{true};
    };
}


