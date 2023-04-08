#include "Observer.h"
#include "Event.h"
#include "GameObject.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"

using namespace dae;

void PlayerObserver::DisplayGameOver(GameObject* object)
{
	auto pTextComp = object->GetComponent<TextComponent>();
	if (pTextComp)
	{
		pTextComp->SetText("Game Over",true);
	}
}

void PlayerObserver::OnNotify(GameObject* object, Event& event)
{
	switch (event.GetId())
	{
	case EventType::PLAYER_DAMAGED:
		UpdateHealthDisplay(object);
		break;
	case EventType::PLAYER_DIED:
		DisplayGameOver(object);
		break;
	case EventType::SCORE_VALUE_CHANGED:
		UpdateScore(object);
	}
}

void PlayerObserver::UpdateScore(GameObject* object)
{
	auto pScoreComp = object->GetComponent<ScoreComponent>();
	auto pTextComp = object->GetComponent<TextComponent>();
	if (!pScoreComp || !pTextComp)
	{
		return;
	}

	int health = pScoreComp->GetScore();
	std::string textBegin = pScoreComp->GetTextBegin();
	std::string text = textBegin + std::to_string(health);
	pTextComp->SetText(text, true);
}

void PlayerObserver::UpdateHealthDisplay(GameObject* object)
{
	auto pHealthComp = object->GetComponent<HealthComponent>();
	auto pTextComp = object->GetComponent<TextComponent>();
	if (!pHealthComp|| !pTextComp)
	{
		return;
	}

	int health = pHealthComp->GetHealth();
	std::string textBegin = pHealthComp->GetTextBegin();
	std::string text = textBegin + std::to_string(health);
	pTextComp->SetText(text,true);
}
