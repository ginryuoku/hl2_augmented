//
//  ce_html.cpp
//  games
//
//  Created by Michelle Darcy on 3/3/14.
//
//	This control is altered for game-specific purposes.
//  In particular, we need to be able to capture key state.

#include "cbase.h"
#include "ce_html.h"
#include "IGameUIFuncs.h" // for key bindings

using namespace vgui;

void CE_HTML::OnKeyCodeTyped(vgui::KeyCode code)
{
	switch( code )
	{
		case KEY_ESCAPE:
		{
			engine->ClientCmd( "vguicancel" );
		}

	}
	
	int m_iInvKey = gameuifuncs->GetButtonCodeForBind( "inventory" );
	
	if (code == m_iInvKey) {
		engine->ClientCmd( "vguicancel" );
	}
	
	HTML::OnKeyCodeTyped(code);
}
