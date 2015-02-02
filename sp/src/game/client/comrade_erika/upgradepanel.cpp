//The following include files are necessary to allow this to compile.
#include "cbase.h"
#include "upgradepanel.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>

#include "c_basehlplayer.h"

#define MAX_AUGS 5  // Defines the number of augmentations
#define MAX_STATS 3 // Defines the number of stats tracked by this panel

//CUpgradePanel class
class CUpgradePanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CUpgradePanel, vgui::Frame);
	//CUpgradePanel : This Class / vgui::Frame : BaseClass

	CUpgradePanel(vgui::VPANEL parent); 	// Constructor
	~CUpgradePanel(){};				// Destructor

protected:
	//VGUI overrides:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);

private:
	//Other used VGUI control Elements:
	Label *pAugLabel[MAX_AUGS];
	Label *pStatLabel[MAX_STATS];
};

// Constructor: Initializes the Panel
CUpgradePanel::CUpgradePanel(vgui::VPANEL parent)
	: BaseClass(NULL, "UpgradePanel")
{
	char buffer[32];

	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(true);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(true);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(true);
	
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	for (int i = 0; i < MAX_AUGS; ++i)
	{
		Q_snprintf(buffer, sizeof(buffer), "auglabel%i", i + 1);
		pAugLabel[i] = new Label(this, buffer, "BLANK");
	}

	for (int i = 0; i < MAX_STATS; ++i)
	{
		Q_snprintf(buffer, sizeof(buffer), "statlabel%i", i + 1);
		pStatLabel[i] = new Label(this, buffer, "BLANK");
	}

	LoadControlSettings("resource/UI/upgradepanel.res");

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	DevMsg("UpgradePanel has been constructed.\n");

}

//Class: CMyPanelInterface Class. Used for construction.
class CUpgradePanelInterface : public IUpgradePanel
{
private:
	CUpgradePanel *UpgradePanel;
public:
	CUpgradePanelInterface()
	{
		UpgradePanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		UpgradePanel = new CUpgradePanel(parent);
	}
	void Destroy()
	{
		if (UpgradePanel)
		{
			UpgradePanel->SetParent((vgui::Panel *)NULL);
			delete UpgradePanel;
		}
	}
};
static CUpgradePanelInterface g_UpgradePanel;
IUpgradePanel* upgradepanel = (IUpgradePanel*)&g_UpgradePanel;

ConVar cl_showupgradepanel("cl_showupgradepanel", "0", FCVAR_CLIENTDLL, "Sets the state of the upgrade panel: <state>");

void CUpgradePanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_showupgradepanel.GetBool());

	CBasePlayer *pPlayer = ToBasePlayer(UTIL_PlayerByIndex(1));
	if (pPlayer)
	{
		char buffer[256];
		C_BaseHLPlayer *pHL2Player = dynamic_cast<C_BaseHLPlayer*>(pPlayer);
		if (pHL2Player)
		{
			int healthups = pPlayer->m_Local.m_iHealthUpgrades;
			int healthsegups = pPlayer->m_Local.m_iHealthSegmentUpgrades;
			int armorups = pPlayer->m_Local.m_iArmorUpgrades;
			int armorsegups = pPlayer->m_Local.m_iArmorSegmentUpgrades;
			int auxcells = pHL2Player->m_HL2Local.m_iAuxPowerUpgradeCells;

			Q_snprintf(buffer, sizeof(buffer), "Next upgrade costs %i\nCurrent Upgrade %i / 10", (healthups + 1) * 1000, healthups);
			pAugLabel[0]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Next upgrade costs %i\nCurrent Upgrade %i / 10", (healthsegups + 1) * 1000, healthsegups);
			pAugLabel[1]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Next upgrade costs %i\nCurrent Upgrade %i / 14", (armorups + 1) * 1000, armorups);
			pAugLabel[2]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Next upgrade costs %i\nCurrent Upgrade %i / 10", (armorsegups + 1) * 1000, armorsegups);
			pAugLabel[3]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Next upgrade costs %i\nCurrent Upgrade %i / 10", (auxcells + 1) * 2000, auxcells);
			pAugLabel[4]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Max Health: %i segments x %i segment strength = %i", (healthups + 5), (10 + (5 * healthsegups)), pPlayer->GetMaxHealth());
			pStatLabel[0]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Max Armor: %i segments x %i segment strength = %i", (armorups + 1), (10 + (5 * armorsegups)), (armorups + 1) * (10 + (5 * armorsegups)));
			pStatLabel[1]->SetText(buffer);

			Q_snprintf(buffer, sizeof(buffer), "Aux Power: 50 x %i power cells = %i", 1 + auxcells, (auxcells + 1) * 50);
			pStatLabel[2]->SetText(buffer);
		}
	}
}

void CUpgradePanel::OnCommand(const char* pcCommand)
{
	if (!Q_stricmp(pcCommand, "vguicancel"))
		cl_showupgradepanel.SetValue(0);
	if (!Q_strncmp(pcCommand, "upgrade_", 8))
	{
		engine->ServerCmd(pcCommand);
	}
	BaseClass::OnCommand(pcCommand);
}

CON_COMMAND(ToggleUpgradePanel, "Toggles upgradePanel on or off")
{
	cl_showupgradepanel.SetValue(!cl_showupgradepanel.GetBool());
};

