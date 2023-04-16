#pragma once
#include "steam_api.h"
#include "Component.h"
#include "Singleton.h"
struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

enum class EAchievements : int
{
	ACH_WIN_ONE_GAME = 0,
	ACH_WIN_100_GAMES = 1,
	ACH_TRAVEL_FAR_ACCUM = 2,
	ACH_TRAVEL_FAR_SINGLE = 3,
};

namespace dae {
	//Is a singleton so that the steam callback doesn't need to be performed multiple  times
	class SteamArchievements : public Singleton<SteamArchievements>
	{
	private:
		uint64 m_iAppID; // Our current AppID
		const static int m_TotalAchievements = 4;
		Achievement_t m_Achievements[m_TotalAchievements]
		{
			Achievement_t(static_cast<int>(EAchievements::ACH_WIN_ONE_GAME), "ACH_WIN_ONE_GAME"),
			Achievement_t(static_cast<int>(EAchievements::ACH_WIN_100_GAMES), "ACH_WIN_100_GAMES"),
			Achievement_t(static_cast<int>(EAchievements::ACH_TRAVEL_FAR_ACCUM), "ACH_TRAVEL_FAR_ACCUM"),
			Achievement_t(static_cast<int>(EAchievements::ACH_TRAVEL_FAR_SINGLE), "ACH_TRAVEL_FAR_SINGLE"),
		};
		bool m_bInitialized; // Have we called Request stats and received the callback?

	public:
		SteamArchievements();

		bool RequestStats();
		bool SetAchievement(int id);

		STEAM_CALLBACK(SteamArchievements, OnUserStatsReceived, UserStatsReceived_t,
			m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamArchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamArchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);
	};
}

