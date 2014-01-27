//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "fx.h"
#include "c_te_effect_dispatch.h"
#include "c_te_legacytempents.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ShellEjectCallback( const CEffectData &data )
{
	// Use the gun angles to orient the shell
	IClientRenderable *pRenderable = data.GetRenderable();
	if ( pRenderable )
	{
		tempents->EjectBrass( data.m_vOrigin, data.m_vAngles, pRenderable->GetRenderAngles(), 0 );
	}
}

DECLARE_CLIENT_EFFECT( "ShellEject", ShellEjectCallback );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void RifleShellEjectCallback( const CEffectData &data )
{
	// Use the gun angles to orient the shell
	IClientRenderable *pRenderable = data.GetRenderable();
	if ( pRenderable )
	{
		tempents->EjectBrass( data.m_vOrigin, data.m_vAngles, pRenderable->GetRenderAngles(), 1 );
	}
}

DECLARE_CLIENT_EFFECT( "RifleShellEject", RifleShellEjectCallback );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ShotgunShellEjectCallback( const CEffectData &data )
{
	// Use the gun angles to orient the shell
	IClientRenderable *pRenderable = data.GetRenderable();
	if ( pRenderable )
	{
		tempents->EjectBrass( data.m_vOrigin, data.m_vAngles, pRenderable->GetRenderAngles(), 2 );
	}
}

DECLARE_CLIENT_EFFECT( "ShotgunShellEject", ShotgunShellEjectCallback );


void CStrike_EjectBrass( int shell, const CEffectData &data )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if( !pPlayer )
		return;

	tempents->CSEjectBrass( data.m_vOrigin, data.m_vAngles, data.m_fFlags, shell, pPlayer );
}

void FX_EjectBrass_9mm_Callback( const CEffectData &data )
{
	CStrike_EjectBrass( CS_SHELL_9MM, data );
}

void FX_EjectBrass_12Gauge_Callback( const CEffectData &data )
{
	CStrike_EjectBrass( CS_SHELL_12GAUGE, data );
}

void FX_EjectBrass_57_Callback( const CEffectData &data )
{
	CStrike_EjectBrass( CS_SHELL_57, data );
}

void FX_EjectBrass_556_Callback( const CEffectData &data )
{
	CStrike_EjectBrass( CS_SHELL_556, data );
}

void FX_EjectBrass_762Nato_Callback( const CEffectData &data )
{
	CStrike_EjectBrass( CS_SHELL_762NATO, data );
}

void FX_EjectBrass_338Mag_Callback( const CEffectData &data )
{
	CStrike_EjectBrass( CS_SHELL_338MAG, data );
}


DECLARE_CLIENT_EFFECT( "EjectBrass_9mm",		FX_EjectBrass_9mm_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_12Gauge",	FX_EjectBrass_12Gauge_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_57",			FX_EjectBrass_57_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_556",		FX_EjectBrass_556_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_762Nato",	FX_EjectBrass_762Nato_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_338Mag",		FX_EjectBrass_338Mag_Callback );