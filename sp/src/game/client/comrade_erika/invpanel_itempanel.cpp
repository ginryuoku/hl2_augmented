
#include "cbase.h"
#include "invpanel_itempanel.h"
#include "vgui_imagebutton.h"
#include "invpanel_contextmenu.h"

using namespace vgui;

ItemPanel::ItemPanel(Panel *parent, const char *name, const char *normalImage, int item_index, const char *mouseOverImage /* = NULL */, const char *mouseClickImage /* = NULL */, const char *pCmd /* = NULL */) : ImageButton(parent, name, normalImage, mouseOverImage, mouseClickImage, pCmd)
{
	m_iItemIndex = item_index;
	m_hContextMenu = nullptr;
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
