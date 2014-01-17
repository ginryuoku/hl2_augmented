
#include "cbase.h"
#include "comrade_erika/baseinventory.h"

#include "tier0/memdbgon.h"

CBaseInventory::CBaseInventory()
{
	return;
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

int CBaseInventory::GetItemBaseWeight( int element )
{
	return ItemBaseWeight[element];
}

int CBaseInventory::GetItemUnitWeight( int element )
{
	return ItemUnitWeight[element];
}

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

void CBaseInventory::MakeNewItem( int itemtype, int itemid, int cap, int maxcap, int baseweight, int unitweight )
{
	return;
}

void CBaseInventory::MakeEntityIntoObject( CBaseEntity *pEntity )
{
	if (!pEntity)
	{
		Warning("CInventory::MakeEntityIntoObject passed a NULL entity pointer!\n");
		return;
	}

	//int type = pEntity->GetItemType();
	//int id = pEntity->GetItemID();
	//int cap = pEntity->GetItemCapacity();
	//int maxcap = pEntity->GetItemMaxCapacity();
	//int baseweight = pEntity->GetItemBaseWeight();
	//int unitweight = pEntity->GetItemUnitWeight();

	//MakeNewItem(type,id,cap,maxcap,baseweight,unitweight);

	UTIL_Remove(pEntity);
}