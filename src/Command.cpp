/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"

using namespace Acore::ChatCommands;

class nbe_commandscript : public CommandScript
{
public:
    nbe_commandscript() : CommandScript("nbe_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable nbeEquipCommandTable =
        {
            { "equip",     HandleEquipEquipCommand,        SEC_PLAYER,      Console::No  }
        };
        static ChatCommandTable nbeCommandTable = 
        {
            { "equip",     nbeEquipCommandTable}
        };
        static ChatCommandTable commandTable =
        {
            { "nbe", nbeCommandTable }
        };
        return commandTable;
    }

    static bool HandleEquipEquipCommand(ChatHandler* handler, Optional<bool> enableArg)
    {
        Creature* bot = handler->getSelectedCreature();
        if (!bot || !bot->IsNPCBot())
        {
            handler->SendSysMessage("you should select a npcbot");
            return true;

        }

        return true;
    }
};

// Add all scripts in one
void AddNbeScripts()
{
    new nbe_commandscript();
}
