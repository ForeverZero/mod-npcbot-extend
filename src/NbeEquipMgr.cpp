//
// Created by Administrator on 2023/1/29.
//

#include "NbeEquipMgr.h"
#include "Item.h"
#include "ItemTemplate.h"

NbeEquipMgr::NbeEquipMgr()
{
    
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
    uint8 level = player->GetLevel();
    if (level < 15)
    {
        LOG_INFO("server.bot", "player {}, level less then 15, cannot auto equip bot", player->GetName());
        return;
    }

    ItemSubclassArmor subclass = getItemSubclassArmor(ai, level);
    AllowableClass ac = getAllowableClass(ai);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_HEAD, INVTYPE_HEAD, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_SHOULDERS, INVTYPE_SHOULDERS, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_CHEST, INVTYPE_CHEST, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_WAIST, INVTYPE_WAIST, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_LEGS, INVTYPE_LEGS, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_FEET, INVTYPE_FEET, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_WRIST, INVTYPE_WRISTS, subclass, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_HANDS, INVTYPE_HANDS, subclass, ac);

    randomEquipArmorByMasterLevel(ai, BOT_SLOT_BACK, INVTYPE_CLOAK, ITEM_SUBCLASS_ARMOR_MISC, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_NECK, INVTYPE_NECK, ITEM_SUBCLASS_ARMOR_MISC, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_FINGER1, INVTYPE_FINGER, ITEM_SUBCLASS_ARMOR_MISC, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_FINGER2, INVTYPE_FINGER, ITEM_SUBCLASS_ARMOR_MISC, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_TRINKET1, INVTYPE_TRINKET, ITEM_SUBCLASS_ARMOR_MISC, ac);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_TRINKET2, INVTYPE_TRINKET, ITEM_SUBCLASS_ARMOR_MISC, ac);

    randomEquipWeaponByMasterLevel(ai);
}

ItemSubclassArmor NbeEquipMgr::getItemSubclassArmor(bot_ai* ai, uint8 level)
{
    uint8 botClass = ai->GetBotClass();
    if (botClass == BOT_CLASS_DRUID || botClass == BOT_CLASS_ROGUE)
    {
        return ITEM_SUBCLASS_ARMOR_LEATHER;
    }

    if (botClass == BOT_CLASS_DEATH_KNIGHT)
    {
        return ITEM_SUBCLASS_ARMOR_PLATE;
    }
    if (botClass == BOT_CLASS_SHAMAN || botClass == BOT_CLASS_HUNTER)
    {
        return level < 40 ? ITEM_SUBCLASS_ARMOR_LEATHER : ITEM_SUBCLASS_ARMOR_MAIL;
    }
    if (botClass == BOT_CLASS_WARRIOR || botClass == BOT_CLASS_PALADIN)
    {
        return level < 40 ? ITEM_SUBCLASS_ARMOR_MAIL : ITEM_SUBCLASS_ARMOR_PLATE;
    }
    return ITEM_SUBCLASS_ARMOR_CLOTH;
}

ItemSubclassWeapon NbeEquipMgr::getItemSubclassWeaponByMasterLevel(bot_ai* ai)
{
    uint8 botClass = ai->GetBotClass();
    if (botClass == BOT_CLASS_MAGE || botClass == BOT_CLASS_WARLOCK || botClass == BOT_CLASS_PRIEST || botClass == BOT_CLASS_SHAMAN || botClass == BOT_CLASS_DRUID)
    {
        return ITEM_SUBCLASS_WEAPON_MACE2;
    }
    if (botClass == BOT_CLASS_WARRIOR || botClass == BOT_CLASS_PALADIN)
    {
        uint8 spec = ai->GetSpec();
        if (spec == BOT_SPEC_WARRIOR_PROTECTION || spec == BOT_SPEC_PALADIN_PROTECTION)
        {
            return ITEM_SUBCLASS_WEAPON_SWORD;
        }
        else
        {
            return ITEM_SUBCLASS_WEAPON_SWORD2;
        }
    }
    if (botClass == BOT_CLASS_DEATH_KNIGHT || botClass == BOT_CLASS_HUNTER)
    {
        return ITEM_SUBCLASS_WEAPON_SWORD;
    }
    if (botClass == BOT_CLASS_ROGUE)
    {
        return ITEM_SUBCLASS_WEAPON_DAGGER;
    }
    return ITEM_SUBCLASS_WEAPON_FISHING_POLE;
}

void NbeEquipMgr::randomEquipArmorByMasterLevel(bot_ai* ai, uint8 botSlot, InventoryType inventoryType, ItemSubclassArmor subclass, AllowableClass allowableClass)
{
    uint8 toLevel = ai->GetBotOwner()->GetLevel();
    uint8 fromLevel = toLevel - 5;
    Player* owner = ai->GetBotOwner();
    ObjectGuid botGuid = ai->GetGUID();
    LOG_INFO("server.bot", "random equip bot armor by owner level for player {}", owner->GetPlayerName());

    std::string statSql = getItemModTypeArmorSql(ai);

    std::string sql = "select entry, name from item_template";
    sql += " where class = 4 and subclass = {} and InventoryType = {} and RequiredLevel between {} and {}";
    sql += " and (AllowableClass = -1 or AllowableClass & {} > 0)";
    sql += " and (stat_type1 in" + statSql + "or stat_type2 in" + statSql + "or stat_type3 in" + statSql + "or stat_type4 in" + statSql
        + "or stat_type5 in" + statSql + "or stat_type6 in" + statSql + "or stat_type7 in" + statSql + "or stat_type8 in" + statSql
        +  ")";
    sql += " order by rand() limit 1";

    QueryResult rst = WorldDatabase.Query(sql, subclass, inventoryType, fromLevel, toLevel, allowableClass);
    if (!rst)
    {
        LOG_WARN("server.bot", "unable to find equip slot {} type {} for bot {}", botSlot, inventoryType, botGuid.GetCounter());
        return;
    }

    Field* field = rst->Fetch();
    uint32 entry = field[0].Get<uint32>();
    std::string name = field[1].Get<std::string>();
    LOG_INFO("server.bot", "equip {} - {} for bot {}", entry, name, botGuid.GetCounter());
    Equip(owner, ai, entry, botSlot);
}

void NbeEquipMgr::randomEquipWeaponByMasterLevel(bot_ai* ai)
{

}

AllowableClass NbeEquipMgr::getAllowableClass(bot_ai* ai)
{
    uint8 botClass = ai->GetBotClass();
    switch (botClass)
    {
    case BOT_CLASS_MAGE:
        return ALLOWABLE_CLASS_MAGE;
    case BOT_CLASS_PRIEST:
        return ALLOWABLE_CLASS_PRIEST;
    case BOT_CLASS_WARLOCK:
        return ALLOWABLE_CLASS_WARLOCK;
    case BOT_CLASS_ROGUE:
        return ALLOWABLE_CLASS_ROUGE;
    case BOT_CLASS_DRUID:
        return ALLOWABLE_CLASS_DRUID;
    case BOT_CLASS_HUNTER:
        return ALLOWABLE_CLASS_HUNTER;
    case BOT_CLASS_SHAMAN:
        return ALLOWABLE_CLASS_SHAMAN;
    case BOT_CLASS_WARRIOR:
        return ALLOWABLE_CLASS_WARRIOR;
    case BOT_CLASS_PALADIN:
        return ALLOWABLE_CLASS_PALADIN;
    case BOT_CLASS_DEATH_KNIGHT:
        return ALLOWABLE_CLASS_DEATH_KNIGHT;
    default:
        return ALLOWABLE_CLASS_UNKNOWN;
    }
}

std::string NbeEquipMgr::getItemModTypeArmorSql(bot_ai* ai)
{
    // HEALER
    uint8 spec = ai->GetSpec();
    if (spec == BOT_SPEC_DRUID_RESTORATION || spec == BOT_SPEC_SHAMAN_RESTORATION || spec == BOT_SPEC_PRIEST_HOLY
        || spec == BOT_SPEC_PRIEST_DISCIPLINE || spec == BOT_SPEC_PALADIN_HOLY)
    {
        return "(6, 5, 41, 45)";
    }

    // TANK
    if ((spec == BOT_SPEC_DRUID_FERAL && ai->HasRole(BOT_ROLE_TANK)) || spec == BOT_SPEC_WARRIOR_PROTECTION
        || spec == BOT_SPEC_PALADIN_PROTECTION || spec == BOT_SPEC_DK_BLOOD)
    {
        return "(1, 7, 12, 13, 14, 15, 22, 23, 24, 25, 26, 27, 33, 34, 48)";
    }

    // DPS
    uint8 cls = ai->GetBotClass();
    if (cls == BOT_CLASS_PRIEST || cls == BOT_CLASS_WARLOCK || cls == BOT_CLASS_MAGE || cls == BOT_CLASS_SHAMAN
        ||  spec == BOT_SPEC_DRUID_BALANCE || spec == BOT_SPEC_SHAMAN_ELEMENTAL)
    {
        return "(5, 18, 21, 30, 32, 36, 42, 45, 47)";
    }

    if (cls == BOT_CLASS_HUNTER)
    {
        return "(3, 17, 20, 29, 31, 32, 36, 39, 44)";
    }
    if (spec == BOT_SPEC_DRUID_FERAL || cls == BOT_CLASS_ROGUE || spec == BOT_SPEC_SHAMAN_ENHANCEMENT)
    {
        return "(3, 16, 19, 28, 31, 32, 38, 44)";
    }
    if (cls == BOT_CLASS_WARRIOR || cls == BOT_CLASS_PALADIN || cls == BOT_CLASS_DEATH_KNIGHT)
    {
        return "(4, 16, 19, 28, 31, 32, 36, 38, 44)";
    }

    LOG_WARN("server.bot", "getItemModTypeArmorSql failed for bot {}", ai->GetGUID().GetCounter());
    return "(0)";
}
