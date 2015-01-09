
#include "cbase.h"
#include <vgui_controls/Button.h>
#include "invpanel_contextmenu.h"

using namespace vgui;

CInvPanelContext::CInvPanelContext(vgui::Panel *pParent, int itemindex) : BaseClass(pParent)
{
	m_iItemIndex = itemindex;
	
	m_pContextButtons[0] = new Button(this, "ButtonDropItem", "#CE_ButtonDropItem");
	m_pContextButtons[1] = new Button(this, "ButtonSellItem", "#CE_ButtonSellItem");
	m_pContextButtons[2] = new Button(this, "ButtonUseItem", "#CE_ButtonUseItem");

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/sourceschemebase.res", "SourceScheme"));
	LoadControlSettings("Resource/UI/invbutton.res");

	SetPaintBackgroundType(0);
}

CInvPanelContext::~CInvPanelContext()
{
}

void CInvPanelContext::OnCommand(const char *command)
{
	//Tony; modify all commands to have wsid attached to the end

	// Michi: repurpose this for including the item index.
	char szModifiedCommand[256];
	Q_snprintf(szModifiedCommand, 256, "%s %i", command, m_iItemIndex);
	
	engine->ServerCmd(szModifiedCommand);

	Close();
}

