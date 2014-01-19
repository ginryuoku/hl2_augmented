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
	
	void PurgeObject( int element );
	void PurgeAllObjects();

private:
	int ItemID[100];
	int ItemCap[100];
	int ItemMaxCap[100];

};

#endif