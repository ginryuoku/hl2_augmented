//////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "comrade_erika/baseinventory.h"

#include "tier0/memdbgon.h"

BEGIN_SIMPLE_DATADESC( CBaseInventory )
	DEFINE_ARRAY(ItemID, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemCap, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemMaxCap, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemType, FIELD_INTEGER, MAX_INVENTORY),
END_DATADESC()

CBaseInventory::CBaseInventory()
{
	PurgeAllObjects();
}

void CBaseInventory::ClientUpdateMessage( CBasePlayer *pBasePlayer, int element )
{
	CSingleUserRecipientFilter filter ( pBasePlayer ); // set recipient
	filter.MakeReliable();  // reliable transmission

	UserMessageBegin( filter, "InventoryUpdate" ); // create message
	WRITE_BYTE( element );
	WRITE_LONG( ItemID[element] ); // Long required to express signed integer. Artifact of VS6?
	WRITE_BYTE( ItemCap[element] );
	WRITE_BYTE( ItemMaxCap[element] );
	MessageEnd(); //send message
	Msg("Server - pushing message: %d, %d, %d, %d\n", element, ItemID[element], ItemCap[element], ItemMaxCap[element]);
	ItemDirty[element] = false;
}

void CBaseInventory::FlushPendingObjects( CBasePlayer *pBasePlayer )
{
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (ItemDirty[i] == true)
		{
			ClientUpdateMessage(pBasePlayer, i);
		}
	}
}

void CBaseInventory::PurgeObject( int element )
{
	ItemID[element] = -1;
	ItemCap[element] = 0;
	ItemMaxCap[element] = 0;
	ItemType[element] = 0;
	ItemDirty[element] = true;
}

void CBaseInventory::PurgeAllObjects()
{
	for ( int i = 0; i < 100; ++i)
	{
		PurgeObject(i);
	}
}

int CBaseInventory::GetItemID( int element )
{
	return ItemID[element];
}

int CBaseInventory::GetItemCapacity( int element )
{
	return ItemCap[element];
}

int CBaseInventory::GetItemMaxCapacity( int element )
{
	return ItemMaxCap[element];
}

int CBaseInventory::GetItemType( int element )
{
	return ItemType[element];
}

#if 0
int CBaseInventory::GetItemTotalWeight( int element )
{
	if (ItemUnitWeight[element] && ItemBaseWeight[element])
	{
		int baseweight;
		int totalunitweight;

		baseweight = ItemBaseWeight[element];
		totalunitweight = ItemUnitWeight[element] * ItemCap[element];

		return ( baseweight + totalunitweight );
	} 
	else
	{
		return -1;
	}
}
#endif

void CBaseInventory::ConvertEntityToObject( CBaseEntity *pEntity )
{
	int NewObjectIndex = FindFirstFreeObject();
	if (NewObjectIndex == -1)
		return;
	
	if ( pEntity )
	{
		int itemid = pEntity->GetItemID();
		int itemcap = pEntity->GetItemCapacity();
		int itemmaxcap = pEntity->GetItemMaxCapacity();
		NewObject( NewObjectIndex, itemid, itemcap, itemmaxcap );
	}

	UTIL_Remove(pEntity);
	return;
}

void CBaseInventory::NewObject( int ObjectIndex, int NewItemID, int NewItemCap, int NewItemMaxCap )
{
	ItemID[ObjectIndex] = NewItemID;
	ItemCap[ObjectIndex] = NewItemCap;
	ItemMaxCap[ObjectIndex] = NewItemMaxCap;
	ItemType[ObjectIndex] = FindItemType(NewItemID);
	
	ItemDirty[ObjectIndex] = true;
	
	Msg("Server: Created new object at position %d of type %d with capacity %d and max capacity %d\n", ObjectIndex, NewItemID, NewItemCap, NewItemMaxCap);
}

int CBaseInventory::FindFirstObject(int itemid)
{
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (ItemID[i] == itemid)
		{
			return i;
		}
	}
	return -1;
}

int CBaseInventory::FindFirstFreeObject()
{
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (ItemID[i] == -1)
		{
			return i;
		}
	}
	return -1;
}

int CBaseInventory::FindFirstFullObject(int itemid)
{
	int element = 0;
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (GetItemID(i) == itemid)
		{
			if (GetItemCapacity(i) == GetItemMaxCapacity(i))
				return i;

			if (GetItemCapacity(i) > GetItemCapacity(element))
			{
				element = i;
			}
		}
	}
	return element;
}

// This function is *exclusively* for fungible items. Fungibles include healing items,
// power items and armor items. 
int CBaseInventory::FindFirstFullObjectByType(int itemtype)
{

	if (itemtype != 1 && itemtype != 3)
	{
		DevMsg("Called FindFirstFullObjectByType on non-fungible item type\n");
		return -1;
	}
	
	int element = 0;
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (GetItemType(i) == itemtype)
		{
			if (GetItemCapacity(i) == GetItemMaxCapacity(i))
				return i;

			if (GetItemCapacity(i) > GetItemCapacity(element))
			{
				element = i;
			}
		}
	}
	return element;
}

int CBaseInventory::FindItemType(int itemid)
{
	if (itemid >= 1000)
		return -1;
	else if (itemid >= 500)
		return 11;	// KEY_ITEM
	else if (itemid >= 350)
		return 10;	// WEAPON
	else if (itemid >= 250)
		return 9;	// MAGAZINE
	else if (itemid >= 180)
		return 8;	// AMMO
	else if (itemid >= 120)
		return 7;	// W_ACCESSORY
	else if (itemid >= 60)
		return 6;	// EQUIP
	else if (itemid >= 50)
		return 5;	// ENHANCE
	else if (itemid >= 40)
		return 4;	// BLEED
	else if (itemid >= 30)
		return 3;	// POWER
	else if (itemid >= 20)
		return 2;	// ARMOR
	else if (itemid >= 10)
		return 1;	// HEALTH
	else return 0;
}

int CBaseInventory::FindHealthItem()
{
	return FindFirstFullObjectByType(1);
}

// Returns amount actually used.
// Takes 'used' (amount you need from such item), and object (index of item)
int CBaseInventory::UseItem(int used, int object)
{
	if (used > ItemCap[object])
		return 0; // you can't use more than the object has
	ItemCap[object] = ItemCap[object] - used;
	
	if (ItemCap[object] == 0)
	{
		PurgeObject(object); // it's empty, throw it away.
		return used;
	}
	
	else
	{
		ItemDirty[object] = true; // we changed it, so it's just dirty.
		return used;
	}

}

int CBaseInventory::CountAllObjectsOfID(int itemid)
{
	int itemcount = 0;

	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (GetItemID(i) == itemid)
		{
			itemcount = itemcount + GetItemCapacity(i);
		}
	}

	return itemcount;
}

void UseHealthItem(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetLocalPlayer();

	if (pPlayer)
	{
		int itemid = pPlayer->m_pInventory.FindHealthItem();
		int used = pPlayer->TakeHealth(pPlayer->m_pInventory.GetItemCapacity(itemid), itemid);
		pPlayer->m_pInventory.UseItem(used, itemid);
	}
}

ConCommand use_heal("use_heal", UseHealthItem, "Use first healing item in inventory", 0);
