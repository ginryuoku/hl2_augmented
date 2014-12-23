// Implementation of all magazine entities
#include "cbase.h"
#include "items.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CItem_Mag_9x19_15 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_9x19_15, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");
		
		// ItemID must be established before running the common spawn code
		m_iItemID = 250;

		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_mag_9x19_15, CItem_Mag_9x19_15)

class CItem_Mag_9x19_30mp5 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_9x19_30mp5, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 270;

		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_mag_9x19_30mp5, CItem_Mag_9x19_30mp5)

class CItem_Mag_57x28_50p90 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_57x28_50p90, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 271;

		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_mag_57x28_50p90, CItem_Mag_57x28_50p90)

class CItem_Mag_357_Speed6 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_357_Speed6, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 260;

		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_mag_357_speed6, CItem_Mag_357_Speed6)

class CItem_Mag_556_STANAG30 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_556_STANAG30, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 280;

		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_mag_556_stanag30, CItem_Mag_556_STANAG30)
