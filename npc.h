#ifndef NPC_H
#define NPC_H

#include <string>
#include "player.h"

class NPC {
private:
    std::string name;
    std::string questDescription;
    int posX, posY;
    bool questAccepted;
    bool questCompleted;
    int rewardXP;

public:
    NPC(std::string name, int x, int y, std::string questDesc, int xp);

    void interact(Player& player);
    int getX() const;
    int getY() const;

    
    bool isQuestAccepted() const { return questAccepted; }
    bool isQuestCompleted() const { return questCompleted; }
    void setQuestState(bool accepted, bool completed) {
        questAccepted = accepted;
        questCompleted = completed;
    }
};

#endif // NPC_H
