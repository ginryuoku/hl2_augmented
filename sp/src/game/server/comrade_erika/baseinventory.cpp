//////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "comrade_erika/baseinventory.h"

#include "tier0/memdbgon.h"

BEGIN_SIMPLE_DATADESC( CBaseInventory )
	DEFINE_ARRAY(ItemID, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemCap, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemMaxCap, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemType, FIELD_INTEGER, MAX_INVENTORY),
	DEFINE_ARRAY(ItemContains, FIELD_INTEGER, MAX_INVENTORY),
END_DATADESC()

ITEM_FILE_INFO_HANDLE m_hInventoryFileInfo;

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
	ItemContains[element] = 0;
	ItemDirty[element] = true;

	DevMsg("Purged index %d\n", element);
}

void CBaseInventory::PurgeAllObjects()
{
	for ( int i = 0; i < MAX_INVENTORY; ++i)
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

int CBaseInventory::GetItemContains(int element)
{
	return ItemContains[element];
}

void CBaseInventory::SetItemCapacity(int element, int newcapacity)
{
	if (newcapacity == 0 && GetItemType(element) != ITEM_MAGAZINE)
	{
		PurgeObject(element);
	}

	if (newcapacity > GetItemMaxCapacity(element))
	{
		ItemCap[element] = GetItemMaxCapacity(element);
		ItemDirty[element] = true;
	}
	else 
	{
		ItemCap[element] = newcapacity;
		ItemDirty[element] = true;
	}
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

bool CBaseInventory::LoadInfo(int itemid)
{
	return ReadItemDataFromFileInSlot(filesystem, itemid, &m_hInventoryFileInfo);
}
// ALL CALLS MUST BE PRECEDED BY LOADINFO!
const FileInventoryInfo_t &CBaseInventory::GetItemInfo(void) const
{
	return *GetFileItemInfoFromHandle(m_hInventoryFileInfo);
}

int CBaseInventory::ReturnPrice(int itemindex)
{
	if (LoadInfo(GetItemID(itemindex)))
	{
		int baseprice = GetItemInfo().item_baseprice;
		int unitprice = GetItemCapacity(itemindex) * GetItemInfo().item_unitprice;
		return baseprice + unitprice;
	}
	return 0;
}

void CBaseInventory::NewObject( int ObjectIndex, int NewItemID, int NewItemCap, int NewItemMaxCap )
{
	if (ObjectIndex < 0)
		return;

	if (LoadInfo(NewItemID))
	{
		ItemID[ObjectIndex] = NewItemID;
		ItemCap[ObjectIndex] = NewItemCap;
		ItemMaxCap[ObjectIndex] = NewItemMaxCap;
		ItemType[ObjectIndex] = FindItemType(NewItemID);
		ItemContains[ObjectIndex] = GetItemInfo().item_contains;

		ItemDirty[ObjectIndex] = true;

		Msg("Server: Created new object at position %d, item ID %d, capacity %d, max capacity %d, which contains %d\n",
			ObjectIndex, NewItemID, NewItemCap, NewItemMaxCap, ItemContains[ObjectIndex]);
	}
	return;
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
	UTIL_CenterPrintAll("You can't pick that up!");
	return -1;
}

int CBaseInventory::FindFirstFullObject(int itemid)
{
	int element = -1;
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (GetItemID(i) == itemid)
		{
			if (GetItemCapacity(i) > 0 && element < 0)
				element = i;

			if (GetItemCapacity(i) == GetItemMaxCapacity(i))
				return i;

			if ((GetItemCapacity(i) > GetItemCapacity(element) && GetItemCapacity(i) <= GetItemMaxCapacity(i)))
			{
				element = i;
			}
		}
	}
	return element;
}

// Returns -1 if not found, otherwise returns index of object
int CBaseInventory::FindFirstEmptyObject(int itemid)
{
	int element = GetItemInfo().item_maxcapacity;
	int item_index = -1;
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (GetItemID(i) == itemid)
		{
			if (GetItemCapacity(i) == 0)
				return i;

			if (GetItemCapacity(i) < element)
			{
				element = GetItemCapacity(i);
				item_index = i;
			}
		}
	}
	return item_index;
}


int CBaseInventory::FindFirstObjectContainingThis(int itemid)
{
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (ItemContains[i] == itemid)
		{
			return i;
		}
	}
	return -1;
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
	if (element <= 0)
		return -1;
	else
		return element;
}
// Takes an item ID and types it.
int CBaseInventory::FindItemType(int itemid)
{
	if (itemid >= 1000)
		return -1;
	else if (itemid >= 500)
		return 11;	// KEY_ITEM
	else if (itemid >= 350)
		return 10;	// WEAPON
	else if (itemid >= 250)
		return ITEM_MAGAZINE;	// MAGAZINE
	else if (itemid >= 180)
		return ITEM_AMMO;	// AMMO
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

int CBaseInventory::FindArmorItem()
{
	return FindFirstFullObjectByType(3);
}

// Returns amount actually used.
// Takes 'used' (amount you need from such item), and object (index of item)
int CBaseInventory::UseItem(int used, int itemid)
{
	if (used > ItemCap[itemid])
		return 0; // you can't use more than the object has
	ItemCap[itemid] = ItemCap[itemid] - used;
	
	if (ItemCap[itemid] == 0 && FindItemType(itemid) != ITEM_MAGAZINE)
	{
		PurgeObject(itemid); // it's empty, throw it away.
		return used;
	}
	
	else
	{
		ItemDirty[itemid] = true; // we changed it, so it's just dirty.
		return used;
	}

}

int CBaseInventory::CountAllObjectContentsOfID(int itemid)
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

int CBaseInventory::CountAllObjectsOfID(int itemid, bool non_empty)
{
	int items = 0;
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (non_empty)
		{
			if (GetItemCapacity(i) != 0 && GetItemID(i) == itemid)
				++items;
		}
		else if (GetItemID(i) == itemid)
			++items;
	}
	return items;
}

int CBaseInventory::SwapMagazines(int itemid, int remaining)
{
	if (FindItemType(itemid) != ITEM_MAGAZINE)
	{
		return -1;
	}

	int mag = FindFirstFullObject(itemid);
	if (mag == -1)
		return -1;
	int used = GetItemCapacity(mag);

	ItemCap[mag] = remaining;
	ItemDirty[mag] = true;

	return used;
}

int CBaseInventory::FindMagForReloading(int itemid)
{
	int index = -1;
	if (FindItemType(itemid) != ITEM_AMMO)
		return -1;
	int candidate = 999;
	for (int i = 0; i < MAX_INVENTORY; ++i)
	{
		if (GetItemContains(i) == itemid && GetItemID(i) != itemid && 
			FindItemType(GetItemID(i)) == ITEM_MAGAZINE)
		{
			if (GetItemCapacity(i) < candidate && GetItemCapacity(i) < GetItemMaxCapacity(i))
			{
				candidate = GetItemCapacity(i);
				index = i;
				Msg("Candidate: index %d, with capacity %d\n", candidate, GetItemCapacity(i));
			}
		}
	}

	return index;
}

void CBaseInventory::CombineItems(int itemindex1, int itemindex2)
{
	int newcap1 = 0;
	int newcap2 = 0;
	int merged = 0;

	if (GetItemID(itemindex1) != GetItemID(itemindex2))
		return;
	if (GetItemContains(itemindex1) != (GetItemContains(itemindex2)))
		return;

	merged = GetItemCapacity(itemindex1) + GetItemCapacity(itemindex2);

	if (merged > GetItemMaxCapacity(itemindex1))
	{
		newcap1 = GetItemMaxCapacity(itemindex1);
		newcap2 = merged - GetItemMaxCapacity(itemindex1);
	}
	else
	{
		newcap1 = merged;
		newcap2 = 0;
	}

	SetItemCapacity(itemindex1, newcap1);
	SetItemCapacity(itemindex2, newcap2);
}

void CBaseInventory::SwapItems(int itemindex1, int itemindex2)
{
	int newid1 = GetItemID(itemindex2);
	int newcap1 = GetItemCapacity(itemindex2);
	int newmaxcap1 = GetItemMaxCapacity(itemindex2);
	int newcontains1 = GetItemContains(itemindex2);
	int newtype1 = GetItemType(itemindex2);

	int newid2 = GetItemID(itemindex1);
	int newcap2 = GetItemCapacity(itemindex1);
	int newmaxcap2 = GetItemMaxCapacity(itemindex1);
	int newcontains2 = GetItemContains(itemindex1);
	int newtype2 = GetItemType(itemindex1);

	ItemID[itemindex1] = newid1;
	ItemCap[itemindex1] = newcap1;
	ItemMaxCap[itemindex1] = newmaxcap1;
	ItemContains[itemindex1] = newcontains1;
	ItemType[itemindex1] = newtype1;
	
	ItemID[itemindex2] = newid2;
	ItemCap[itemindex2] = newcap2;
	ItemMaxCap[itemindex2] = newmaxcap2;
	ItemContains[itemindex2] = newcontains2;
	ItemType[itemindex2] = newtype2;

	ItemDirty[itemindex1] = true;
	ItemDirty[itemindex2] = true;
}

int CBaseInventory::QSPartition(int p, int r)
{
	unsigned int x = GetItemID(p);
	int i = p - 1;
	int j = r;

	while (1)
	{
		do --j; while ((unsigned int) GetItemID(j) > x);
		do ++i; while ((unsigned int) GetItemID(i) < x);

		if (i < j)
			SwapItems(i, j);
		else
			return j + 1;
	}
}

bool CBaseInventory::SanityCheck(void)
{
	bool insanity = false;
	for (int i = 0; i <= MAX_INVENTORY; ++i)
	{
		if (GetItemID(i) > 0)
		{
			if (!LoadInfo(GetItemID(i))) 
			{
				PurgeObject(i);
				insanity = true;
			}

			if (GetItemCapacity(i) > GetItemMaxCapacity(i))
			{
				SetItemCapacity(i, GetItemMaxCapacity(i));
				insanity = true;
			}

			if (GetItemInfo().item_maxcapacity != GetItemMaxCapacity(i))
			{
				ItemMaxCap[i] = GetItemInfo().item_maxcapacity;
				ItemDirty[i] = true;
				insanity = true;
			}

			if (GetItemInfo().item_contains != GetItemContains(i))
			{
				ItemContains[i] = GetItemInfo().item_contains;
				ItemDirty[i] = true;
				insanity = true;
			}

		}
		else if (GetItemContains(i) > 0 || GetItemCapacity(i) > 0 || GetItemMaxCapacity(i) > 0 || GetItemType(i) > 0)
		{
			PurgeObject(i);
			insanity = true;
		}
	}
	return insanity;
}

void CBaseInventory::QuickSort(int indexleft, int indexright)
{
	int q;

	// Hoare partitioning breaks on cases of 0/1 separation
	if (indexright - indexleft < 2)
		return;
	
	q = QSPartition(indexleft, indexright);

	QuickSort(indexleft, q);
	QuickSort(q, indexright);
}

void CBaseInventory::ConsolidateAmmo(void)
{
	for (int i = 180; i < 195; ++i)
	{
		int totalammo = 0;
		if (FindFirstObject(i) > -1)
		{
			int maxcap = GetItemMaxCapacity(FindFirstObject(i));
			if (maxcap > 0)
			{
				for (int x = 0; x < MAX_INVENTORY; ++x)
				{
					if (GetItemID(x) == i)
					{
						totalammo = totalammo + GetItemCapacity(x);
						PurgeObject(x);
					}
				}

				Msg("Total ammo for type %d: %d\n", i, totalammo);

				while (totalammo > 0)
				{
					if (totalammo > maxcap)
					{
						NewObject(FindFirstFreeObject(), i, maxcap, maxcap);
						totalammo = totalammo - maxcap;
					}
					else if (totalammo <= maxcap && totalammo > 0)
					{
						NewObject(FindFirstFreeObject(), i, totalammo, maxcap);
						totalammo = 0;
					}
				}
			}
		}
	}
}

void ConsolidateAmmoManually(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		pPlayer->m_pInventory.ConsolidateAmmo();
		pPlayer->m_pInventory.QuickSort(0, MAX_INVENTORY);
	}
}
ConCommand mergeammo("mergeammo", ConsolidateAmmoManually, "Consolidates ammunition.", 0);

void UseItemFromInventory(const CCommand &args)
{
	if (args.ArgC() < 1)
	{
		Msg("Usage: use_item <item index>. You normally need not call this manually.\n");
		return;
	}

	int itemindex = atoi(args.Arg(1));
	if (itemindex < 0 || itemindex > MAX_INVENTORY)
	{
		Msg("Was passed nonsense value %d, ignoring command\n", itemindex);
		return;
	}

	Msg("Item index passed: %d\n", itemindex);

	CBasePlayer *pPlayer = UTIL_GetCommandClient();

	if (pPlayer)
	{
		int itemtype = pPlayer->m_pInventory.FindItemType(pPlayer->m_pInventory.GetItemID(itemindex));
		int itemcontains = pPlayer->m_pInventory.GetItemContains(itemindex);
		switch (itemtype)
		{
		case ITEM_HEALTH:
		{
			int used = pPlayer->TakeHealth(pPlayer->m_pInventory.GetItemCapacity(itemindex), DMG_GENERIC);
			pPlayer->m_pInventory.UseItem(used, itemindex);
			break;
		}
		case ITEM_POWER:
		{
			int used = pPlayer->MaxArmorValue() - pPlayer->ArmorValue();
			if (used <= 0)
				return;

			if (used > pPlayer->m_pInventory.GetItemCapacity(itemindex))
				used = pPlayer->m_pInventory.GetItemCapacity(itemindex);

			pPlayer->IncrementArmorValue(used, pPlayer->MaxArmorValue());
			pPlayer->m_pInventory.UseItem(used, itemindex);
		}
		case ITEM_AMMO:
		{
			if (itemcontains == -1)
				return;
			int ammobox = pPlayer->m_pInventory.GetItemID(itemindex);

			int mag = pPlayer->m_pInventory.FindMagForReloading(ammobox);
			if (mag == -1)
				return;
			if (pPlayer->m_pInventory.GetItemCapacity(mag) == pPlayer->m_pInventory.GetItemMaxCapacity(mag))
				return;

			int used = pPlayer->m_pInventory.GetItemMaxCapacity(mag) - pPlayer->m_pInventory.GetItemCapacity(mag);
			if (used > pPlayer->m_pInventory.GetItemCapacity(itemindex))
			{
				used = pPlayer->m_pInventory.GetItemCapacity(itemindex);
			}

			int newmag = pPlayer->m_pInventory.GetItemCapacity(mag) + pPlayer->m_pInventory.UseItem(used, itemindex);
			if (newmag == 0)
				return;

			pPlayer->m_pInventory.SetItemCapacity(mag, newmag);
			
			break;
		}
		case ITEM_MAGAZINE:
		{
			if (itemcontains == -1)
				return;
			int needed = pPlayer->m_pInventory.GetItemMaxCapacity(itemindex) - pPlayer->m_pInventory.GetItemCapacity(itemindex);
			if (needed == 0)
			{
				return;
			}
			int ammobox = pPlayer->m_pInventory.FindFirstFullObject(itemcontains);
			if (pPlayer->m_pInventory.FindItemType(pPlayer->m_pInventory.GetItemID(ammobox)) != ITEM_AMMO || pPlayer->m_pInventory.GetItemCapacity(ammobox) == 0)
				return;

			if (needed > pPlayer->m_pInventory.GetItemCapacity(ammobox))
			{
				needed = pPlayer->m_pInventory.GetItemCapacity(ammobox);
			}

			int newmag = pPlayer->m_pInventory.GetItemCapacity(itemindex) + pPlayer->m_pInventory.UseItem(needed, ammobox);
			if (newmag == 0)
				return;

			pPlayer->m_pInventory.SetItemCapacity(itemindex, newmag);
		}
		default:
			return;
		};
	}
}
ConCommand use_item("use_item", UseItemFromInventory, "use_item <item index>", 0);

void UseHealthItem(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();

	if (pPlayer)
	{
		int itemid = pPlayer->m_pInventory.FindHealthItem();
		int used = pPlayer->TakeHealth(pPlayer->m_pInventory.GetItemCapacity(itemid), DMG_GENERIC);
		pPlayer->m_pInventory.UseItem(used, itemid);
	}
}

ConCommand use_heal("use_heal", UseHealthItem, "Use first healing item in inventory", 0);

void UseArmorItem(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();

	if (pPlayer)
	{
		int used = pPlayer->MaxArmorValue() - pPlayer->ArmorValue();
		if (used <= 0)
			return;

		int itemid = pPlayer->m_pInventory.FindArmorItem();

		if (itemid <= -1)
			return;

		if (used > pPlayer->m_pInventory.GetItemCapacity(itemid))
			used = pPlayer->m_pInventory.GetItemCapacity(itemid);

		pPlayer->IncrementArmorValue(used, pPlayer->MaxArmorValue());
		pPlayer->m_pInventory.UseItem(used, itemid);
	}
}

ConCommand use_armor("use_armor", UseArmorItem, "Use first armor item in inventory", 0);

void SellItem(const CCommand &args)
{
	if (args.ArgC() < 1)
	{
		Msg("Usage: sell_item <item index>.\n");
		return;
	}

	int itemindex = atoi(args.Arg(1));
	if (itemindex < 0 || itemindex > MAX_INVENTORY)
	{
		Msg("Was passed nonsense value %d, ignoring command\n", itemindex);
		return;
	}

	Msg("Item index passed: %d\n", itemindex);

	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		int price = pPlayer->m_pInventory.ReturnPrice(itemindex);
		if (price > 0)
		{
			pPlayer->AddPlayerCash(price);
			pPlayer->m_pInventory.PurgeObject(itemindex);
		}
	}
}
ConCommand sell_item("sell_item", SellItem, "Sells the item at the item index", 0);

void CombineItems(const CCommand &args)
{
	if (args.ArgC() < 2)
	{
		Msg("Usage: merge_items <first index> <second index>.\n");
		return;
	}

	int itemindex = atoi(args.Arg(1));
	if (itemindex < 0 || itemindex > MAX_INVENTORY)
	{
		Msg("Was passed nonsense value %d, ignoring command\n", itemindex);
		return;
	}

	int itemindex2 = atoi(args.Arg(2));
	if (itemindex2 < 0 || itemindex2 > MAX_INVENTORY)
	{
		Msg("Was passed nonsense value %d, ignoring command\n", itemindex2);
		return;
	}

	if (itemindex == itemindex2)
		return;

	CBasePlayer *pPlayer = UTIL_GetCommandClient();

	if (pPlayer)
	{
		pPlayer->m_pInventory.CombineItems(itemindex, itemindex2);
	}
}

ConCommand merge_items("merge_items", CombineItems, "Merges two items together\n", 0);

void InvSanityCheck(const CCommand &args)
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if (pPlayer)
	{
		if (pPlayer->m_pInventory.SanityCheck())
		{
			Msg("Errors detected in inventory contents. You may have lost items. SAVE NOW!\n");
		}
		else
		{
			Msg("Inventory contents are sane.\n");
		}
	}
}
ConCommand inv_sanitycheck("inv_sanitycheck", InvSanityCheck, "Checks inventory for problems.\n", 0);

