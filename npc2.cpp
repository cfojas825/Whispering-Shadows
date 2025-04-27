#include "npc2.h"
#include <iostream>

NPC2::NPC2(std::string name, std::string item, int xp)
    : name(name), questItem(item), questGiven(false), questComplete(false), rewardXP(xp) {
}

void NPC2::interact(Player& player) {
    std::cout << name << ": \"I need a " << questItem << ". Can you find one for me?\"\n";

    if (!questGiven) {
        std::cout << "1. Accept\n2. Decline\n> ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            questGiven = true;
            std::cout << name << ": Thank you. I'll be waiting...\n";
        }
        else {
            std::cout << name << ": Perhaps another time.\n";
        }
    }
    else if (!questComplete) {
        bool foundItem = false;
        for (const auto& item : player.getInventory()) {
            if (item.name == questItem) {
                foundItem = true;
                break;
            }
        }
        if (foundItem) {
            questComplete = true;
            std::cout << name << ": You found the " << questItem << "! Here’s your reward.\n";
            player.gainXP(rewardXP);
        }
        else {
            std::cout << name << ": You haven’t found the " << questItem << " yet...\n";
        }
    }
    else {
        std::cout << name << ": You've already helped me. Thank you.\n";
    }
}