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
    Item* item = Item::CreateItem(entry, 1, nullptr);
    ai->Equip(slot, item, player->GetGUID());
}
