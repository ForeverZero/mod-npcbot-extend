//
// Created by Administrator on 2023/1/29.
//

#ifndef AZEROTHCORE_NBEEQUIPMGR_H
#define AZEROTHCORE_NBEEQUIPMGR_H

#include "bot_ai.h"
#include "Player.h"
#include "botcommon.h"
#include "ItemTemplate.h"

typedef std::map<uint8, uint32> EquipEntries;

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
    void randomEquipArmorByMasterLevel(bot_ai* ai, uint8 botSlot, InventoryType inventoryType, ItemSubclassArmor subclass);
};

#define sNbeEquipMgr NbeEquipMgr::instance()
#endif //AZEROTHCORE_NBEEQUIPMGR_H
