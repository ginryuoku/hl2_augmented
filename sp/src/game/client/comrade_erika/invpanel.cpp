//The following include files are necessary to allow your MyPanel.cpp to compile.
#include "cbase.h"
#include "vgui_grid.h"
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Label.h>

#include "invpanel.h"

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
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	SetMoveable(false);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional(true);
	
	InvSubPanel = new ControlGrid(this, "InvSubPanel");
	InvSubPanel->SetNumColumns(15);
	InvSubPanel->SetShouldDrawLabels(true);
	InvSubPanel->SetProportional(true);
	InvSubPanel->SetMinimumSize(600, 400);

	LoadControlSettings("resource/UI/invpanel.res");
	InvalidateLayout();
	SetSize(ScreenWidth(), ScreenHeight());
	
	char buffer[40];

	for (int i = 0; i < MAX_INVENTORY; i++)
	{
		//Create Image
		Q_snprintf(buffer, sizeof(buffer), "image%i", i);
		ImagePanel* imagePanel = new ImagePanel(this, buffer);
		imagePanel->SetImage(scheme()->GetImage("inv/0", false));
		imagePanel->SetMinimumSize(32, 32);
		imagePanel->SetProportional(true);

		//Create Label
		Q_snprintf(buffer, sizeof(buffer), "label%i", i);
		Label* label = new Label(this, buffer, buffer);
		label->SetText(buffer);

		//Add Label and Image to PanelListPanel
		InvSubPanel->AddItem(imagePanel, label);
	}

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
		
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
					ImagePanel* imagepanel = dynamic_cast<ImagePanel*>(panel);
					if (imagepanel)
					{
						Q_snprintf(buffer, sizeof(buffer), "inv/%i", pPlayer->m_pInventory.GetItemID(i));
						imagepanel->SetImage(scheme()->GetImage(buffer, false));
					}
				}
				// Next, grab the label panel.
				Panel* panel2 = InvSubPanel->GetItemLabel(i);
				if (panel2)
				{
					Label* label = dynamic_cast<Label*>(panel2);
					if (label)
					{
						Q_snprintf(buffer, sizeof(buffer), "%i/%i", pPlayer->m_pInventory.GetItemCapacity(i), pPlayer->m_pInventory.GetItemMaxCapacity(i));
						label->SetText(buffer);
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
