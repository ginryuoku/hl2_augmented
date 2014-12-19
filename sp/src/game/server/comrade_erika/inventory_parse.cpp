
#include "cbase.h"
#include "inventory_parse.h"
#include <KeyValues.h>
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CUtlDict< FileInventoryInfo_t*, unsigned short > m_ItemInfoDatabase;

FileInventoryInfo_t::FileInventoryInfo_t()
{
	bParsedScript = false;

	item_id						= 0;
	item_entityname[0]			= 0;
	item_friendlyname[0]		= 0;
	item_friendlydescription[0] = 0;
	item_capacity				= 0;
	item_maxcapacity			= 0;
	item_contains				= 0;
	item_baseprice				= 0;
	item_unitprice				= 0;
	item_baseweight				= 0;
	item_unitweight				= 0;
}

static ITEM_FILE_INFO_HANDLE FindItemInfoSlot(int itemid)
{
	char szLookup[MAX_NAME];

	Q_snprintf(szLookup, sizeof(szLookup), "%d", itemid);

	unsigned short lookup = m_ItemInfoDatabase.Find(szLookup);

	if (lookup != m_ItemInfoDatabase.InvalidIndex())
	{
		return lookup;
	}

	FileInventoryInfo_t *insert = CreateItemInfo();

	lookup = m_ItemInfoDatabase.Insert(szLookup, insert);

	Assert(lookup != m_ItemInfoDatabase.InvalidIndex());
	return lookup;
}

// FIXME: Valve didn't like this idea to begin with.
static FileInventoryInfo_t gNullItemInfo;

FileInventoryInfo_t *GetFileItemInfoFromHandle(ITEM_FILE_INFO_HANDLE handle)
{
	if (handle < 0 || handle >= m_ItemInfoDatabase.Count())
	{
		return &gNullItemInfo;
	}

	if (handle == m_ItemInfoDatabase.InvalidIndex())
	{
		return &gNullItemInfo;
	}

	return m_ItemInfoDatabase[handle];
}

// Reads KV file.
// As we are not writing a multi-player game, we do not encrypt our KV files.
// Should this change, update function accordingly.
KeyValues* ReadKVFile(IFileSystem *filesystem, const char *szFilenameWithoutExtension)
{
	Assert(strchr(szFilenameWithoutExtension, '.') == NULL);
	char szFullName[512];

	const char *pSearchPath = "GAME";

	KeyValues *pKV = new KeyValues("ItemDatafile");

	Q_snprintf(szFullName, sizeof(szFullName), "%s.txt", szFilenameWithoutExtension);

	if (!pKV->LoadFromFile(filesystem, szFullName, pSearchPath))
	{
		pKV->deleteThis();
		return NULL;
	}

	DevMsg("Loaded scripts/items/%s.txt for inventory parser\n", szFilenameWithoutExtension);

	return pKV;
}


bool ReadItemDataFromFileInSlot(IFileSystem* filesystem, int itemid, ITEM_FILE_INFO_HANDLE *phandle)
{
	if (!phandle)
	{
		Assert(0);
		return false;
	}

	// The referenced handle does *not* map to inventory numeric IDs!
	*phandle = FindItemInfoSlot(itemid);
	FileInventoryInfo_t *pFileInfo = GetFileItemInfoFromHandle(*phandle);
	Assert(pFileInfo);

	if (pFileInfo->bParsedScript)
		return true;

	char sz[128];
	Q_snprintf(sz, sizeof(sz), "scripts/items/%d", itemid);

	KeyValues *pKV = ReadKVFile(filesystem, sz);

	if ( !pKV)
	{
		return false;
	}

	pFileInfo->Parse(pKV, itemid);

	pKV->deleteThis();

	return true;
}
void FileInventoryInfo_t::Parse(KeyValues *pKeyValuesData, int itemid)
{
	bParsedScript = true;

	// Entity classname
	Q_strncpy(item_entityname, pKeyValuesData->GetString("entity_name"), MAX_NAME);
	Q_strncpy(item_friendlyname, pKeyValuesData->GetString("friendly_name", MISSING_DESCRIPTION), MAX_NAME);
	Q_strncpy(item_friendlydescription, pKeyValuesData->GetString("description", MISSING_DESCRIPTION), MAX_DESCRIPTION);
	
	item_id = itemid;

	item_capacity = pKeyValuesData->GetInt("item_capacity");
	item_maxcapacity = pKeyValuesData->GetInt("item_maxcapacity");
	
	item_contains = pKeyValuesData->GetInt("item_contains");
	item_baseprice = pKeyValuesData->GetInt("item_baseprice");
	item_unitprice = pKeyValuesData->GetInt("item_unitprice");
	item_baseweight = pKeyValuesData->GetInt("item_baseweight");
	item_unitweight = pKeyValuesData->GetInt("item_unitweight");

	DevMsg("Parsed script for item with numeric ID %d\n", itemid);

}


