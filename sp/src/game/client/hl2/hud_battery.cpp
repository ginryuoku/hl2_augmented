//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
// battery.cpp
//
// implementation of CHudBattery class
//
#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "hud_numericdisplay.h"
#include "iclientmode.h"

#include <vgui/ISurface.h>
#include "vgui_controls/AnimationController.h"
#include "vgui/ILocalize.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_BAT	-1

//-----------------------------------------------------------------------------
// Purpose: Displays suit power (armor) on hud
//-----------------------------------------------------------------------------
class CHudBattery : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudBattery, CHudNumericDisplay );

public:
	CHudBattery( const char *pElementName );
	void Init( void );
	void Reset( void );
	void VidInit( void );
	void OnThink( void );
	void MsgFunc_Battery(bf_read &msg );
	bool ShouldDraw();
	void Paint();
	
private:
	int		m_iBat;	
	int		m_iNewBat;
};

DECLARE_HUDELEMENT( CHudBattery );
DECLARE_HUD_MESSAGE( CHudBattery, Battery );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudBattery::CHudBattery( const char *pElementName ) : BaseClass(NULL, "HudSuit"), CHudElement( pElementName )
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_NEEDSUIT );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudBattery::Init( void )
{
	HOOK_HUD_MESSAGE( CHudBattery, Battery);
	Reset();
	m_iBat		= INIT_BAT;
	m_iNewBat   = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudBattery::Reset( void )
{
	SetLabelText(g_pVGuiLocalize->Find("#Valve_Hud_SUIT"));
	SetDisplayValue(m_iBat);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudBattery::VidInit( void )
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudBattery::ShouldDraw( void )
{
	bool bNeedsDraw = ( m_iBat != m_iNewBat ) || ( GetAlpha() > 0 );

	return ( bNeedsDraw && CHudElement::ShouldDraw() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudBattery::OnThink( void )
{
	if ( m_iBat == m_iNewBat )
		return;

	if ( !m_iNewBat )
	{
	 	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitPowerZero");
	}
	else if ( m_iNewBat < m_iBat )
	{
		// battery power has decreased, so play the damaged animation
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitDamageTaken");

		// play an extra animation if we're super low
		if ( m_iNewBat < 20 )
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitArmorLow");
		}
	}
	else
	{
		// battery power has increased (if we had no previous armor, or if we just loaded the game, don't use alert state)
		if ( m_iBat == INIT_BAT || m_iBat == 0 || m_iNewBat >= 20)
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitPowerIncreasedAbove20");
		}
		else
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitPowerIncreasedBelow20");
		}
	}

	m_iBat = m_iNewBat;

	SetDisplayValue(m_iBat);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudBattery::MsgFunc_Battery( bf_read &msg )
{
	m_iNewBat = msg.ReadShort();
}

void CHudBattery::Paint(void)
{
	int chunkCount = 1;
	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
	if (local)
		chunkCount = local->m_Local.m_iArmorUpgrades + 1;

	bool transition_chunk = false;
	int transition_chunk_alpha = 15 + (12 * (m_iBat % 20));
	int enabledChunks = (int)(m_iBat / 20);
	if (m_iBat % 20)
	{
		transition_chunk = true;
	}

	vgui::surface()->DrawSetColor(Color(255, 220, 0, 255));
	int xpos = 8, ypos = 8;

	for (int i = 0; i < enabledChunks; ++i)
	{
		vgui::surface()->DrawFilledRect(xpos, ypos, xpos + 10, ypos + 10);
		xpos += 12;
	}

	if (transition_chunk)
	{
		vgui::surface()->DrawSetColor(Color(255, 220, 0, transition_chunk_alpha));
		vgui::surface()->DrawFilledRect(xpos, ypos, xpos + 10, ypos + 10);
	}

	vgui::surface()->DrawSetColor(Color(255, 220, 0, 15));
	for (int i = enabledChunks; i < chunkCount; ++i)
	{
		vgui::surface()->DrawFilledRect(xpos, ypos, xpos + 10, ypos + 10);
		xpos += 12;
	}

	BaseClass::Paint();
}