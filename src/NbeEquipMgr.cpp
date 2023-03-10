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
    LOG_WARN("server.bot", "equip {}", entry);

    Item* bItem = ai->GetEquips(slot);
    if (bItem && bItem->GetEntry() == entry)
        return;

    player->AddItem(entry, 1);
    Item* item = player->GetItemByEntry(entry);
    ai->Equip(slot, item, player->GetGUID());
}

void NbeEquipMgr::AutoEquip(Player* player, bot_ai* ai)
{
    uint8 spec = ai->GetSpec();
    LOG_WARN("server.bot", "auto equip spec {}", spec);
    if (equipTemplates.find(spec) == equipTemplates.end())
    {
        LOG_WARN("server.bot", "not templates found");
        return;
    }
    BotSpecLevelEquipTemplate bset = equipTemplates.at(spec);

    uint16 masterLevel = getMasterLevel(player);
    uint16 maxLevel = 0;

    for (auto& [level, ee] : bset)
    {
        LOG_WARN("server.bot", "bset {}", level);

        if (level > masterLevel || level < maxLevel)
            continue;
        maxLevel = level;
    }
    // not found
    if (maxLevel <= 0)
    {
        LOG_WARN("server.bot", "templates level {} not found", masterLevel);
        return;
    }

    // apply all equip entries
    EquipEntries ee = bset[maxLevel];
    for (uint8 slot = 0; slot < BOT_INVENTORY_SIZE; slot++)
    {
        LOG_WARN("server.bot", "templates level {} {} {}", masterLevel, slot, ee[slot]);

        if (uint32 entry = ee[slot])
        {
            Equip(player, ai, entry, slot);
        }
    }
}

void NbeEquipMgr::buildTemplates()
{
    buildWarriorTemplates();
    buildPriestTemplates();
    buildWarlockTemplates();
    buildRogueTemplates();
}

void NbeEquipMgr::buildWarriorTemplates()
{
    buildWarriorProtectionTemplates();
}

/*
    EquipEntries level_15;
    level_15[BOT_SLOT_MAINHAND] = 0;
    level_15[BOT_SLOT_OFFHAND] = 0;
    level_15[BOT_SLOT_RANGED] = 0;
    level_15[BOT_SLOT_HEAD] = 0;
    level_15[BOT_SLOT_SHOULDERS] = 0;
    level_15[BOT_SLOT_CHEST] = 0;
    level_15[BOT_SLOT_WAIST] = 0;
    level_15[BOT_SLOT_LEGS] = 0;
    level_15[BOT_SLOT_FEET] = 0;
    level_15[BOT_SLOT_WRIST] = 0;
    level_15[BOT_SLOT_HANDS] = 0;
    level_15[BOT_SLOT_BACK] = 0;
    level_15[BOT_SLOT_BODY] = 0;
    level_15[BOT_SLOT_FINGER1] = 0;
    level_15[BOT_SLOT_FINGER2] = 0;
    level_15[BOT_SLOT_TRINKET1] = 0;
    level_15[BOT_SLOT_TRINKET2] = 0;
    level_15[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(15, level_15);
*/

void NbeEquipMgr::buildWarriorProtectionTemplates()
{
    BotSpecLevelEquipTemplate bset;
    EquipEntries level_15;
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
    bset.insert_or_assign(15, level_15);

    EquipEntries level_30;
    level_30[BOT_SLOT_MAINHAND] = 3850;
    level_30[BOT_SLOT_OFFHAND] = 4477;
    level_30[BOT_SLOT_RANGED] = 11305;
    level_30[BOT_SLOT_HEAD] = 3836;
    level_30[BOT_SLOT_SHOULDERS] = 6404;
    level_30[BOT_SLOT_CHEST] = 7688;
    level_30[BOT_SLOT_WAIST] = 9405;
    level_30[BOT_SLOT_LEGS] = 3843;
    level_30[BOT_SLOT_FEET] = 4076;
    level_30[BOT_SLOT_WRIST] = 14759;
    level_30[BOT_SLOT_HANDS] = 23170;
    level_30[BOT_SLOT_BACK] = 18427;
    level_30[BOT_SLOT_BODY] = 0;
    level_30[BOT_SLOT_FINGER1] = 0;
    level_30[BOT_SLOT_FINGER2] = 0;
    level_30[BOT_SLOT_TRINKET1] = 0;
    level_30[BOT_SLOT_TRINKET2] = 0;
    level_30[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(30, level_30);

    equipTemplates[BOT_SPEC_WARRIOR_PROTECTION] = bset;
}

void NbeEquipMgr::buildPriestTemplates()
{
    buildPriestHolyTemplates();
}

void NbeEquipMgr::buildPriestHolyTemplates()
{
    BotSpecLevelEquipTemplate bset;
    EquipEntries level_15;
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
    bset.insert_or_assign(15, level_15);

    EquipEntries level_30;
    level_30[BOT_SLOT_MAINHAND] = 19569;
    level_30[BOT_SLOT_OFFHAND] = 0;
    level_30[BOT_SLOT_RANGED] = 7708;
    level_30[BOT_SLOT_HEAD] = 7691;
    level_30[BOT_SLOT_SHOULDERS] = 4718;
    level_30[BOT_SLOT_CHEST] = 7728;
    level_30[BOT_SLOT_WAIST] = 20099;
    level_30[BOT_SLOT_LEGS] = 2277;
    level_30[BOT_SLOT_FEET] = 9454;
    level_30[BOT_SLOT_WRIST] = 6047;
    level_30[BOT_SLOT_HANDS] = 9395;
    level_30[BOT_SLOT_BACK] = 5181;
    level_30[BOT_SLOT_BODY] = 0;
    level_30[BOT_SLOT_FINGER1] = 0;
    level_30[BOT_SLOT_FINGER2] = 0;
    level_30[BOT_SLOT_TRINKET1] = 0;
    level_30[BOT_SLOT_TRINKET2] = 0;
    level_30[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(30, level_30);

    equipTemplates[BOT_SPEC_PRIEST_HOLY] = bset;
    equipTemplates[BOT_SPEC_PRIEST_DISCIPLINE] = bset;

}

void NbeEquipMgr::buildWarlockTemplates()
{
    BotSpecLevelEquipTemplate bset;
    EquipEntries level_15;
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
    bset.insert_or_assign(15, level_15);

    EquipEntries level_30;
    level_30[BOT_SLOT_MAINHAND] = 19569;
    level_30[BOT_SLOT_OFFHAND] = 0;
    level_30[BOT_SLOT_RANGED] = 7708;
    level_30[BOT_SLOT_HEAD] = 7691;
    level_30[BOT_SLOT_SHOULDERS] = 4718;
    level_30[BOT_SLOT_CHEST] = 7728;
    level_30[BOT_SLOT_WAIST] = 20099;
    level_30[BOT_SLOT_LEGS] = 2277;
    level_30[BOT_SLOT_FEET] = 9454;
    level_30[BOT_SLOT_WRIST] = 6047;
    level_30[BOT_SLOT_HANDS] = 9395;
    level_30[BOT_SLOT_BACK] = 5181;
    level_30[BOT_SLOT_BODY] = 0;
    level_30[BOT_SLOT_FINGER1] = 0;
    level_30[BOT_SLOT_FINGER2] = 0;
    level_30[BOT_SLOT_TRINKET1] = 0;
    level_30[BOT_SLOT_TRINKET2] = 0;
    level_30[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(30, level_30);

    equipTemplates[BOT_SPEC_WARLOCK_DESTRUCTION] = bset;
    equipTemplates[BOT_SPEC_WARLOCK_AFFLICTION] = bset;
    equipTemplates[BOT_SPEC_WARLOCK_DEMONOLOGY] = bset;
}

void NbeEquipMgr::buildRogueTemplates()
{
    BotSpecLevelEquipTemplate bset;
    EquipEntries level_15;
    level_15[BOT_SLOT_MAINHAND] = 3184;
    level_15[BOT_SLOT_OFFHAND] = 3184;
    level_15[BOT_SLOT_RANGED] = 3107;
    level_15[BOT_SLOT_HEAD] = 0;
    level_15[BOT_SLOT_SHOULDERS] = 0;
    level_15[BOT_SLOT_CHEST] = 2317;
    level_15[BOT_SLOT_WAIST] = 5780;
    level_15[BOT_SLOT_LEGS] = 1934;
    level_15[BOT_SLOT_FEET] = 4788;
    level_15[BOT_SLOT_WRIST] = 14561;
    level_15[BOT_SLOT_HANDS] = 1314;
    level_15[BOT_SLOT_BACK] = 0;
    level_15[BOT_SLOT_BODY] = 0;
    level_15[BOT_SLOT_FINGER1] = 0;
    level_15[BOT_SLOT_FINGER2] = 0;
    level_15[BOT_SLOT_TRINKET1] = 0;
    level_15[BOT_SLOT_TRINKET2] = 0;
    level_15[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(15, level_15);

    EquipEntries level_30;
    level_30[BOT_SLOT_MAINHAND] = 7682;
    level_30[BOT_SLOT_OFFHAND] = 7682;
    level_30[BOT_SLOT_RANGED] = 3108;
    level_30[BOT_SLOT_HEAD] = 6204;
    level_30[BOT_SLOT_SHOULDERS] = 4721;
    level_30[BOT_SLOT_CHEST] = 7374;
    level_30[BOT_SLOT_WAIST] = 4257;
    level_30[BOT_SLOT_LEGS] = 4054;
    level_30[BOT_SLOT_FEET] = 4055;
    level_30[BOT_SLOT_WRIST] = 9428;
    level_30[BOT_SLOT_HANDS] = 7690;
    level_30[BOT_SLOT_BACK] = 14593;
    level_30[BOT_SLOT_BODY] = 0;
    level_30[BOT_SLOT_FINGER1] = 0;
    level_30[BOT_SLOT_FINGER2] = 0;
    level_30[BOT_SLOT_TRINKET1] = 0;
    level_30[BOT_SLOT_TRINKET2] = 0;
    level_30[BOT_SLOT_NECK] = 0;
    bset.insert_or_assign(30, level_30);

    equipTemplates[BOT_SPEC_ROGUE_COMBAT] = bset;
    equipTemplates[BOT_SPEC_ROGUE_ASSASINATION] = bset;
    equipTemplates[BOT_SPEC_ROGUE_SUBTLETY] = bset;
}

uint16 NbeEquipMgr::getMasterLevel(Player* player)
{
    // TODO level >= 80, return avg equip level
    return player->GetLevel();
}
