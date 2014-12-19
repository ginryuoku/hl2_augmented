
// Default implementation of the inventory item definition parser.

#include "cbase.h"
#include "inventory_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

FileInventoryInfo_t* CreateItemInfo()
{
	return new FileInventoryInfo_t;
}
