//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "NPCevent.h"
#include "basecombatcharacter.h"
#include "AI_BaseNPC.h"
#include "player.h"
#include "game.h"
#include "in_buttons.h"
#include "AI_Memory.h"
#include "soundent.h"
#include "rumble_shared.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar    sk_plr_dmg_smg1_grenade;	

class CWeaponM4A1 : public CHLMachineGun
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CWeaponM4A1, CHLMachineGun );

	CWeaponM4A1();

	DECLARE_SERVERCLASS();
	
	void	Precache( void );
	void	AddViewKick( void );

	int		GetMinBurst() { return 2; }
	int		GetMaxBurst() { return 5; }

	virtual void Equip( CBaseCombatCharacter *pOwner );
	bool	Reload( void );

	float	GetFireRate( void ) { return 0.08; }	// 12.5hz, 750rpm
	int		CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	Activity	GetPrimaryAttackActivity( void );

	virtual const Vector& GetBulletSpread( void )
	{
		static const Vector cone = VECTOR_CONE_2DEGREES / 60;
		return cone;
	}

	const WeaponProficiencyInfo_t *GetProficiencyValues();

	void FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir );
	void Operator_ForceNPCFire( CBaseCombatCharacter  *pOperator, bool bSecondary );
	void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	bool m_bIsSuppressed;

	DECLARE_ACTTABLE();
	void DryFire( void );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void WeaponIdle( void );
	bool Deploy(void);
};

IMPLEMENT_SERVERCLASS_ST(CWeaponM4A1, DT_WeaponM4A1)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_arifle_m4a1, CWeaponM4A1 );
PRECACHE_WEAPON_REGISTER(weapon_arifle_m4a1);

BEGIN_DATADESC( CWeaponM4A1 )
	DEFINE_FIELD(m_bIsSuppressed, FIELD_BOOLEAN),
END_DATADESC()

acttable_t	CWeaponM4A1::m_acttable[] = 
{
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_SMG1,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SMG1,				true },
	{ ACT_IDLE,						ACT_IDLE_SMG1,					true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SMG1,			true },

	{ ACT_WALK,						ACT_WALK_RIFLE,					true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true  },
	
// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_SMG1_RELAXED,			false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_SMG1_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_RELAXED,				ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_RIFLE_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_RELAXED,				ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_RIFLE_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_RIFLE,				false },//always aims

// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_SMG1_RELAXED,			false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_RIFLE_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_RIFLE_STIMULATED,	false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_RIFLE_STIMULATED,	false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_RIFLE,				false },//always aims
//End readiness activities

	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,		true },
	{ ACT_RUN,						ACT_RUN_RIFLE,					true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_RIFLE,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_SMG1,	true },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_SMG1_LOW,		true },
	{ ACT_COVER_LOW,				ACT_COVER_SMG1_LOW,				false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_SMG1_LOW,			false },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SMG1_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SMG1,		true },
};

IMPLEMENT_ACTTABLE(CWeaponM4A1);

//=========================================================
CWeaponM4A1::CWeaponM4A1( )
{
	m_fMinRange1		= 0;// No minimum range. 
	m_fMaxRange1		= 1400;
	m_bIsSuppressed = false;
	m_bAltFiresUnderwater = false;

	m_nFireMode = 0;
	m_nShotsLeft = m_nFireMode;
	m_nBurstRate = 3;
	m_nBurstToothState = m_nBurstRate;
	m_bFMAutomatic = true;
	m_bHasSemiAuto = true;
	m_bHasBurstGroup = false;
	m_bHKBurstType = false; // if this M4A1 somehow sprouted a burst group, it'd be the special snowflake.
	m_bIsClosedBolt = true;
	m_bManuallyOperated = false;
	m_bFMCycleDirection = true;
	m_bUsesSwitchToChange = true;
	m_bAcceleratesFA = false;
	m_fAlternateBurstROF = 10.0f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM4A1::Precache( void )
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Give this weapon longer range when wielded by an ally NPC.
//-----------------------------------------------------------------------------
void CWeaponM4A1::Equip( CBaseCombatCharacter *pOwner )
{
	if( pOwner->Classify() == CLASS_PLAYER_ALLY )
	{
		m_fMaxRange1 = 3000;
	}
	else
	{
		m_fMaxRange1 = 1400;
	}

	BaseClass::Equip( pOwner );
}

void CWeaponM4A1::DryFire( void )
{
	WeaponSound( EMPTY );
	if (m_bIsSuppressed) {
		SendWeaponAnim( ACT_VM_DRYFIRE_SILENCED );
	} else {
		SendWeaponAnim( ACT_VM_DRYFIRE );
	}

	m_flNextPrimaryAttack		= gpGlobals->curtime + SequenceDuration();
}
void CWeaponM4A1::PrimaryAttack( void )
{
	// If there is less than 1 shot left in this firing mode... return
	if (m_nShotsLeft < 1)
		return;

	// If the firing mode is less than four, remove one from the shots left counter...
	if (m_nFireMode < 3)
		m_nShotsLeft--;

	// and update the burst tooth state.
	--m_nBurstToothState;

	if (m_bIsSuppressed)
	{
		CSoundEnt::InsertSound(SOUND_WORLD, GetAbsOrigin(), SOUNDENT_VOLUME_EMPTY, 0.2, GetOwner());
	}
	else
	{
		CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, GetOwner());
	}
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if( pOwner )
	{
		// Each time the player fires the pistol, reset the view punch. This prevents
		// the aim from 'drifting off' when the player fires very quickly. This may
		// not be the ideal way to achieve this, but it's cheap and it works, which is
		// great for a feature we're evaluating. (sjb)
		pOwner->ViewPunchReset();
	}

	// If my clip is empty (and I use clips) start reload
	if ( UsesClipsForAmmo1() && !m_iClip1 ) 
	{
		Reload();
		return;
	}

	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if (!pPlayer)
	{
		return;
	}

	// MUST call sound before removing a round from the clip of a CMachineGun
	if (m_bIsSuppressed) {
		WeaponSound(SPECIAL1);
	} else {
		WeaponSound(SINGLE);
	}

	pPlayer->DoMuzzleFlash();
	if (m_bIsSuppressed) {
		SendWeaponAnim( ACT_VM_PRIMARYATTACK_SILENCED );
	} else {
		SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	}

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	FireBulletsInfo_t info;
	info.m_vecSrc	 = pPlayer->Weapon_ShootPosition( );

	info.m_vecDirShooting = pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	info.m_iShots = 0;

	float fireRate = 0.5f;
	// If I accelerate ROF in burst, and I'm in the appropriate fire mode 
	// (Nikonov weapons in burst/FA, G11 in burst), use the alternate rate of fire.
	if (m_fAlternateBurstROF < 5 && ((m_bAcceleratesFA && m_nFireMode > 1) || (!m_bAcceleratesFA && m_nFireMode == 2)))
		fireRate = m_fAlternateBurstROF;
	else
		fireRate = GetFireRate();

	while ( m_flNextPrimaryAttack <= gpGlobals->curtime )
	{
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		info.m_iShots++;
		if ( !fireRate )
			break;
	}

	// Make sure we don't fire more than the amount in the clip
	if ( UsesClipsForAmmo1() )
	{
		info.m_iShots = min( info.m_iShots, m_iClip1 );
		m_iClip1 -= info.m_iShots;
	}
	else
	{
		info.m_iShots = min( info.m_iShots, pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) );
		pPlayer->RemoveAmmo( info.m_iShots, m_iPrimaryAmmoType );
	}

	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;

	// Fire the bullets
	info.m_vecSpread = pPlayer->GetAttackSpread( this );

	pPlayer->FireBullets( info );
	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0 &&
		((!UsesMagazines() && pPlayer->m_pInventory.CountAllObjectsOfID(GetPrimaryAmmoID()) <= 0)) &&
		(UsesMagazines() && pPlayer->m_pInventory.CountAllObjectContentsOfID(GetPrimaryMagazineID()) <= 0) )
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	//Add our view kick in
	AddViewKick();
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM4A1::SecondaryAttack( void )
{
	DisableIronsights();
	if (m_bIsSuppressed) 
	{
		SendWeaponAnim( ACT_VM_DETACH_SILENCER );
		m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
		m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
		m_bIsSuppressed = false;
	} 
	else 
	{
		SendWeaponAnim( ACT_VM_ATTACH_SILENCER );
		m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
		m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
		m_bIsSuppressed = true;
	}
}
void CWeaponM4A1::WeaponIdle( void )
{
	if (m_bIsSuppressed) {
		//Idle again if we've finished
		if ( HasWeaponIdleTimeElapsed() )
		{
			SendWeaponAnim( ACT_VM_IDLE_SILENCED );
		}
	} else {
		//Idle again if we've finished
		if ( HasWeaponIdleTimeElapsed() )
		{
			SendWeaponAnim( ACT_VM_IDLE );
		}
	}
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM4A1::FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir )
{
	// FIXME: use the returned number of bullets to account for >10hz firerate
	WeaponSoundRealtime( SINGLE_NPC );

	CSoundEnt::InsertSound( SOUND_COMBAT|SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy() );
	pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED,
		MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2, entindex(), 0 );

	pOperator->DoMuzzleFlash();
	m_iClip1 = m_iClip1 - 1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM4A1::Operator_ForceNPCFire( CBaseCombatCharacter *pOperator, bool bSecondary )
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	Vector vecShootOrigin, vecShootDir;
	QAngle	angShootDir;
	GetAttachment( LookupAttachment( "muzzle" ), vecShootOrigin, angShootDir );
	AngleVectors( angShootDir, &vecShootDir );
	FireNPCPrimaryAttack( pOperator, vecShootOrigin, vecShootDir );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM4A1::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_SMG1:
		{
			Vector vecShootOrigin, vecShootDir;
			QAngle angDiscard;

			// Support old style attachment point firing
			if ((pEvent->options == NULL) || (pEvent->options[0] == '\0') || (!pOperator->GetAttachment(pEvent->options, vecShootOrigin, angDiscard)))
			{
				vecShootOrigin = pOperator->Weapon_ShootPosition();
			}

			CAI_BaseNPC *npc = pOperator->MyNPCPointer();
			ASSERT( npc != NULL );
			vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin );

			FireNPCPrimaryAttack( pOperator, vecShootOrigin, vecShootDir );
		}
		break;

		/*//FIXME: Re-enable
		case EVENT_WEAPON_AR2_GRENADE:
		{
		CAI_BaseNPC *npc = pOperator->MyNPCPointer();

		Vector vecShootOrigin, vecShootDir;
		vecShootOrigin = pOperator->Weapon_ShootPosition();
		vecShootDir = npc->GetShootEnemyDir( vecShootOrigin );

		Vector vecThrow = m_vecTossVelocity;

		CGrenadeAR2 *pGrenade = (CGrenadeAR2*)Create( "grenade_ar2", vecShootOrigin, vec3_angle, npc );
		pGrenade->SetAbsVelocity( vecThrow );
		pGrenade->SetLocalAngularVelocity( QAngle( 0, 400, 0 ) );
		pGrenade->SetMoveType( MOVETYPE_FLYGRAVITY ); 
		pGrenade->m_hOwner			= npc;
		pGrenade->m_pMyWeaponAR2	= this;
		pGrenade->SetDamage(sk_npc_dmg_ar2_grenade.GetFloat());

		// FIXME: arrgg ,this is hard coded into the weapon???
		m_flNextGrenadeCheck = gpGlobals->curtime + 6;// wait six seconds before even looking again to see if a grenade can be thrown.

		m_iClip2--;
		}
		break;
		*/

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CWeaponM4A1::GetPrimaryAttackActivity( void )
{
	return ACT_VM_PRIMARYATTACK;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CWeaponM4A1::Reload( void )
{
	CBaseCombatCharacter *pOwner = GetOwner();
	if (!pOwner)
		return false;

	if (m_bIsSuppressed) {
		bool fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD_SILENCED );
		if ( fRet )
		{
			// Play the player's reload animation
			if ( pOwner->IsPlayer() )
			{
				( ( CBasePlayer * )pOwner)->SetAnimation( PLAYER_RELOAD );
			}
		}
		return fRet;
	} else {
		bool fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD );
		if ( fRet )
		{
			// Play the player's reload animation
			if ( pOwner->IsPlayer() )
			{
				( ( CBasePlayer * )pOwner)->SetAnimation( PLAYER_RELOAD );
			}
		}
		return fRet;
	}

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM4A1::AddViewKick( void )
{
	#define	EASY_DAMPEN			0.5f
	#define	MAX_VERTICAL_KICK	1.0f	//Degrees
	#define	SLIDE_LIMIT			2.0f	//Seconds
	
	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( pPlayer == NULL )
		return;

	DoMachineGunKick( pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT );
}



//-----------------------------------------------------------------------------
const WeaponProficiencyInfo_t *CWeaponM4A1::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.75	},
		{ 5.00,		0.75	},
		{ 10.0/3.0, 0.75	},
		{ 5.0/3.0,	0.75	},
		{ 1.00,		1.0		},
	};

	COMPILE_TIME_ASSERT( ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}

bool CWeaponM4A1::Deploy(void)
{
	if (m_bIsSuppressed)
	{
		DefaultDeploy((char*)GetViewModel(), (char*)GetWorldModel(), ACT_VM_DRAW_SILENCED, (char*)GetAnimPrefix());
	} 
	else
	{
		DefaultDeploy((char*)GetViewModel(), (char*)GetWorldModel(), ACT_VM_DRAW, (char*)GetAnimPrefix());
	}

	m_nShotsFired = 0;
	m_bFMReady = true;

	return BaseClass::Deploy();
}


