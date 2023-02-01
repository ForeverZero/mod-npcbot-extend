//
// Created by Administrator on 2023/1/29.
//

#ifndef AZEROTHCORE_NBEEQUIPMGR_H
#define AZEROTHCORE_NBEEQUIPMGR_H

#include "bot_ai.h"
#include "Player.h"
#include "botcommon.h"

typedef std::map<uint8, uint32> EquipEntries;
typedef std::map<uint16, EquipEntries> BotSpecLevelEquipTemplate;

class NbeEquipMgr {
public:
    NbeEquipMgr();
    virtual ~NbeEquipMgr() {}
    static NbeEquipMgr* instance();
public:
    void Equip(Player* player, bot_ai* ai, uint32 entry, uint8 slot);
    void AutoEquip(Player* player, bot_ai* ai);
private:
    void buildTemplates();

    void buildWarriorTemplates();
    void buildWarriorProtectionTemplates();

    void buildPriestTemplates();
    void buildPriestHolyTemplates();

    void buildWarlockTemplates();
    void buildWarlockDestroyTemplates();
private:
    std::map<uint8, BotSpecLevelEquipTemplate> equipTemplates;
    uint16 getMasterLevel(Player*);
};

#define sNbeEquipMgr NbeEquipMgr::instance()
#endif //AZEROTHCORE_NBEEQUIPMGR_H
