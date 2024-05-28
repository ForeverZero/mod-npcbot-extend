//
// Created by qq821 on 2024/5/28.
//

#ifndef NBEMGR_H
#define NBEMGR_H

#include "player.h"

class NbeMgr {
public:
    NbeMgr() = default;
    virtual ~NbeMgr() = default;
    static NbeMgr* instance();

public:
    void DelAllBot(Player* owner);
};


#define sNbeMgr NbeMgr::instance()
#endif //NBEMGR_H
