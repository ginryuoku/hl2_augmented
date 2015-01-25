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
		pPlayer->UpgradeHealth();
	}
}

ConCommand upgrade_health("upgrade_health", UpgradeHealth, "Upgrades health - adds a segment to the health bar.");


void UpgradeArmor(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		pPlayer->UpgradeArmor();
	}
}
ConCommand upgrade_armor("upgrade_armor", UpgradeArmor, "Upgrades armor - adds a segment to the battery bar.");

void UpgradeHealthUnits(void)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		pPlayer->UpgradeHealthSegments();
	}
}
ConCommand upgrade_healthunits("upgrade_healthsegments", UpgradeHealthUnits, "Upgrades health - each segment of the health bar counts for more.");

void UpgradeArmorUnits(void)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		pPlayer->UpgradeArmorSegments();
	}
}
ConCommand upgrade_armorunits("upgrade_armorsegments", UpgradeArmorUnits, "Upgrades armor battery - each segment of the battery bar counts for more.");


void HL2ArmorCapacity(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		bool upgrade = false;
		if (pPlayer->m_Local.m_iHealthSegmentUpgrades < 2)
		{
			pPlayer->m_Local.m_iHealthSegmentUpgrades = 2;
			pPlayer->ResetMaxHealth();
		}
		if (pPlayer->m_Local.m_iArmorUpgrades < 4 )
		{
			pPlayer->m_Local.m_iArmorUpgrades = 4;
		}
		if (pPlayer->m_Local.m_iArmorSegmentUpgrades < 2)
		{
			pPlayer->m_Local.m_iArmorSegmentUpgrades = 2;
		}

		if (upgrade)
		{
			UTIL_CenterPrintAll("HEV upgraded to HL2 defaults");
		}
	}
}
ConCommand hl2_hev("hl2_hev", HL2ArmorCapacity, "Sets armor/health upgrades to HL2 default");