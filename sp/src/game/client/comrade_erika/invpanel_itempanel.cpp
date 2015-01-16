
#include "cbase.h"
#include "invpanel_itempanel.h"
#include "vgui_imagebutton.h"
#include "invpanel_contextmenu.h"

using namespace vgui;

ItemPanel::ItemPanel(Panel *parent, const char *name, const char *normalImage, int item_index, const char *mouseOverImage /* = NULL */, const char *mouseClickImage /* = NULL */, const char *pCmd /* = NULL */) : ImageButton(parent, name, normalImage, mouseOverImage, mouseClickImage, pCmd)
{
	m_iItemIndex = item_index;
	m_hContextMenu = nullptr;
	char buffer[40];

	vgui::IScheme* pScheme = vgui::scheme()->GetIScheme(GetScheme());
	vgui::HFont hFont = pScheme->GetFont("inv_labels");

	Q_snprintf(buffer, sizeof(buffer), "label%i", item_index);
	m_hLabel = new Label(this, buffer, buffer);
	m_hLabel->SetFont(hFont);
	m_hLabel->SetText(buffer);
		
	// We hard-code these values for now; this will eventually be calculated by the size of the image.
	int x = 2;
	int y = 64 - 2 - 8;

	m_hLabel->SetPos(x, y);
}

void ItemPanel::OnMousePressed(vgui::MouseCode code)
{
	//Tony; if nothing is selected, and they do a right click.. fake a left click - FIRST!
	if (code == MOUSE_RIGHT /* && GetSelectedItemsCount() <= 0 */)
		BaseClass::OnMousePressed(MOUSE_LEFT);
	
	//Tony; pass it through as normal also. if it's a real right click, it'll be okay.
	BaseClass::OnMousePressed(code);

	if (code == MOUSE_RIGHT /* && GetSelectedItemsCount() > 0 */)
	{
		m_hContextMenu = new CInvPanelContext(this, m_iItemIndex);
		m_hContextMenu->Activate();
	}
}

void vgui::ItemPanel::UpdateLabel(const char *contents)
{
	m_hLabel->SetText(contents);
	Repaint();
}
