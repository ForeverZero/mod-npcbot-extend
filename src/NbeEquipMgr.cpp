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
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_HEAD, INVTYPE_HEAD, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_SHOULDERS, INVTYPE_SHOULDERS, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_CHEST, INVTYPE_CHEST, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_WAIST, INVTYPE_WAIST, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_LEGS, INVTYPE_LEGS, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_FEET, INVTYPE_FEET, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_WRIST, INVTYPE_WRISTS, subclass);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_HANDS, INVTYPE_HANDS, subclass);

    randomEquipArmorByMasterLevel(ai, BOT_SLOT_BACK, INVTYPE_CLOAK, ITEM_SUBCLASS_ARMOR_MISC);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_NECK, INVTYPE_NECK, ITEM_SUBCLASS_ARMOR_MISC);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_FINGER1, INVTYPE_FINGER, ITEM_SUBCLASS_ARMOR_MISC);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_FINGER2, INVTYPE_FINGER, ITEM_SUBCLASS_ARMOR_MISC);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_TRINKET1, INVTYPE_TRINKET, ITEM_SUBCLASS_ARMOR_MISC);
    randomEquipArmorByMasterLevel(ai, BOT_SLOT_TRINKET2, INVTYPE_TRINKET, ITEM_SUBCLASS_ARMOR_MISC);
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

void NbeEquipMgr::randomEquipArmorByMasterLevel(bot_ai* ai, uint8 botSlot, InventoryType inventoryType, ItemSubclassArmor subclass)
{
    uint8 toLevel = ai->GetBotOwner()->GetLevel();
    uint8 fromLevel = toLevel - 5;
    Player* owner = ai->GetBotOwner();
    ObjectGuid botGuid = ai->GetGUID();
    LOG_INFO("server.bot", "random equip bot armor by owner level for player {}", owner->GetPlayerName());

    std::string sql = "select entry, name from item_template";
    sql += " where class = 4 and subclass = {} and InventoryType = {} and RequiredLevel between {} and {}";
    sql += " order by rand() limit 1";

    QueryResult rst = WorldDatabase.Query(sql, subclass, inventoryType, fromLevel, toLevel);
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
