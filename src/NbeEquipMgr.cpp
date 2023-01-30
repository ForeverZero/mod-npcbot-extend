//
// Created by Administrator on 2023/1/29.
//

#include "NbeEquipMgr.h"
#include "Item.h"

NbeEquipMgr::NbeEquipMgr()
{
    buildTemplates();
}

NbeEquipMgr* NbeEquipMgr::instance()
{
    static NbeEquipMgr intance;
    return &intance;
}

void NbeEquipMgr::Equip(Player* player, bot_ai* ai, uint32 entry, uint8 slot)
{
    Item* item = Item::CreateItem(entry, 1, nullptr);
    ai->Equip(slot, item, player->GetGUID());
}

void NbeEquipMgr::AutoEquip(Player* player, bot_ai* ai)
{
    uint8 spec = ai->GetSpec();
    if (equipTemplates.find(spec) == equipTemplates.end())
        return;
    BotSpecLevelEquipTemplate bset = equipTemplates[spec];

    uint16 masterLevel = getMasterLevel(player);
    uint16 maxLevel = 0;

    for (auto& [level, ee] : bset)
    {
        if (level > masterLevel || level < maxLevel)
            continue;
        maxLevel = level;
    }
    // not found
    if (maxLevel <= 0)
        return;

    // apply all equip entries
    EquipEntries* ee = bset[maxLevel];
    for (uint8 slot = 0; slot < BOT_INVENTORY_SIZE; slot++)
    {
        if (uint32 entry = (*ee)[slot])
        {
            Equip(player, ai, entry, slot);
        }
    }
}

void NbeEquipMgr::buildTemplates()
{
    buildWarriorTemplates();
    buildPriestTemplates();
    buildWarlockDestroyTemplates();
}

void NbeEquipMgr::buildWarriorTemplates()
{
    buildWarriorProtectionTemplates();
}

void NbeEquipMgr::buildWarriorProtectionTemplates()
{
    BotSpecLevelEquipTemplate bset = equipTemplates[BOT_SPEC_WARRIOR_PROTECTION];

    EquipEntries level_15 = { 0 };
    level_15[BOT_SLOT_MAINHAND]     = 935;
    level_15[BOT_SLOT_OFFHAND]      = 5443;
    level_15[BOT_SLOT_RANGED]       = 11304;
    level_15[BOT_SLOT_HEAD]         = 0;
    level_15[BOT_SLOT_SHOULDERS]    = 0;
    level_15[BOT_SLOT_CHEST]        = 6731;
    level_15[BOT_SLOT_WAIST]        = 15510;
    level_15[BOT_SLOT_LEGS]         = 14727;
    level_15[BOT_SLOT_FEET]         = 1446;
    level_15[BOT_SLOT_WRIST]        = 3211;
    level_15[BOT_SLOT_HANDS]        = 1448;
    level_15[BOT_SLOT_BACK]         = 1190;
    level_15[BOT_SLOT_BODY]         = 0;
    level_15[BOT_SLOT_FINGER1]      = 0;
    level_15[BOT_SLOT_FINGER2]      = 0;
    level_15[BOT_SLOT_TRINKET1]     = 0;
    level_15[BOT_SLOT_TRINKET2]     = 0;
    level_15[BOT_SLOT_NECK]         = 0;
    bset.insert_or_assign(1, &level_15);
}

void NbeEquipMgr::buildPriestTemplates()
{
    buildPriestHolyTemplates();
}

void NbeEquipMgr::buildPriestHolyTemplates()
{
    BotSpecLevelEquipTemplate bset = equipTemplates[BOT_SPEC_PRIEST_HOLY];

    EquipEntries level_15 = { 0 };
    level_15[BOT_SLOT_MAINHAND] = 1300;
    level_15[BOT_SLOT_OFFHAND] = 0;
    level_15[BOT_SLOT_RANGED] = 0;
    level_15[BOT_SLOT_HEAD] = 0;
    level_15[BOT_SLOT_SHOULDERS] = 0;
    level_15[BOT_SLOT_CHEST] = 4781;
    level_15[BOT_SLOT_WAIST] = 4786;
    level_15[BOT_SLOT_LEGS] = 4434;
    level_15[BOT_SLOT_FEET] = 4313;
    level_15[BOT_SLOT_WRIST] = 6563;
    level_15[BOT_SLOT_HANDS] = 12977;
    level_15[BOT_SLOT_BACK] = 6632;
    level_15[BOT_SLOT_BODY] = 0;
    level_15[BOT_SLOT_FINGER1] = 0;
    level_15[BOT_SLOT_FINGER2] = 0;
    level_15[BOT_SLOT_TRINKET1] = 0;
    level_15[BOT_SLOT_TRINKET2] = 0;
    level_15[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(1, &level_15);
}

void NbeEquipMgr::buildWarlockTemplates()
{
    buildWarlockDestroyTemplates();
}

void NbeEquipMgr::buildWarlockDestroyTemplates()
{
    BotSpecLevelEquipTemplate bset = equipTemplates[BOT_SPEC_WARLOCK_DESTRUCTION];

    EquipEntries level_15 = { 0 };
    level_15[BOT_SLOT_MAINHAND] = 1300;
    level_15[BOT_SLOT_OFFHAND] = 0;
    level_15[BOT_SLOT_RANGED] = 0;
    level_15[BOT_SLOT_HEAD] = 0;
    level_15[BOT_SLOT_SHOULDERS] = 0;
    level_15[BOT_SLOT_CHEST] = 4781;
    level_15[BOT_SLOT_WAIST] = 4786;
    level_15[BOT_SLOT_LEGS] = 4434;
    level_15[BOT_SLOT_FEET] = 4313;
    level_15[BOT_SLOT_WRIST] = 6563;
    level_15[BOT_SLOT_HANDS] = 12977;
    level_15[BOT_SLOT_BACK] = 6632;
    level_15[BOT_SLOT_BODY] = 0;
    level_15[BOT_SLOT_FINGER1] = 0;
    level_15[BOT_SLOT_FINGER2] = 0;
    level_15[BOT_SLOT_TRINKET1] = 0;
    level_15[BOT_SLOT_TRINKET2] = 0;
    level_15[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(1, &level_15);
}

uint16 NbeEquipMgr::getMasterLevel(Player* player)
{
    // TODO level >= 80, return avg equip level
    return player->GetLevel();
}
