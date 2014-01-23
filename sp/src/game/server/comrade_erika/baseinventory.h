#ifndef BASEINVENTORY_H
#define BASEINVENTORY_H

#include "cbase.h"

#define MAX_INVENTORY 100

class CBaseInventory
{
public:
	CBaseInventory();

	int GetItemID( int element );
	int GetItemCapacity( int element );
	int GetItemMaxCapacity( int element );
	
	void PurgeObject( int element );
	void PurgeAllObjects();
	void ConvertEntityToObject( CBaseEntity *pEntity );

	int FindFirstFreeObject();

	void NewObject( int ObjectIndex, int NewItemID, int NewItemCap, int NewItemMaxCap );
private:
	int ItemID[MAX_INVENTORY];
	int ItemCap[MAX_INVENTORY];
	int ItemMaxCap[MAX_INVENTORY];

};

#endif