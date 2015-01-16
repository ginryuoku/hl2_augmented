//The following include files are necessary to allow your MyPanel.cpp to compile.
#include "cbase.h"
#include "vgui_grid.h"
#include <vgui_controls/Label.h>
#include <vgui_controls/Panel.h>

#include "invpanel.h"
#include "invpanel_itempanel.h"
#include "IGameUIFuncs.h" // for key bindings

#include "tier0/memdbgon.h"

// Constructor: Initializes the Panel
CInvPanel::CInvPanel(IViewPort *pViewPort) : BaseClass(NULL, PANEL_INVENTORY)
{
	m_pViewPort = pViewPort;
	m_iInvKey = BUTTON_CODE_INVALID;

	// initialize dialog
	SetTitle("", true);

	// load the new scheme early!!
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/sourceschemebase.res", "SourceScheme"));
	SetMoveable(false);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional(true);
	
	int num_cols = scheme()->GetProportionalScaledValue(600) / 72;
	Msg("Number of proposed columns: %i, ScreenWidth reports: %i\n", num_cols, ScreenWidth());
	InvSubPanel = new ControlGrid(this, "InvSubPanel");
	InvSubPanel->SetNumColumns(num_cols);
	InvSubPanel->SetShouldDrawLabels(false);
	InvSubPanel->SetProportional(true);
	InvSubPanel->SetBounds(scheme()->GetProportionalScaledValue(20), scheme()->GetProportionalScaledValue(20),
		scheme()->GetProportionalScaledValue(600), scheme()->GetProportionalScaledValue(400));

	LoadControlSettings("resource/UI/invpanel.res");
	InvalidateLayout();
	SetSize(ScreenWidth(), ScreenHeight());
	
	char buffer[40];

	for (int i = 0; i < MAX_INVENTORY; i++)
	{
		//Create Image
		Q_snprintf(buffer, sizeof(buffer), "image%i", i);
		ItemPanel* itempanel = new ItemPanel(this, buffer, "inv/0", i);
		itempanel->SetProportional(false);
		itempanel->SetSize(64, 64);
		itempanel->SetMinimumSize(64, 64);
		itempanel->SetPaintBackgroundEnabled(true);
		itempanel->SetPaintBackgroundType(2);

		//Add Image to PanelListPanel
		InvSubPanel->AddItem(itempanel);
	}

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
		
	DevMsg("InvPanel has been constructed\n");
}

void CInvPanel::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow)
		return;
	
	if (bShow)
	{
		Activate();

		SetMouseInputEnabled(true);

		if ( m_iInvKey == BUTTON_CODE_INVALID )
		{
			m_iInvKey = gameuifuncs->GetButtonCodeForBind( "inventory" );
		}
		engine->ClientCmd_Unrestricted("gameui_preventescapetoshow\n");
	}
	else
	{
		SetVisible(false);
		SetMouseInputEnabled(false);
		engine->ClientCmd_Unrestricted("gameui_allowescapetoshow\n");
	}

}

void CInvPanel::BeginUpdates()
{
	CBasePlayer *pPlayer = ToBasePlayer(UTIL_PlayerByIndex(1));
	char buffer[40];
	if (pPlayer)
	{
		for (int i = 0; i < MAX_INVENTORY; ++i)
		{
			if (pPlayer->m_pInventory.GetItemDirtiness(i))
			{
				// First, grab the image panel.
				Panel* panel = InvSubPanel->GetItemPanel(i);
				if (panel)
				{
					ItemPanel* itempanel = dynamic_cast<ItemPanel*>(panel);
					if (itempanel)
					{
						Q_snprintf(buffer, sizeof(buffer), "inv/%i", pPlayer->m_pInventory.GetItemID(i));
						itempanel->ChangeNormalImage(buffer);
						itempanel->SetNormalImage();
						if (pPlayer->m_pInventory.GetItemMaxCapacity(i) <= 1)
						{
							Q_snprintf(buffer, sizeof(buffer), "");
							itempanel->UpdateLabel(buffer);
						}
						else
						{
							Q_snprintf(buffer, sizeof(buffer), "%i/%i", pPlayer->m_pInventory.GetItemCapacity(i), pPlayer->m_pInventory.GetItemMaxCapacity(i));
							itempanel->UpdateLabel(buffer);
						}
					}
				}

				// Great. Now to 'clean' the item so that we don't waste time updating it.
				pPlayer->m_pInventory.ItemIsClean(i);
			}
		}
	}

	return;
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

void CInvPanel::OnClose()
{
	engine->ClientCmd_Unrestricted("gameui_allowescapetoshow\n");
	BaseClass::OnClose();
}

void CInvPanel::OnKeyCodeTyped(KeyCode code)
{
	if (code == KEY_ESCAPE ||
		code == m_iInvKey) 
	{
		OnClose();
	}
	else
	{
		BaseClass::OnKeyCodeTyped(code);
	}
}
