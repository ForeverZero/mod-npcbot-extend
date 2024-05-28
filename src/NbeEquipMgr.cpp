//
// Created by Administrator on 2023/1/29.
//

#include "NbeEquipMgr.h"
#include "Item.h"

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

uint16 NbeEquipMgr::getMasterLevel(Player* player)
{
    // TODO level >= 80, return avg equip level
    return player->GetLevel();
}
