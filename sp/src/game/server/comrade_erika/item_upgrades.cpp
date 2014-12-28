//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements health kits and wall mounted health chargers.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "gamerules.h"
#include "player.h"
#include "items.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pPlayer - 
// Output : 
//-----------------------------------------------------------------------------

void UpgradeHealth(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		if (pPlayer->m_iCash < 1000)
			return;
		++pPlayer->m_Local.m_iHealthUpgrades;
		pPlayer->ResetMaxHealth();
		Msg("Health upgrades now: %d\n", pPlayer->m_Local.m_iHealthUpgrades);
	}
}

ConCommand upgrade_health("upgrade_health", UpgradeHealth, "Upgrades health");


void UpgradeArmor(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		if (pPlayer->m_iCash < 1000)
			return;

		++pPlayer->m_Local.m_iArmorUpgrades;
		Msg("Armor upgrades now: %d\n", pPlayer->m_Local.m_iArmorUpgrades);
	}
}
ConCommand upgrade_armor("upgrade_armor", UpgradeArmor, "Upgrades armor");

void HL2ArmorCapacity(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		if (pPlayer->m_Local.m_iHealthUpgrades < 3)
		{
			pPlayer->m_Local.m_iHealthUpgrades = 3;
			pPlayer->ResetMaxHealth();
		}
		if (pPlayer->m_Local.m_iArmorUpgrades < 4 )
		{
			pPlayer->m_Local.m_iArmorUpgrades = 4;
		}
		Msg("Armor upgrades now: %d\n", pPlayer->m_Local.m_iArmorUpgrades);
		Msg("Health upgrades now: %d\n", pPlayer->m_Local.m_iHealthUpgrades);
	}
}
ConCommand hl2_hev("hl2_hev", HL2ArmorCapacity, "Sets armor/health upgrades to HL2 default");