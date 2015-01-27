"Resource/UI/BuyAmmo.res"
{
	"BuySubMenu"
	{
		"ControlName"		"WizardSubPanel"
		"fieldName"		"BuySubMenu"
		"xpos"			"0"
		"ypos"			"0"
		"wide"			"640"
		"tall"			"480"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
	}

	"Title"
	{
		"ControlName"		"Label"
		"fieldName"		"Title"
				"xpos"		"52"
		"ypos"		"22"
		"wide"		"500"
		"tall"		"48"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"labelText"		"#CE_BuyAmmo_Label"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"font"		"MenuTitle"
		"wrap"		"0"
	}

	"ItemInfo"
	{
		"ControlName"		"Panel"
		"fieldName"		"ItemInfo"
		"xpos"		"244"
		"ypos"		"116"
		"wide"		"400"
		"tall"		"380"
		"autoResize"		"3"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
	}

	"lightammo"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"lightammo"
		"xpos"		"52"
		"ypos"		"116"
		"wide"		"170"
		"tall"		"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#CE_LightAmmo"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"command"		"resource/ui/buylightammo.res"
		"cost"			"650"
	}
	"556"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"556"
				"xpos"		"52"
		"xpos"		"0"
		"ypos"		"148"
		"wide"		"170"
		"tall"		"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#item_ammo_ar2_large"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"command"		"buy item_ammo_ar2_large"
		"cost"			"500"
	}
    "762nato"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"762nato"
				"xpos"		"52"
		"xpos"		"0"
		"ypos"		"180"
		"wide"		"170"
		"tall"		"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#CE_762nato"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"command"		"buy item_ammo_762nato"
		"cost"			"500"
	}
	"xbow"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"xbow"
				"xpos"		"52"
		"xpos"		"0"
		"ypos"		"212"
		"wide"		"170"
		"tall"		"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#item_ammo_crossbow"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"command"		"buy item_ammo_crossbow"
		"cost"			"21"
	}
    
	"40mm"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"40mm"
				"xpos"		"52"
		"xpos"		"0"
		"ypos"		"244"
		"wide"		"170"
		"tall"		"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#item_ammo_smg1_grenade"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"command"		"buy item_ammo_smg1_grenade"
		"cost"			"100"
	}
    "buckshot"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"buckshot"
		"xpos"			"52"
		"ypos"			"276"
		"wide"			"170"
		"tall"			"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#item_box_buckshot"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"command"		"buy item_box_buckshot"
		"cost"			"25"
	}
	"dissolver"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"dissolver"
		"xpos"			"52"
		"ypos"			"308"
		"wide"			"170"
		"tall"			"20"
		"autoResize"	"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#item_ammo_ar2_altfire"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"	"0"
		"command"		"buy item_ammo_ar2_altfire"
		"cost"			"25"
	}
	"CancelButton"
	{
		"ControlName"		"MouseOverPanelButton"
		"fieldName"		"CancelButton"
				"xpos"		"52"
		"ypos"		"380"
		"wide"		"170"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"labelText"		"#Cstrike_Cancel"
		"textAlignment"		"west"
		"dulltext"		"0"
		"brighttext"		"0"
		"Command"		"vguicancel"
		"Default"		"1"
	}
	"MarketSticker"
	{
		"ControlName"	"ImagePanel"
		"fieldName"	"MarketSticker"
		"image"			"gfx/vgui/market_sticker_category"
		"xpos"	"508"
		"ypos"	"250"
		"wide"	"84"
		"tall"	"84"
		"autoresize"	"2"
		"pinCorner"	"0"
		"visible"	"1"
		"enabled"	"1"
		"tabPosition"	"0"
		"vpos"	"-1"
		"scaleImage"	"1"
	}
}
