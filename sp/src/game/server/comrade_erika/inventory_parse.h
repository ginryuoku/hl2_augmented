#ifndef ITEM_PARSE_H
#define ITEM_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"

#define MAX_NAME 80
#define MAX_DESCRIPTION 1024
#define MISSING_DESCRIPTION "!!! DESCRIPTION MISSING"

class IFileSystem;

typedef unsigned short ITEM_FILE_INFO_HANDLE;

enum ItemType_t
{
	NONE,
	HEALING,
	ARMOR,
	POWER,
	BLEEDING,
	ENHANCEMENT,
	EQUIPMENT,
	WPN_ACCESSORY,
	AMMO,
	MAGAZINE,
	WEAPON,
	KEY_ITEM,
};

class KeyValues;

int GetItemTypeFromString(const char *pszString);

class FileInventoryInfo_t
{
public:
	FileInventoryInfo_t();

	// This can be overridden to get whatever values are wanted from the script.
	virtual void Parse(KeyValues *pKeyValuesData, int itemid);

public:

	bool bParsedScript;

	int item_id;

	char item_entityname[MAX_NAME];
	char item_friendlyname[MAX_NAME];
	char item_friendlydescription[MAX_DESCRIPTION];

	int item_capacity;
	int item_maxcapacity;
	
	int item_contains;

	int item_baseprice; // price for empty item
	int item_unitprice; // price for empty item

	int item_baseweight; // grams
	int item_unitweight; // grams
};

bool ReadItemDataFromFileInSlot(IFileSystem* filesystem, int itemid, ITEM_FILE_INFO_HANDLE *phandle);

ITEM_FILE_INFO_HANDLE LookupItemInfoSlot(int itemid);

FileInventoryInfo_t *GetFileItemInfoFromHandle(ITEM_FILE_INFO_HANDLE handle);
WEAPON_FILE_INFO_HANDLE GetInvalidWeaponInfoHandle(void);

extern FileInventoryInfo_t* CreateItemInfo();

#endif
