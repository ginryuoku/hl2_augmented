///////////////////////////////////////
// Header for the VGUI Context Menu. Original code by Tony Sergi.
///////////////////////////////////////


#ifndef vgui_contextmenu_h__
#define vgui_contextmenu_h__

#include "cbase.h"
#include <vgui/VGUI.h>
#include <vgui/IInput.h>
#include <vgui/ISurface.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/EditablePanel.h>

//Tony; if any context menu needs more than this many buttons, this number has to be increased!
#define CONTEXTMENU_MAX_BUTTONS 10
class CContextMenuBase : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CContextMenuBase, vgui::EditablePanel);

	CContextMenuBase(vgui::Panel *parent) : vgui::EditablePanel(parent, NULL)
	{
		m_pOuter = parent;
	}

	virtual ~CContextMenuBase()
	{
	}

	virtual void ApplySchemeSettings(vgui::IScheme *pScheme)
	{
		BaseClass::ApplySchemeSettings(pScheme);
	}
	//Tony; the activate function positions us under the mouse cursor.
	virtual void Activate();
	virtual void Close();

	virtual void OnThink();
	virtual void PerformLayout();
	virtual int NumButtons() const { return 0; }


	virtual void OnCommand(const char *command)
	{
		m_pOuter->OnCommand(command);
		//Tony; as soon as any command is executed on this context menu, just close it.
		Close();
	}

private:
	vgui::Panel *m_pOuter;
protected:
	vgui::Button *m_pContextButtons[CONTEXTMENU_MAX_BUTTONS];
};
#endif // vgui_contextmenu_h__
