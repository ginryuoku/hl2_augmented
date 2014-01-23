//////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "comrade_erika/baseinventory.h"

#include "tier0/memdbgon.h"

CBaseInventory::CBaseInventory()
{
	PurgeAllObjects();
}

void CBaseInventory::PurgeObject( int element )
{
	ItemID[element] = -1;
	ItemCap[element] = 0;
	ItemMaxCap[element] = 0;
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
	int i = 0;
	int NewObjectIndex = 0;
	do 
	{
		if (ItemID < 0)
		{
			++i;
		}
		else
		{
			NewObjectIndex = i;
		}
	} while (i < MAX_INVENTORY && !NewObjectIndex );

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
}
