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

class CItem_Mag_9x19_18 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_9x19_18, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 252;

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
LINK_ENTITY_TO_CLASS(item_mag_9x19_18, CItem_Mag_9x19_18)

class CItem_Mag_45acp_12usp : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_45acp_12usp, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 253;

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
LINK_ENTITY_TO_CLASS(item_mag_45acp_12usp, CItem_Mag_45acp_12usp)

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

class CItem_Mag_45acp_25ump : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_45acp_25ump, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 273;

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
LINK_ENTITY_TO_CLASS(item_mag_45acp_25ump, CItem_Mag_45acp_25ump)

class CItem_Mag_57x28_20fs : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_57x28_20fs, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 251;

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
LINK_ENTITY_TO_CLASS(item_mag_57x28_20fs, CItem_Mag_57x28_20fs)

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

class CItem_Mag_46x30_40mp7 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_46x30_40mp7, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 272;

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
LINK_ENTITY_TO_CLASS(item_mag_46x30_40mp7, CItem_Mag_46x30_40mp7)

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

class CItem_Mag_357_Deagle9 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_357_Deagle9, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 261;

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
LINK_ENTITY_TO_CLASS(item_mag_357_deagle9, CItem_Mag_357_Deagle9)

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

class CItem_Mag_762N_SCAR20 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_762N_SCAR20, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 281;

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
LINK_ENTITY_TO_CLASS(item_mag_762n_scar20, CItem_Mag_762N_SCAR20)

class CItem_Mag_762x39_30 : public CItem
{
public:
	DECLARE_CLASS(CItem_Mag_762x39_30, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");

		// ItemID must be established before running the common spawn code
		m_iItemID = 282;

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
LINK_ENTITY_TO_CLASS(item_mag_762x39_30, CItem_Mag_762x39_30)
