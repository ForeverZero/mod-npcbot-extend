/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "NbeMgr.h"


class nbe_playerscript : public PlayerScript
{
public:
    nbe_playerscript() : PlayerScript("nbe_playerscript") { }

    void OnBeforeLogout(Player* player) {
        LOG_INFO("server.bot", "player {} logout, delete all bots", player->GetName());
        sNbeMgr->DelAllBot(player);
    }
};

// Add all scripts in one
void AddNbePlayerScripts()
{
    new nbe_playerscript();
}
