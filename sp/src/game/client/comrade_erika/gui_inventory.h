#ifndef INVPANEL_H
#define INVPANEL_H

#include "VAwesomium.h"

#define EXIT_COMMAND "ExitMyVAwesomium"

#include <vgui/VGUI.h>
#include <vgui_controls/Panel.h>

class VInvPanel : public VAwesomium
{
	DECLARE_CLASS_SIMPLE(VInvPanel, VAwesomium);
public:
	VInvPanel(vgui::Panel *parent, const char *panelName);

	virtual void OnDocumentReady(Awesomium::WebView* caller, const Awesomium::WebURL& url);

	virtual void OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args);
};

#endif