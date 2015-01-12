//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "soundent.h"
#include "ai_basenpc.h"
#include "game.h"
#include "in_buttons.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_SERVERCLASS_ST( CHLMachineGun, DT_HLMachineGun )
END_SEND_TABLE()

//=========================================================
//	>> CHLSelectFireMachineGun
//=========================================================
BEGIN_DATADESC( CHLMachineGun )

	DEFINE_FIELD( m_nShotsFired,	FIELD_INTEGER ),
	DEFINE_FIELD( m_flNextSoundTime, FIELD_TIME ),

END_DATADESC()


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHLMachineGun::CHLMachineGun( void )
{
	// I'm assuming a typical H&K weapon here.
	m_nFireMode = 0;
	m_nShotsLeft = m_nFireMode;
	m_nBurstRate = 3;
	m_nBurstToothState = m_nBurstRate;
	m_bFMAutomatic = true;
	m_bHasSemiAuto = true;
	m_bHasBurstGroup = true;
	m_bHKBurstType = true;
	m_bIsClosedBolt = true;
	m_bManuallyOperated = false;
	m_bFMCycleDirection = true;
	m_bUsesSwitchToChange = true;
	m_bAcceleratesFA = false;
	m_fAlternateBurstROF = 10.0f;
}

const Vector &CHLMachineGun::GetBulletSpread( void )
{
	static Vector cone = VECTOR_CONE_3DEGREES;
	return cone;
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CHLMachineGun::PrimaryAttack( void )
{
	// If there is less than 1 shot left in this firing mode... return
	if (m_nShotsLeft < 1)
		return;

	// If the firing mode is less than four, remove one from the shots left counter...
	if (m_nFireMode < 3)
		m_nShotsLeft--;

	// and update the burst tooth state.
	--m_nBurstToothState;

	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;

	
	// Abort here to handle burst and auto fire modes
	if ( (UsesClipsForAmmo1() && m_iClip1 == 0) || ( !UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType) ) )
		return;

	m_nShotsFired++;

	pPlayer->DoMuzzleFlash();

	// To make the firing frame-rate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	int iBulletsToFire = 0;
	float fireRate = 0.5f;
	// If I accelerate ROF in burst, and I'm in the appropriate fire mode 
	// (Nikonov weapons in burst/FA, G11 in burst), use the alternate rate of fire.
	if (m_fAlternateBurstROF < 5 && ((m_bAcceleratesFA && m_nFireMode > 1) || (!m_bAcceleratesFA && m_nFireMode == 2)))
		fireRate = m_fAlternateBurstROF;
	else
		fireRate = GetFireRate();

	// MUST call sound before removing a round from the clip of a CHLMachineGun
	while ( m_flNextPrimaryAttack <= gpGlobals->curtime )
	{
		WeaponSound(SINGLE, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		iBulletsToFire++;
	}

	// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
	if ( UsesClipsForAmmo1() )
	{
		if ( iBulletsToFire > m_iClip1 )
			iBulletsToFire = m_iClip1;
		m_iClip1 -= iBulletsToFire;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );

	// Fire the bullets
	FireBulletsInfo_t info;
	info.m_iShots = iBulletsToFire;
	info.m_vecSrc = pPlayer->Weapon_ShootPosition( );
	info.m_vecDirShooting = pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );
	info.m_vecSpread = pPlayer->GetAttackSpread( this );
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;
	FireBullets( info );

	//Factor in the view kick
	AddViewKick();

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pPlayer );
	
	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0 && 
		((!UsesMagazines() && pPlayer->m_pInventory.CountAllObjectsOfID(GetPrimaryAmmoID()) <= 0)) &&
		(UsesMagazines() && pPlayer->m_pInventory.CountAllObjectContentsOfID(GetPrimaryMagazineID()) <= 0) )
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0); 
	}

	SendWeaponAnim( GetPrimaryAttackActivity() );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// Register a muzzleflash for the AI
	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
//-----------------------------------------------------------------------------
void CHLMachineGun::FireBullets( const FireBulletsInfo_t &info )
{
	if(CBasePlayer *pPlayer = ToBasePlayer ( GetOwner() ) )
	{
		pPlayer->FireBullets(info);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Weapon firing conditions
//-----------------------------------------------------------------------------
int CHLMachineGun::WeaponRangeAttack1Condition( float flDot, float flDist )
{
	if ( m_iClip1 <=0 )
	{
		return COND_NO_PRIMARY_AMMO;
	}
	else if ( flDist < m_fMinRange1 ) 
	{
		return COND_TOO_CLOSE_TO_ATTACK;
	}
	else if ( flDist > m_fMaxRange1 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}
	else if ( flDot < 0.5f )	// UNDONE: Why check this here? Isn't the AI checking this already?
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_RANGE_ATTACK1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHLMachineGun::DoMachineGunKick( CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime )
{
	#define	KICK_MIN_X			0.2f	//Degrees
	#define	KICK_MIN_Y			0.2f	//Degrees
	#define	KICK_MIN_Z			0.1f	//Degrees

	QAngle vecScratch;
	
	//Find how far into our accuracy degradation we are
	float duration	= ( fireDurationTime > slideLimitTime ) ? slideLimitTime : fireDurationTime;
	float kickPerc = duration / slideLimitTime;

	// do this to get a hard discontinuity, clear out anything under 10 degrees punch
	pPlayer->ViewPunchReset( 10 );

	//Apply this to the view angles as well
	vecScratch.x = -( KICK_MIN_X + ( maxVerticleKickAngle * kickPerc ) );
	vecScratch.y = -( KICK_MIN_Y + ( maxVerticleKickAngle * kickPerc ) ) / 3;
	vecScratch.z = KICK_MIN_Z + ( maxVerticleKickAngle * kickPerc ) / 8;

	//Wibble left and right
	if ( random->RandomInt( -1, 1 ) >= 0 )
		vecScratch.y *= -1;

	//Wobble up and down
	if ( random->RandomInt( -1, 1 ) >= 0 )
		vecScratch.z *= -1;

	//If we're in easy, dampen the effect a bit
	if ( g_pGameRules->IsSkillLevel( SKILL_EASY ) )
	{
		for ( int i = 0; i < 3; i++ )
		{
			vecScratch[i] *= dampEasy;
		}
	}

	//Clip this to our desired min/max
	UTIL_ClipPunchAngleOffset( vecScratch, pPlayer->m_Local.m_vecPunchAngle, QAngle( 24.0f, 3.0f, 1.0f ) );

	//Add it to the view punch
	// NOTE: 0.5 is just tuned to match the old effect before the punch became simulated
	pPlayer->ViewPunch( vecScratch * 0.5 );
}

//-----------------------------------------------------------------------------
// Purpose: Reset our shots fired
//-----------------------------------------------------------------------------
bool CHLMachineGun::Deploy( void )
{
	m_nShotsFired = 0;
	m_bFMReady = true;

	return BaseClass::Deploy();
}

bool CHLMachineGun::Holster(CBaseCombatWeapon *pSwitchingTo)
{
	m_nShotsLeft = 0;
	m_nFireMode = 0;
	return BaseClass::Holster(pSwitchingTo);
}

//----------------------------------------------------------
// Purpose: Implementation of firemode in all machine guns
//----------------------------------------------------------
void CHLMachineGun::FireMode(void)
{
	int oldmode = m_nFireMode;
	// Grab the current player
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer) // if there isn't one, return...
		return;

	// If player is pushing the firemode button and is okay to change mode,
	if (((pPlayer->m_nButtons & IN_FIREMODE) && (m_bFMReady == true)) && !(pPlayer->m_nButtons & IN_ATTACK))
	{
		if (m_bFMCycleDirection)
		{
			switch (m_nFireMode)
			{
			case 0:
			{
				if (m_bHasSemiAuto)
					m_nFireMode = 1;
				else // slam it to full-auto. Nobody known builds a burst-only weapon.
					m_nFireMode = 3;
				break;
			}
			case 1:
			{
				if (m_bHasBurstGroup)
					m_nFireMode = 2;
				else if (m_bFMAutomatic)
					m_nFireMode = 3;
				break; // get out. For semi-autos.
			}
			case 2:
			{
				if (m_bFMAutomatic)
					m_nFireMode = 3;
				else m_nFireMode = 1;
				break;
			}
			case 3:
			{
				if (m_bHasSemiAuto)
					m_nFireMode = 1;
				break; // If we're auto-only, do nothing.
			}
			default:
			{
				m_nFireMode = 0; // on safe, it'll at least go into a known-valid state.
				break;
			}
			}
		}
		else // We're an AK or other weapon with a reversed order of fire modes.
		{
			switch (m_nFireMode)
			{
			case 0: // we're at the top of the rotation.
			{
				if (m_bFMAutomatic)
					m_nFireMode = 3;
				else if (m_bHasBurstGroup)
					m_nFireMode = 2;
				else
					m_nFireMode = 1;
				break;
			}
			case 1: // we're at the bottom of the rotation.
			{
				if (m_bHasBurstGroup)
					m_nFireMode = 2;
				else if (m_bFMAutomatic)
					m_nFireMode = 3;
				break; 
			}
			case 2:
			{
				if (m_bFMAutomatic)
					m_nFireMode = 3;
				else m_nFireMode = 1; // change this to add safety functionality.
				break;
			}
			case 3:
			{
				if (m_bHasBurstGroup)
					m_nFireMode = 2;
				else m_nFireMode = 1; // change this to add safety functionality.
				break;
			}
			default:
			{
				m_nFireMode = 0; // on safe, it'll at least go into a known-valid state.
				break;
			}
			}
		}

		if (m_bUsesSwitchToChange && m_nFireMode != oldmode)
			// play weapon sound so you know the button has been pressed
			WeaponSound(EMPTY);

		if (m_bHKBurstType)
		{
			if (m_nFireMode == 2)// sets the counter to the correct amount (depending on firing mode
				m_nShotsLeft = m_nBurstRate;
			else
				m_nShotsLeft = m_nFireMode;
		}
		else {
			if (m_nFireMode == 2)// sets the counter to the correct amount (depending on firing mode
				m_nShotsLeft = m_nBurstToothState;
			else
				m_nShotsLeft = m_nFireMode;
		}

		// and set ready to false to keep from running through the list over and over
		m_bFMReady = false;
		//DevMsg("There are %d bullets being fired at a time...\\n", m_nFireMode);
	}

	return;
}

//-----------------------------------------------------------------------------
// Purpose: Make enough sound events to fill the estimated think interval
// returns: number of shots needed
//-----------------------------------------------------------------------------
int CHLMachineGun::WeaponSoundRealtime( WeaponSound_t shoot_type )
{
	int numBullets = 0;

	// ran out of time, clamp to current
	if (m_flNextSoundTime < gpGlobals->curtime)
	{
		m_flNextSoundTime = gpGlobals->curtime;
	}

	// make enough sound events to fill up the next estimated think interval
	float dt = clamp( m_flAnimTime - m_flPrevAnimTime, 0, 0.2 );
	if (m_flNextSoundTime < gpGlobals->curtime + dt)
	{
		WeaponSound( SINGLE_NPC, m_flNextSoundTime );
		m_flNextSoundTime += GetFireRate();
		numBullets++;
	}
	if (m_flNextSoundTime < gpGlobals->curtime + dt)
	{
		WeaponSound( SINGLE_NPC, m_flNextSoundTime );
		m_flNextSoundTime += GetFireRate();
		numBullets++;
	}

	return numBullets;
}




//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHLMachineGun::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	if (m_nBurstToothState <= 0)
		m_nBurstToothState = m_nBurstRate;

	// Debounce the recoiling counter
	if ( ( pOwner->m_nButtons & IN_ATTACK ) == false )
	{
		if (m_bHKBurstType)
		{
			if (m_nFireMode == 2)// sets the counter to the correct amount (depending on firing mode
				m_nShotsLeft = m_nBurstRate;
			else
				m_nShotsLeft = m_nFireMode;
		}
		else {
			if (m_nFireMode == 2)// sets the counter to the correct amount (depending on firing mode
				m_nShotsLeft = m_nBurstToothState;
			else
				m_nShotsLeft = m_nFireMode;
		}

		m_nShotsFired = 0;
	}
	
	if (!(pOwner->m_nButtons & IN_FIREMODE)) // when not pressing firemode button
		m_bFMReady = true; // set ready to true

	BaseClass::ItemPostFrame();
}

IMPLEMENT_SERVERCLASS_ST( CHLSelectFireMachineGun, DT_HLSelectFireMachineGun )
END_SEND_TABLE()

//=========================================================
//	>> CHLSelectFireMachineGun
//=========================================================
BEGIN_DATADESC( CHLSelectFireMachineGun )

	DEFINE_FIELD( m_iBurstSize,		FIELD_INTEGER ),
	DEFINE_FIELD( m_iFireMode,		FIELD_INTEGER ),
	
	// Function pinters
	DEFINE_FUNCTION( BurstThink ),

END_DATADESC()



float CHLSelectFireMachineGun::GetBurstCycleRate( void )
{
	// this is the time it takes to fire an entire 
	// burst, plus whatever amount of delay we want
	// to have between bursts.
	return 0.5f;
}

float CHLSelectFireMachineGun::GetFireRate( void )
{
	switch( m_iFireMode )
	{
	case FIREMODE_FULLAUTO:
		// the time between rounds fired on full auto
		return 0.1f;	// 600 rounds per minute = 0.1 seconds per bullet
		break;

	case FIREMODE_3RNDBURST:
		// the time between rounds fired within a single burst
		return 0.1f;	// 600 rounds per minute = 0.1 seconds per bullet
		break;

	default:
		return 0.1f;
		break;
	}
}

bool CHLSelectFireMachineGun::Deploy( void )
{
	// Forget about any bursts this weapon was firing when holstered
	m_iBurstSize = 0;
	return BaseClass::Deploy();
}


//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CHLSelectFireMachineGun::PrimaryAttack( void )
{
	if (m_bFireOnEmpty)
	{
		return;
	}
	switch( m_iFireMode )
	{
	case FIREMODE_FULLAUTO:
		BaseClass::PrimaryAttack();
		// Msg("%.3f\n", m_flNextPrimaryAttack.Get() );
		SetWeaponIdleTime( gpGlobals->curtime + 3.0f );
		break;

	case FIREMODE_3RNDBURST:
		m_iBurstSize = GetBurstSize();
		
		// Call the think function directly so that the first round gets fired immediately.
		BurstThink();
		SetThink( &CHLSelectFireMachineGun::BurstThink );
		m_flNextPrimaryAttack = gpGlobals->curtime + GetBurstCycleRate();
		m_flNextSecondaryAttack = gpGlobals->curtime + GetBurstCycleRate();

		// Pick up the rest of the burst through the think function.
		SetNextThink( gpGlobals->curtime + GetFireRate() );
		break;
	}

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner )
	{
		m_iPrimaryAttacks++;
		gamestats->Event_WeaponFired( pOwner, true, GetClassname() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CHLSelectFireMachineGun::SecondaryAttack( void )
{
	// change fire modes.

	switch( m_iFireMode )
	{
	case FIREMODE_FULLAUTO:
		//Msg( "Burst\n" );
		m_iFireMode = FIREMODE_3RNDBURST;
		WeaponSound(SPECIAL2);
		break;

	case FIREMODE_3RNDBURST:
		//Msg( "Auto\n" );
		m_iFireMode = FIREMODE_FULLAUTO;
		WeaponSound(SPECIAL1);
		break;
	}
	
	SendWeaponAnim( GetSecondaryAttackActivity() );

	m_flNextSecondaryAttack = gpGlobals->curtime + 0.3;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner )
	{
		m_iSecondaryAttacks++;
		gamestats->Event_WeaponFired( pOwner, false, GetClassname() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CHLSelectFireMachineGun::BurstThink( void )
{
	CHLMachineGun::PrimaryAttack();

	m_iBurstSize--;

	if( m_iBurstSize == 0 )
	{
		// The burst is over!
		SetThink(NULL);

		// idle immediately to stop the firing animation
		SetWeaponIdleTime( gpGlobals->curtime );
		return;
	}

	SetNextThink( gpGlobals->curtime + GetFireRate() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CHLSelectFireMachineGun::WeaponSound( WeaponSound_t shoot_type, float soundtime /*= 0.0f*/ )
{
	if (shoot_type == SINGLE)
	{
		switch( m_iFireMode )
		{
		case FIREMODE_FULLAUTO:
			BaseClass::WeaponSound( SINGLE, soundtime );
			break;

		case FIREMODE_3RNDBURST:
			if( m_iBurstSize == GetBurstSize() && m_iClip1 >= m_iBurstSize )
			{
				// First round of a burst, and enough bullets remaining in the clip to fire the whole burst
				BaseClass::WeaponSound( BURST, soundtime );
			}
			else if( m_iClip1 < m_iBurstSize )
			{
				// Not enough rounds remaining in the magazine to fire a burst, so play the gunshot
				// sounds individually as each round is fired.
				BaseClass::WeaponSound( SINGLE, soundtime );
			}

			break;
		}
		return;
	}

	BaseClass::WeaponSound( shoot_type, soundtime );
}

// BUGBUG: These need to be rethought
//-----------------------------------------------------------------------------
int CHLSelectFireMachineGun::WeaponRangeAttack1Condition( float flDot, float flDist )
{
	if (m_iClip1 <=0)
	{
		return COND_NO_PRIMARY_AMMO;
	}
	else if ( flDist < m_fMinRange1) 
	{
		return COND_TOO_CLOSE_TO_ATTACK;
	}
	else if (flDist > m_fMaxRange1) 
	{
		return COND_TOO_FAR_TO_ATTACK;
	}
	else if (flDot < 0.5)	// UNDONE: Why check this here? Isn't the AI checking this already?
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_RANGE_ATTACK1;
}

//-----------------------------------------------------------------------------
int CHLSelectFireMachineGun::WeaponRangeAttack2Condition( float flDot, float flDist )
{
	return COND_NONE; // FIXME: disabled for now

	// m_iClip2 == -1 when no secondary clip is used
	if ( m_iClip2 == 0 && UsesSecondaryAmmo() )
	{
		return COND_NO_SECONDARY_AMMO;
	}
	else if ( flDist < m_fMinRange2 )
	{
		// Don't return	COND_TOO_CLOSE_TO_ATTACK only for primary attack
		return COND_NONE;
	}
	else if (flDist > m_fMaxRange2 )
	{
		// Don't return COND_TOO_FAR_TO_ATTACK only for primary attack
		return COND_NONE;
	}
	else if ( flDot < 0.5 ) // UNDONE: Why check this here? Isn't the AI checking this already?
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_RANGE_ATTACK2;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHLSelectFireMachineGun::CHLSelectFireMachineGun( void )
{
	m_fMinRange1	= 65;
	m_fMinRange2	= 65;
	m_fMaxRange1	= 1024;
	m_fMaxRange2	= 1024;
	m_iFireMode		= FIREMODE_FULLAUTO;
}
