#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <EngineCore/Minigin.h>
#include <memory>
#include "Utils.h"
#include "Components/GridComponent.h"



void load()
{
	MG_ASSERT_INFO("Starting load Process!!");
	MG_ASSERT_INFO("Adding scenes!!");
	Utils::AddScenes();
}

int main(int, char*[]) {
	MG_ASSERT_INFO("Starting program!!");
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}