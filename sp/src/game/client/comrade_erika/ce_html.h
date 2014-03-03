//
//  ce_html.h
//  games
//
//  Created by Michelle Darcy on 3/3/14.
//
//

#ifndef __games__ce_html__
#define __games__ce_html__

#include "cbase.h"
#include <vgui_controls/Panel.h>
#include <vgui_controls/HTML.h>

using namespace vgui;

class CE_HTML : public HTML
{
	DECLARE_CLASS_SIMPLE( CE_HTML, HTML );
	
public:
	CE_HTML(Panel *parent,const char *name, bool allowJavaScript = false, bool bPopupWindow = false) : HTML( parent, name, allowJavaScript, bPopupWindow )
	{
		
	}
	void OnKeyCodeTyped(vgui::KeyCode code);
};

#endif /* defined(__games__ce_html__) */
