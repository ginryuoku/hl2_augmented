#ifndef BASEINVENTORY_H
#define BASEINVENTORY_H

#include "cbase.h"

#define MAX_INVENTORY 100

//ID	TYPENAME	TYPENUMBER
//10	HEALTH		1
//20	ARMOR		2
//30	POWER		3
//40	BLEED		4
//50	ENHANCE		5
//60	EQUIP		6
//120	W_ACCESSORY 7
//180	AMMO		8
//250	MAGAZINE	9
//350	WEAPON		10
//500	KEY_ITEM	11

class CBaseInventory
{
public:
	CBaseInventory();

	DECLARE_SIMPLE_DATADESC();
	DECLARE_CLASS_NOBASE( CBaseInventory );
	//DECLARE_SERVERCLASS();

	void FlushPendingObjects( CBasePlayer *pBasePlayer );
	void ClientUpdateMessage( CBasePlayer *pBasePlayer, int element );
	
	int GetItemID( int element );
	int GetItemCapacity( int element );
	int GetItemMaxCapacity( int element );
	int GetItemType(int element);

	void PurgeObject( int element );
	void PurgeAllObjects();
	void ConvertEntityToObject( CBaseEntity *pEntity );

	int FindFirstFreeObject();
	int FindFirstFullObject(int itemid);
	int FindFirstFullObjectByType(int itemtype);
	int FindHealthItem();
	int FindItemType(int itemid);

	int UseItem(int used, int object);
	int UseHealthItem(int used, int itemid);
	int CountAllObjectsOfID(int itemid);

	void NewObject( int ObjectIndex, int NewItemID, int NewItemCap, int NewItemMaxCap );
private:
	int  ItemID     [MAX_INVENTORY];
	int  ItemCap    [MAX_INVENTORY];
	int  ItemMaxCap [MAX_INVENTORY];
	int  ItemType   [MAX_INVENTORY];
	bool ItemDirty  [MAX_INVENTORY]; // dirty flag for each
};

#endif
