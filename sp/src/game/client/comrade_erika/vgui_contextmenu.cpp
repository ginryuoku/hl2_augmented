///////////////////////////////////////
// Source for the VGUI Context Menu. Original code from Tony Sergi.
///////////////////////////////////////

#include "cbase.h"
#include <vgui_controls/EditablePanel.h>
#include "vgui_contextmenu.h"

using namespace vgui;

void CContextMenuBase::Activate()
{
	MoveToFront();
	RequestFocus();
	SetVisible(true);
	SetEnabled(true);

	MakePopup(false, false);

	//Tony; taken PlaceUnderCursor, and just do it's shit in here!
	// get cursor position, this is local to this text edit window
	int cursorX, cursorY;
	input()->GetCursorPos(cursorX, cursorY);

	// relayout the menu immediately so that we know it's size
	InvalidateLayout(true);
	int w, h;
	GetSize(w, h);

	// work out where the cursor is and therefore the best place to put the frame
	int sw, sh;
	surface()->GetScreenSize(sw, sh);

	// Try to center it first
	int x, y;
	x = cursorX - (w / 2);
	y = cursorY - (h / 2);

	// Clamp to various sides
	if (x + w > sw)
	{
		x = sw - w;
	}
	if (y + h > sh)
	{
		y = sh - h;
	}
	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}

	SetPos(x, y);
}

void CContextMenuBase::OnThink()
{
	BaseClass::OnThink();

	int wide, tall, x, y, cx, cy;

	input()->GetCursorPos(cx, cy);

	GetBounds(x, y, wide, tall);

	//    DevMsg("\nBounds: X:%i U:%i Wide: %i Tall: %i\n", x, y, wide, tall );
	//    DevMsg("Cursor: X: %i y: %i\n", cx, cy );

	//Tony; now close us!!
	if (cx < x || cx > x + wide || cy < y || cy > y + tall)
		Close();

}

void CContextMenuBase::Close()
{
	SetVisible(false);
	MarkForDeletion();
}
void CContextMenuBase::PerformLayout()
{
	BaseClass::PerformLayout();

	//Tony; layout context menu buttons
	int wide, tall, ypos, xpos;

	//Tony; take visibility into account for ALL of this!!
	//find the widest button
	int widest = 0;
	for (int i = 0; i < NumButtons(); i++)
	{
		if (!m_pContextButtons[i]->IsVisible())
			continue;
		int w, t;
		m_pContextButtons[i]->GetContentSize(w, t);
		if (w > widest)
			widest = w;
	}
	//min
	int minWidth = scheme()->GetProportionalNormalizedValue(64);
	if (widest < minWidth)
		widest = minWidth;

	//Tony; now that we know the widest size, lay them out.
	m_pContextButtons[0]->GetSize(wide, tall);

	ypos = scheme()->GetProportionalNormalizedValue(8);    //we start at 2.
	xpos = scheme()->GetProportionalNormalizedValue(4);    //xpos is always indented by 2.

	//Tony; use visibleButtons to determine the height
	int visibleButtons = 0;
	for (int i = 0; i < NumButtons(); i++)
	{
		if (!m_pContextButtons[i]->IsVisible())
			continue;
		visibleButtons++;
		m_pContextButtons[i]->SetBounds(xpos, ypos, widest, tall);
		ypos += tall;
	}

	SetSize(scheme()->GetProportionalNormalizedValue(8) + widest, scheme()->GetProportionalNormalizedValue(16) + (tall * visibleButtons));
}
