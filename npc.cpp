#include "npc.h"
#include <iostream>

NPC::NPC(std::string name, int x, int y, std::string questDesc, int xp)
    : name(name), posX(x), posY(y), questDescription(questDesc),
    questAccepted(false), questCompleted(false), rewardXP(xp) {
}

int NPC::getX() const {
    return posX;
}

int NPC::getY() const {
    return posY;
}

void NPC::interact(Player& player) {
    std::cout << "\nYou meet " << name << ".\n";

    if (!questAccepted) {
        std::cout << "\"" << questDescription << "\"\n";
        std::cout << "1. Accept\n2. Decline\n> ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            questAccepted = true;
            std::cout << name << ": Good luck... You’ll need it.\n";
        }
        else {
            std::cout << name << ": Maybe next time.\n";
        }
    }
    else if (!questCompleted) {
        std::cout << name << ": Return once you have slain 1 wraith.\n";
    }
    else {
        std::cout << name << ": You have done well. Here is your reward.\n";
        player.gainXP(rewardXP);
        questAccepted = false;
        questCompleted = false;
    }
}
