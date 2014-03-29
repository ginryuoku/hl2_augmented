//The following include files are necessary to allow your MyPanel.cpp to compile.
#include "cbase.h"
#include "invpanel.h"

#include "IGameUIFuncs.h" // for key bindings

#include "tier0/memdbgon.h"

#define MAX_HTML_UPDATES 5

// Constructor: Initializes the Panel
CInvPanel::CInvPanel(IViewPort *pViewPort) : BaseClass(NULL, PANEL_INVENTORY)
{
	m_pViewPort = pViewPort;
	m_iInvKey = BUTTON_CODE_INVALID;

	// initialize dialog
	SetTitle("", true);

	// load the new scheme early!!
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	SetMoveable(false);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional(true);

	LoadControlSettings("resource/UI/invpanel.res");
	InvalidateLayout();
	SetSize(ScreenWidth(),ScreenHeight());

	m_pInvPanel = new CE_HTML(this, "InvPanelHTML", true);
	m_pInvPanel->SetPos(0,0);
	m_pInvPanel->SetSize(ScreenWidth(),ScreenHeight());
	
	update_counter = 0;
	DevMsg("InvPanel has been constructed\n");
}

void CInvPanel::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow)
	{
		return;
	}
	
	if (bShow)
	{
		Activate();

		SetMouseInputEnabled(true);

		if ( m_iInvKey == BUTTON_CODE_INVALID )
		{
			m_iInvKey = gameuifuncs->GetButtonCodeForBind( "inventory" );
		}

		// This is evil and cannot go into production as-is. We need a better way to push HTML to CEF.
		CUtlString m_URL;
		m_URL.Append(engine->GetGameDirectory());
		m_URL.Append("/html/invpanel/index.html");
		DevMsg("URL: %s\n", m_URL.Get());

		m_pInvPanel->OpenURL(m_URL.Get(), NULL, true);
		for (int i = 0; i < MAX_INVENTORY; ++i)
		{
			CBasePlayer *pPlayer = ToBasePlayer(UTIL_PlayerByIndex(1));
			if (pPlayer)
			{
				pPlayer->m_pInventory.ItemIsDirty(i);
			}

		}
		update_counter = 0;
	}
	else
	{
		SetVisible(false);
		SetMouseInputEnabled(false);
	}

}

void CInvPanel::BeginUpdates()
{
	CBasePlayer *pPlayer = ToBasePlayer(UTIL_PlayerByIndex(1));
	if (pPlayer)
	{
		for (int i = 0; i < MAX_INVENTORY; ++i)
		{
			if (pPlayer->m_pInventory.GetItemDirtiness(i))
			{
				int id = pPlayer->m_pInventory.GetItemID(i);
				int cap = pPlayer->m_pInventory.GetItemCapacity(i);
				int maxcap = pPlayer->m_pInventory.GetItemMaxCapacity(i);

				CUtlString m_buildJSString;
				m_buildJSString.Format("UpdateObject(InvArray, %d, %d, %d, %d);", i, id, cap, maxcap);
				Msg("Sending command: %s\n", m_buildJSString.Get());
				m_pInvPanel->RunJavascript(m_buildJSString.String());
				pPlayer->m_pInventory.ItemIsClean(i);

				++update_counter;
			}

			if (update_counter >= MAX_HTML_UPDATES)
			{
				update_counter = 0;
				break;
			}

		}
	}
}

void CInvPanel::Update()
{
	return;
}

void CInvPanel::OnThink()
{
	BeginUpdates();
	BaseClass::OnThink();
}

void CInvPanel::OnCommand(const char *command)
{
	if ( Q_strcmp( command, "vguicancel") )
	{
		engine->ClientCmd( const_cast<char *>( command ) );
	}
	Close();
	gViewPortInterface->ShowBackGround( false );
	BaseClass::OnCommand(command);
}
