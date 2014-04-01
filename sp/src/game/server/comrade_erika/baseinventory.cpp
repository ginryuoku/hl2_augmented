//////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "comrade_erika/baseinventory.h"

#include "tier0/memdbgon.h"

BEGIN_SIMPLE_DATADESC( CBaseInventory )
	DEFINE_ARRAY(ItemID, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemCap, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemMaxCap, FIELD_INTEGER, MAX_INVENTORY),
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
	
	ItemDirty[ObjectIndex] = true;
	
	Msg("Server: Created new object at position %d of type %d with capacity %d and max capacity %d\n", ObjectIndex, NewItemID, NewItemCap, NewItemMaxCap);
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
