//
// Created by qq821 on 2024/5/28.
//

#include "NbeMgr.h"
#include "botmgr.h"
#include "bot_ai.h"
#include "creature.h"
#include "botdatamgr.h"

NbeMgr* NbeMgr::instance()
{
    static NbeMgr intance;
    return &intance;
}

void NbeMgr::DelAllBot(Player *owner) {
    BotMgr* botMgr = owner->GetBotMgr();
    BotMap* botMap = botMgr->GetBotMap();
    for (auto iter = botMap->begin(); iter != botMap->end(); ++iter)
    {
        Creature* bot = iter->second;
        bot->GetBotAI()->UnEquipAll(owner->GetGUID());
        owner->GetBotMgr()->RemoveBot(bot->GetGUID(), BOT_REMOVE_DISMISS);
        bot->CombatStop();
        bot->GetBotAI()->Reset();
        bot->GetBotAI()->canUpdate = false;
        bot->DeleteFromDB();
        bot->AddObjectToRemoveList();
        BotDataMgr::UpdateNpcBotData(bot->GetEntry(), NPCBOT_UPDATE_ERASE);
    }
}
