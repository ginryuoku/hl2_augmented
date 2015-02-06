//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
// Health.cpp
//
// implementation of CHudHealth class
//
#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>

#include <vgui/ILocalize.h>

using namespace vgui;

#include "hudelement.h"
#include "hud_numericdisplay.h"

#include "convar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_HEALTH -1

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CHudHealth : public CHudElement, public CHudNumericDisplay
{
	DECLARE_CLASS_SIMPLE( CHudHealth, CHudNumericDisplay );

public:
	CHudHealth( const char *pElementName );
	virtual void Init( void );
	virtual void VidInit( void );
	virtual void Reset( void );
	virtual void OnThink();
			void MsgFunc_Damage( bf_read &msg );
	virtual void Paint(void);

private:
	// old variables
	int		m_iHealth;
	
	int		m_bitsDamage;
};	

DECLARE_HUDELEMENT( CHudHealth );
DECLARE_HUD_MESSAGE( CHudHealth, Damage );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudHealth::CHudHealth( const char *pElementName ) : CHudElement( pElementName ), CHudNumericDisplay(NULL, "HudHealth")
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::Init()
{
	HOOK_HUD_MESSAGE( CHudHealth, Damage );
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::Reset()
{
	m_iHealth		= INIT_HEALTH;
	m_bitsDamage	= 0;

	wchar_t *tempString = g_pVGuiLocalize->Find("#Valve_Hud_HEALTH");

	if (tempString)
	{
		SetLabelText(tempString);
	}
	else
	{
		SetLabelText(L"HEALTH");
	}
	SetDisplayValue(m_iHealth);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::VidInit()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::OnThink()
{
	int newHealth = 0;
	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
	if ( local )
	{
		// Never below zero
		newHealth = MAX( local->GetHealth(), 0 );
	}

	// Only update the fade if we've changed health
	if ( newHealth == m_iHealth )
	{
		return;
	}

	m_iHealth = newHealth;

	if (local)
	{
		if (m_iHealth >= local->GetMaxHealth() / 5)
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedAbove20");
		}
		else if (m_iHealth > 0)
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedBelow20");
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthLow");
		}
	}


	SetDisplayValue(m_iHealth);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::MsgFunc_Damage( bf_read &msg )
{

	int armor = msg.ReadByte();	// armor
	int damageTaken = msg.ReadByte();	// health
	long bitsDamage = msg.ReadLong(); // damage bits
	bitsDamage; // variable still sent but not used

	Vector vecFrom;

	vecFrom.x = msg.ReadBitCoord();
	vecFrom.y = msg.ReadBitCoord();
	vecFrom.z = msg.ReadBitCoord();

	// Actually took damage?
	if ( damageTaken > 0 || armor > 0 )
	{
		if ( damageTaken > 0 )
		{
			// start the animation
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthDamageTaken");
		}
	}
}

void CHudHealth::Paint(void)
{
	int chunkCount = 5;
	int segment = 10;
	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
	if (local) 
	{
		chunkCount = local->m_Local.m_iHealthUpgrades + 5;
		segment = local->HealthSegmentValue();
	}

	// Clamp the size of the health bar
	if (chunkCount > 15)
	{
		chunkCount = 15;
	}
	bool transition_chunk = false;
	int transition_chunk_alpha = 15 + (((m_iHealth % segment) * 240) / segment);
	int enabledChunks = (int)(m_iHealth / segment);
	if (enabledChunks > 15)
	{
		enabledChunks = 15;
	}
	if (m_iHealth % segment && enabledChunks < 15)
	{
		transition_chunk = true;
	}

#define BOX_SIZE 10
#define BOX_PADDING 2
	vgui::surface()->DrawSetColor(Color(255, 220, 0, 255));
	int xpos = vgui::scheme()->GetProportionalScaledValue(50);
	int ypos = vgui::scheme()->GetProportionalScaledValue(6);

	for (int i = 0; i < enabledChunks; ++i)
	{
		vgui::surface()->DrawFilledRect(xpos, ypos, xpos + vgui::scheme()->GetProportionalScaledValue(BOX_SIZE), ypos + vgui::scheme()->GetProportionalScaledValue(BOX_SIZE));
		xpos += vgui::scheme()->GetProportionalScaledValue(BOX_PADDING + BOX_SIZE);
	}

	if (transition_chunk)
	{
		vgui::surface()->DrawSetColor(Color(255, 220, 0, transition_chunk_alpha));
		vgui::surface()->DrawFilledRect(xpos, ypos, xpos + vgui::scheme()->GetProportionalScaledValue(BOX_SIZE), ypos + vgui::scheme()->GetProportionalScaledValue(BOX_SIZE));
	}

	vgui::surface()->DrawSetColor(Color(255, 220, 0, 15));
	for (int i = enabledChunks; i < chunkCount; ++i)
	{
		vgui::surface()->DrawFilledRect(xpos, ypos, xpos + vgui::scheme()->GetProportionalScaledValue(BOX_SIZE), ypos + vgui::scheme()->GetProportionalScaledValue(BOX_SIZE));
		xpos += vgui::scheme()->GetProportionalScaledValue(BOX_PADDING + BOX_SIZE);
	}

	BaseClass::Paint();
}
