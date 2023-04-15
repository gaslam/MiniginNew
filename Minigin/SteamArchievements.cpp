#include <Windows.h>
#include "SteamArchievements.h"
#include <debugapi.h>
#include "isteamuserstats.h"

// code copied from https://partner.steamgames.com/doc/features/achievements/ach_guide with some minor adjustments to fit with the rules
using namespace dae;
SteamArchievements::SteamArchievements() :
    m_iAppID(0),
    m_bInitialized(false),
    m_CallbackUserStatsReceived(this, &SteamArchievements::OnUserStatsReceived),
    m_CallbackUserStatsStored(this, &SteamArchievements::OnUserStatsStored),
    m_CallbackAchievementStored(this, &SteamArchievements::OnAchievementStored)
{
    m_iAppID = SteamUtils()->GetAppID();
    RequestStats();
}

bool SteamArchievements::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool SteamArchievements::SetAchievement(int id)
{
	// Have we received a call back from Steam yet?
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(m_Achievements[id].m_pchAchievementID);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

void SteamArchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString("Received stats and achievements from Steam\n");
			m_bInitialized = true;

			// load achievements
			for (int iAch = 0; iAch < m_TotalAchievements; ++iAch)
			{
				Achievement_t& ach = m_Achievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf_s(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"name"));
				_snprintf_s(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"desc"));
			}

			for (auto& achievement : m_Achievements)
			{
				SteamUserStats()->ClearAchievement(achievement.m_pchAchievementID);
				SteamUserStats()->StoreStats();
			}
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(buffer);
		}
	}
}

void SteamArchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString("Stored stats for Steam\n");
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(buffer);
		}
	}
}

void SteamArchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID)
	{
		OutputDebugString("Stored Achievement for Steam\n");
	}
}