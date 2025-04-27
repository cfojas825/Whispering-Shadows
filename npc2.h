#ifndef NPC2_H
#define NPC2_H

#include <string>
#include "player.h"

class NPC2 {
private:
    std::string name;
    std::string questItem;
    bool questGiven;
    bool questComplete;
    int rewardXP;

public:
    NPC2(std::string name, std::string item, int xp);
    void interact(Player& player);
};

#endif