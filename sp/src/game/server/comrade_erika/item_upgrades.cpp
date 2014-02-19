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
// Small health kit. Heals the player when picked up.
//-----------------------------------------------------------------------------
class CHealthUpgrade : public CItem
{
public:
	DECLARE_CLASS( CHealthUpgrade, CItem );

	void Spawn( void );
	void Precache( void );
	bool MyTouch( CBasePlayer *pPlayer );
};

LINK_ENTITY_TO_CLASS( upgrade_health, CHealthUpgrade );
PRECACHE_REGISTER( upgrade_health );


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHealthUpgrade::Spawn( void )
{
	Precache();
	SetModel( "models/items/healthkit.mdl" );

	BaseClass::Spawn();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHealthUpgrade::Precache( void )
{
	PrecacheModel("models/items/healthkit.mdl");

	PrecacheScriptSound( "HealthKit.Touch" );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pPlayer - 
// Output : 
//-----------------------------------------------------------------------------
bool CHealthUpgrade::MyTouch( CBasePlayer *pPlayer )
{
	++pPlayer->m_Local.m_iHealthUpgrades;
	pPlayer->ResetMaxHealth();

	return true;
}

class CArmorUpgrade : public CItem
{
public:
	DECLARE_CLASS( CArmorUpgrade, CItem );

	void Spawn( void )
	{ 
		Precache( );
		SetModel( "models/items/battery.mdl" );
		BaseClass::Spawn( );
	}
	void Precache( void )
	{
		PrecacheModel ("models/items/battery.mdl");

		PrecacheScriptSound( "ItemBattery.Touch" );

	}
	bool MyTouch( CBasePlayer *pPlayer )
	{
		++pPlayer->m_Local.m_iArmorUpgrades;
		return true;
	}
};

LINK_ENTITY_TO_CLASS(upgrade_armor, CArmorUpgrade);
PRECACHE_REGISTER(upgrade_armor);

