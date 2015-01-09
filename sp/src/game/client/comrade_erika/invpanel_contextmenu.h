#ifndef invpanel_contextmenu_h__
#define invpanel_contextmenu_h__

#include "vgui_contextmenu.h"

class CInvPanelContext : public CContextMenuBase
{
public:
	DECLARE_CLASS_SIMPLE(CInvPanelContext, CContextMenuBase);

	CInvPanelContext(vgui::Panel *pParent, int itemindex);
	virtual ~CInvPanelContext();
	virtual void OnCommand(const char *command);
	virtual int NumButtons() const { return 3; }
private:
	int m_iItemIndex;
};
#endif // invpanel_contextmenu_h__
