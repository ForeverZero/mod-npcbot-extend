/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "NbeEquipMgr.h"
#include "bot_ai.h"
#include "ObjectMgr.h"
#include "Creature.h"
#include "botdatamgr.h"


using namespace Acore::ChatCommands;

class nbe_commandscript : public CommandScript
{
public:
    nbe_commandscript() : CommandScript("nbe_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable nbeEquipCommandTable =
        {
            { "auto",     HandleEquipAutoCommand,        SEC_PLAYER,      Console::No  }
        };
        static ChatCommandTable nbeAddCommandTable =
        {
            { "id",     HandleAddIdCommand,        SEC_PLAYER,      Console::No  }
        };
        static ChatCommandTable nbeCommandTable = 
        {
            { "equip",     nbeEquipCommandTable},
            { "add",     nbeAddCommandTable},
        };
        static ChatCommandTable commandTable =
        {
            { "nbe", nbeCommandTable }
        };
        return commandTable;
    }

    static bool HandleEquipAutoCommand(ChatHandler* handler, char const* args)
    {
        Creature* bot = handler->getSelectedCreature();
        if (!bot || !bot->IsNPCBot())
        {
            handler->SendSysMessage("you should select a npcbot");
            return false;
        }

        Player* player = handler->GetPlayer();
        bot_ai* ai = bot->GetBotAI();
        sNbeEquipMgr->AutoEquip(player, ai);
        return true;
    }

    static bool HandleAddIdCommand(ChatHandler* handler, char const* args)
    {
        uint32 id = atoi(strtok((char*)args, " "));
        if (!id)
        {
            handler->SendSysMessage("invalid bot id");
            return false;
        }

        // add bot
        if (!addByBotId(handler, id))
            return false;

        // set ownner


        return true;
    }

private:
    static bool addByBotId(ChatHandler* handler, uint32 id)
    {
        CreatureTemplate const* creInfo = sObjectMgr->GetCreatureTemplate(id);
        if (!creInfo)
        {
            handler->PSendSysMessage("creature %u does not exist!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!(creInfo->flags_extra & CREATURE_FLAG_EXTRA_NPCBOT))
        {
            handler->PSendSysMessage("creature %u is not a npcbot!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (BotDataMgr::SelectNpcBotData(id))
        {
            handler->PSendSysMessage("Npcbot %u already exists in `characters_npcbot` table!", id);
            handler->SendSysMessage("If you want to move this bot to a new location use '.npcbot move' command");
            handler->SetSentErrorMessage(true);
            return false;
        }

        WorldDatabasePreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CREATURE_BY_ID);
        //"SELECT guid FROM creature WHERE id1 = ? OR id2 = ? OR id3 = ?", CONNECTION_SYNCH
        //stmt->setUInt32(0, id);
        stmt->SetArguments(id, id, id);
        PreparedQueryResult res2 = WorldDatabase.Query(stmt);
        if (res2)
        {
            handler->PSendSysMessage("Npcbot %u already exists in `creature` table!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* chr = handler->GetSession()->GetPlayer();

        if (/*Transport* trans = */chr->GetTransport())
        {
            handler->SendSysMessage("Cannot spawn bots on transport!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        //float x = chr->GetPositionX();
        //float y = chr->GetPositionY();
        //float z = chr->GetPositionZ();
        //float o = chr->GetOrientation();
        Map* map = chr->GetMap();

        if (map->Instanceable())
        {
            handler->SendSysMessage("Cannot spawn bots in instances!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* creature = new Creature();
        if (!creature->Create(map->GenerateLowGuid<HighGuid::Unit>(), map, chr->GetPhaseMaskForSpawn(), id, 0, chr->GetPositionX(), chr->GetPositionY(), chr->GetPositionZ(), chr->GetOrientation()))
        {
            delete creature;
            handler->SendSysMessage("Creature is not created!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        NpcBotExtras const* _botExtras = BotDataMgr::SelectNpcBotExtras(id);
        if (!_botExtras)
        {
            delete creature;
            handler->PSendSysMessage("No class/race data found for bot %u!", id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        BotDataMgr::AddNpcBotData(id, bot_ai::DefaultRolesForClass(_botExtras->bclass), bot_ai::DefaultSpecForClass(_botExtras->bclass), creature->GetCreatureTemplate()->faction);

        creature->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), chr->GetPhaseMaskForSpawn());

        uint32 db_guid = creature->GetSpawnId();
        if (!creature->LoadBotCreatureFromDB(db_guid, map))
        {
            delete creature;
            handler->SendSysMessage("Cannot load npcbot from DB!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        sObjectMgr->AddCreatureToGrid(db_guid, sObjectMgr->GetCreatureData(db_guid));

        handler->SendSysMessage("NpcBot successfully spawned");
        return true;
    }
};

// Add all scripts in one
void AddNbeScripts()
{
    new nbe_commandscript();
}
