#include "cbase.h"
#include "gui_inventory.h"

using namespace Awesomium;

#ifdef PostMessage
#undef PostMessage
#endif



void VInvPanel::OnDocumentReady(WebView* caller, const WebURL& url)
{
	JSValue result = caller->CreateGlobalJavascriptObject(WSLit("InvPanel"));

	JSObject &InvPanelObject = result.ToObject();
	InvPanelObject.SetCustomMethod(WSLit("ReturnToGame"), false);

	caller->ExecuteJavascript(WSLit("AwesomiumInitialized()"), WSLit(""));
}

void VInvPanel::OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args)
{
	if (method_name == WSLit("ReturnToGame"))
	{
		BaseClass::GetParent()->PostMessage(BaseClass::GetParent(), new KeyValues("command", "command", EXIT_COMMAND));
	}
}

VInvPanel::VInvPanel( vgui::Panel *parent, const char *panelName ) : VAwesomium(parent, panelName)
{
	OpenURL("https://encrypted.google.com");
}


