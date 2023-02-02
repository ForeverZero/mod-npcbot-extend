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
#include "botmgr.h"


using namespace Acore::ChatCommands;

class nbe_commandscript : public CommandScript
{
public:
    nbe_commandscript() : CommandScript("nbe_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable nbeEquipCommandTable =
        {
            { "auto",     HandleEquipAutoCommand,        SEC_PLAYER,      Console::No  },
            { "autoall",     HandleEquipAutoAllCommand,        SEC_PLAYER,      Console::No  },
        };
        static ChatCommandTable nbeAddCommandTable =
        {
            { "id",     HandleAddIdCommand,        SEC_PLAYER,      Console::No  },
            { "zs",     HandleAddZsCommand,        SEC_PLAYER,      Console::No  },
            { "qs",     HandleAddQsCommand,        SEC_PLAYER,      Console::No  },
            { "lr",     HandleAddLrCommand,        SEC_PLAYER,      Console::No  },
            { "dz",     HandleAddDzCommand,        SEC_PLAYER,      Console::No  },
            { "ms",     HandleAddMsCommand,        SEC_PLAYER,      Console::No  },
            { "dk",     HandleAddDkCommand,        SEC_PLAYER,      Console::No  },
            { "sm",     HandleAddSmCommand,        SEC_PLAYER,      Console::No  },
            { "fs",     HandleAddFsCommand,        SEC_PLAYER,      Console::No  },
            { "ss",     HandleAddSsCommand,        SEC_PLAYER,      Console::No  },
            { "xd",     HandleAddXdCommand,        SEC_PLAYER,      Console::No  },
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

    static bool HandleEquipAutoAllCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetPlayer();
        BotMgr* botmgr = player->GetBotMgr();
        BotMap* map = botmgr->GetBotMap();

        for (auto& [_, creature] : *map)
        {
            bot_ai* ai = creature->GetBotAI();
            sNbeEquipMgr->AutoEquip(player, ai);
        }

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

        return true;
    }

    static bool HandleAddZsCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_WARRIOR);
    }
    static bool HandleAddQsCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_PALADIN);
    }
    static bool HandleAddLrCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_HUNTER);
    }
    static bool HandleAddDzCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_ROGUE);
    }
    static bool HandleAddMsCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_PRIEST);
    }
    static bool HandleAddDkCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_DEATH_KNIGHT);
    }
    static bool HandleAddSmCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_SHAMAN);
    }
    static bool HandleAddFsCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_MAGE);
    }
    static bool HandleAddSsCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_WARLOCK);
    }
    static bool HandleAddXdCommand(ChatHandler* handler, char const* args)
    {
        return addByBotClass(handler, BOT_CLASS_DRUID);
    }
private:
    static bool addByBotClass(ChatHandler* handler, uint8 botClass)
    {
        if (!botClass || botClass == BOT_CLASS_NONE || botClass >= BOT_CLASS_END)
        {
            handler->PSendSysMessage("invalid bot class");
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint8 localeIndex = handler->GetSessionDbLocaleIndex();
        CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
        for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
        {
            uint32 id = itr->second.Entry;

            if (id == BOT_ENTRY_MIRROR_IMAGE_BM)
                continue;
            //Blademaster disabled
            if (botClass == BOT_CLASS_BM)
                continue;

            NpcBotExtras const* _botExtras = BotDataMgr::SelectNpcBotExtras(id);
            if (!_botExtras || _botExtras->bclass != botClass)
                continue;

            if (BotDataMgr::SelectNpcBotData(id))
                continue;

            uint8 race = _botExtras->race;

            if (CreatureLocale const* creatureLocale = sObjectMgr->GetCreatureLocale(id))
            {
                if (creatureLocale->Name.size() > localeIndex && !creatureLocale->Name[localeIndex].empty())
                {
                    continue;
                }
            }

            std::string name = itr->second.Name;
            if (name.empty())
                continue;

            return addByBotId(handler, id);
        }

        handler->PSendSysMessage("not bot found");
        handler->SetSentErrorMessage(true);
        return false;
    }

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

        creature->GetBotAI()->SetBotOwner(handler->GetPlayer());
        return true;
    }
};

// Add all scripts in one
void AddNbeScripts()
{
    new nbe_commandscript();
}
