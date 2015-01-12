//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "c_weapon__stubs.h"
#include "basehlcombatweapon_shared.h"
#include "c_basehlcombatweapon.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

STUB_WEAPON_CLASS( cycler_weapon, WeaponCycler, C_BaseCombatWeapon );

STUB_WEAPON_CLASS( weapon_binoculars, WeaponBinoculars, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_bugbait, WeaponBugBait, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_flaregun, Flaregun, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_annabelle, WeaponAnnabelle, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_gauss, WeaponGaussGun, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_cubemap, WeaponCubemap, C_BaseCombatWeapon );
STUB_WEAPON_CLASS( weapon_alyxgun, WeaponAlyxGun, C_HLSelectFireMachineGun );
STUB_WEAPON_CLASS( weapon_citizenpackage, WeaponCitizenPackage, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_citizensuitcase, WeaponCitizenSuitcase, C_WeaponCitizenPackage );

#ifndef HL2MP
STUB_WEAPON_CLASS( weapon_ar2, WeaponAR2, C_HLMachineGun );
STUB_WEAPON_CLASS( weapon_frag, WeaponFrag, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_rpg, WeaponRPG, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_pistol, WeaponPistol, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_shotgun, WeaponShotgun, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_smg1, WeaponSMG1, C_HLSelectFireMachineGun );
STUB_WEAPON_CLASS( weapon_357, Weapon357, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_crossbow, WeaponCrossbow, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_slam, Weapon_SLAM, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_crowbar, WeaponCrowbar, C_BaseHLBludgeonWeapon );
#ifdef HL2_EPISODIC
STUB_WEAPON_CLASS( weapon_hopwire, WeaponHopwire, C_BaseHLCombatWeapon );
//STUB_WEAPON_CLASS( weapon_proto1, WeaponProto1, C_BaseHLCombatWeapon );
#endif
#ifdef HL2_LOSTCOAST
STUB_WEAPON_CLASS( weapon_oldmanharpoon, WeaponOldManHarpoon, C_WeaponCitizenPackage );
#endif
#endif

STUB_WEAPON_CLASS( weapon_hgun_92fs, Weapon92FS, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS(weapon_hgun_uspmatch9, WeaponUSPMatch9, C_BaseHLCombatWeapon);
STUB_WEAPON_CLASS(weapon_hgun_fiveseven, WeaponFiveSeven, C_BaseHLCombatWeapon);

STUB_WEAPON_CLASS( weapon_mag_mr96, WeaponMR96, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS(weapon_mag_deagle357, WeaponDEagle357, C_BaseHLCombatWeapon);

STUB_WEAPON_CLASS( weapon_shot_870ae, Weapon870AE, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_shot_spas12, WeaponSPAS12, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS(weapon_shot_striker12, WeaponStriker12, C_BaseHLCombatWeapon);


STUB_WEAPON_CLASS( weapon_smg_mp5a2, WeaponMP5A2, C_HLMachineGun );

STUB_WEAPON_CLASS(weapon_pdw_mp7a1, WeaponMP7A1, C_HLMachineGun);
STUB_WEAPON_CLASS( weapon_pdw_p90, WeaponP90, C_HLMachineGun );

STUB_WEAPON_CLASS( weapon_arifle_m4a1, WeaponM4A1, C_HLMachineGun );
STUB_WEAPON_CLASS(weapon_arifle_vltorsbr, WeaponVltorSBR, C_HLMachineGun);

STUB_WEAPON_CLASS( weapon_launcher_m79, WeaponLauncherM79, C_BaseHLCombatWeapon );
STUB_WEAPON_CLASS( weapon_dissolver, WeaponDissolver, C_BaseHLCombatWeapon );

