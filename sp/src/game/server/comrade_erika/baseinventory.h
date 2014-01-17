#ifndef BASEINVENTORY_H
#define BASEINVENTORY_H

#include "cbase.h"

class CBaseInventory
{
public:
	CBaseInventory();

	int GetItemID( int element );
	int GetItemCapacity( int element );
	int GetItemMaxCapacity( int element );

	int GetItemBaseWeight( int element );
	int GetItemUnitWeight( int element );
	int GetItemTotalWeight( int element );

	void MakeNewItem( int itemtype, int itemid, int cap, int maxcap, int baseweight, int unitweight );
	void MakeEntityIntoObject( CBaseEntity *pEntity );

private:
	int ItemType[100];
	int ItemID[100];
	int ItemCap[100];
	int ItemMaxCap[100];
	int ItemBaseWeight[100];
	int ItemUnitWeight[100];

};

#endif