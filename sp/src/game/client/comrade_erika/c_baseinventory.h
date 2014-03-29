//
//  c_baseinventory.h
//  games
//
//  Created by Michelle Darcy on 3/2/14.
//
//

#ifndef __games__c_baseinventory__
#define __games__c_baseinventory__

#include "cbase.h"

#define MAX_INVENTORY 100

class CBaseInventory
{
public:
	CBaseInventory();
	
	//DECLARE_SERVERCLASS();
	
	int GetItemID( int element );
	int GetItemCapacity( int element );
	int GetItemMaxCapacity( int element );
	bool GetItemDirtiness( int element );
 	
	int FindFirstFreeObject();
	
	void UpdateObject( int ObjectIndex, int NewItemID, int NewItemCap, int NewItemMaxCap );
	void ItemIsDirty(int element);
	void ItemIsClean(int element);
private:
	int ItemID[MAX_INVENTORY];
	int ItemCap[MAX_INVENTORY];
	int ItemMaxCap[MAX_INVENTORY];
	bool ItemDirty[MAX_INVENTORY];
};

#endif /* defined(__games__c_baseinventory__) */
