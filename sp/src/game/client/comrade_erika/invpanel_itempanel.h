#ifndef invpanel_itempanel_h__
#define invpanel_itempanel_h__

#include "invpanel_contextmenu.h"
#include "vgui_imagebutton.h"

namespace vgui
{
	class ItemPanel : public vgui::ImageButton
	{
		DECLARE_CLASS_SIMPLE(ItemPanel, ImageButton);
	public:
		ItemPanel(Panel *parent, const char *name, const char *normalImage, int item_index, const char *mouseOverImage = NULL, const char *mouseClickImage = NULL, const char *pCmd = NULL);
		virtual void OnMousePressed(vgui::MouseCode code);

	private:
		int m_iItemIndex;
		bool hasMouseClickImage;
		CInvPanelContext *m_hContextMenu;
	};
}

#endif // invpanel_itempanel_h__
