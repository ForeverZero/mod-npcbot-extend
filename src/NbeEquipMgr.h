//
// Created by Administrator on 2023/1/29.
//

#ifndef AZEROTHCORE_NBEEQUIPMGR_H
#define AZEROTHCORE_NBEEQUIPMGR_H

#include "bot_ai.h"
#include "Player.h"
#include "botcommon.h"
#include "ItemTemplate.h"

enum AllowableClass
{
    ALLOWABLE_CLASS_UNKNOWN = 0,

    ALLOWABLE_CLASS_WARRIOR = 1,
    ALLOWABLE_CLASS_PALADIN = 2,
    ALLOWABLE_CLASS_HUNTER = 4,
    ALLOWABLE_CLASS_ROUGE = 8,
    ALLOWABLE_CLASS_PRIEST = 16,
    ALLOWABLE_CLASS_DEATH_KNIGHT = 32,
    ALLOWABLE_CLASS_SHAMAN = 64,
    ALLOWABLE_CLASS_MAGE = 128,
    ALLOWABLE_CLASS_WARLOCK = 256,
    ALLOWABLE_CLASS_DRUID = 1024
};

class NbeEquipMgr {
public:
    NbeEquipMgr();
    virtual ~NbeEquipMgr() {}
    static NbeEquipMgr* instance();
public:
    void Equip(Player* player, bot_ai* ai, uint32 entry, uint8 slot);
    void AutoEquip(Player* player, bot_ai* ai);
private:
    ItemSubclassArmor getItemSubclassArmor(bot_ai* ai, uint8 level);
    ItemSubclassWeapon getItemSubclassWeapon(bot_ai* ai);
    void randomEquipArmorByMasterLevel(bot_ai* ai, uint8 botSlot, InventoryType inventoryType, ItemSubclassArmor subclass, AllowableClass allowableClass);
    void randomEquipWeaponByMasterLevel(bot_ai* ai, AllowableClass allowableClass);
    AllowableClass getAllowableClass(bot_ai* ai);
    std::string getItemModTypeArmorSql(bot_ai* ai);
};

#define sNbeEquipMgr NbeEquipMgr::instance()
#endif //AZEROTHCORE_NBEEQUIPMGR_H
