//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "basecombatweapon_shared.h"

#ifndef BASEHLCOMBATWEAPON_SHARED_H
#define BASEHLCOMBATWEAPON_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
#define CBaseHLCombatWeapon C_BaseHLCombatWeapon
#endif

class CBaseHLCombatWeapon : public CBaseCombatWeapon
{
#if !defined( CLIENT_DLL )
#ifndef _XBOX
	DECLARE_DATADESC();
#else
protected:
	DECLARE_DATADESC();
private:
#endif
#endif

	DECLARE_CLASS( CBaseHLCombatWeapon, CBaseCombatWeapon );
public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	virtual bool	WeaponShouldBeLowered( void );

			bool	CanLower();
	virtual bool	Ready( void );
	virtual bool	Lower( void );
	virtual bool	Deploy( void );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual void	WeaponIdle( void );

	virtual void	AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles );
	virtual	float	CalcViewmodelBob( void );

	int GetFireMode(void) { return m_nFireMode; }
	int GetBurstSize(void) { return m_nBurstRate; }

	virtual Vector	GetBulletSpread( WeaponProficiency_t proficiency );
	virtual float	GetSpreadBias( WeaponProficiency_t proficiency );

	virtual const	WeaponProficiencyInfo_t *GetProficiencyValues();
	static const	WeaponProficiencyInfo_t *GetDefaultProficiencyValues();

	virtual void	ItemHolsterFrame( void );

	int				m_iPrimaryAttacks;		// # of primary attacks performed with this weapon
	int				m_iSecondaryAttacks;	// # of secondary attacks performed with this weapon

protected:

	bool			m_bLowered;			// Whether the viewmodel is raised or lowered
	float			m_flRaiseTime;		// If lowered, the time we should raise the viewmodel
	float			m_flHolsterTime;	// When the weapon was holstered

	int m_nShotsLeft;			// Fire mode remaining shots
	bool m_bFMReady;			// Fire mode Ready switch
	bool m_bFMAutomatic;		// Can I go full-auto?
	bool m_bHasSemiAuto;		// Some automatic weapons are not actually select-fire.
	bool m_bHasBurstGroup;		// Does my FCG even *have* a burst mode?
	bool m_bHKBurstType;		// Does this weapon have a fire control group that resets?
								// NB: HK weapons do, ARs don't, because reasons.
	bool m_bIsClosedBolt;		// Do I fire from a closed bolt?
	bool m_bManuallyOperated;	// Weapon cannot automatically cycle itself.
	bool m_bFMCycleDirection;	// true for most automatics, false for AKs.
	bool m_bUsesSwitchToChange;	// For special snowflake weapons like the AUG, Uzi and TMP
	// that select fire modes by squeezing the trigger harder.
	bool m_bAcceleratesFA;		// AN-94 shoots twice really fast in full-auto and goes to
	// normal speed after burst.
	float m_fAlternateBurstROF;	// For the AN-94, G11 and other weapons that
	// have a mechanism for accelerating rate of fire during bursts. 

	int m_nBurstToothState;		// The burst FCG of an AR-based weapon is the special snowflakiest.
};

#endif // BASEHLCOMBATWEAPON_SHARED_H
