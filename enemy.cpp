#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>

// Typewriter effect
static void slowPrint(const std::string& text, int delayMs = 35) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

// Pick random string
static const std::string& pick(const std::vector<std::string>& list) {
    return list[std::rand() % list.size()];
}

// Constructor
Enemy::Enemy(const std::string& name, int hp, int atk, int def)
    : name(name), health(hp), attackPower(atk), defense(def) {
}

// Combat
void Enemy::fight(Player& player) {
    if (name == "Wraith") {
        std::vector<std::string> intros = {
            "From the mist drifts a Wraith, its form barely human...\n",
            "A chill wind heralds the arrival of a Wraith...\n",
            "You sense a presence—then the Wraith materializes!\n"
        };
        std::vector<std::string> quotes = {
            "Wraith: \"Your soul tastes sweetest when you despair...\"\n\n",
            "Wraith: \"I will consume your hope, bone by bone...\"\n\n",
            "Wraith: \"There is no light for you here...\"\n\n"
        };
        slowPrint(pick(intros));
        slowPrint(pick(quotes));
    }
    else {
        slowPrint("A strange entity reveals itself...\n\n");
    }

    int tempStrengthBoost = 0; // ?? Added strength boost

    while (health > 0 && player.isAlive()) {
        std::cout << "\n" << name << " HP: " << health << "\n";
        std::cout << player.getName() << " - HP: " << player.getCurrentHealth() << "\n";
        std::cout << "1. Attack\n2. Use Skill\n3. Use Item\n4. Flee\n> ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            slowPrint("You strike swiftly!");
            int baseDmg = std::rand() % 10 + 5;
            int dmg = baseDmg + tempStrengthBoost;  // ?? Apply strength boost
            std::cout << "You deal " << dmg << " damage!\n";
            health -= dmg;
        }
        else if (choice == 2) {
            slowPrint("You unleash a special attack!");
            int skillDmg = std::rand() % 15 + 10 + tempStrengthBoost; // ?? Boosted skill too
            std::cout << "Skill hits for " << skillDmg << " damage!\n";
            health -= skillDmg;
        }
        else if (choice == 3) {
            std::cin.ignore();
            const auto& inv = player.getInventory();
            if (inv.empty()) {
                std::cout << "Inventory is empty.\n";
                continue;
            }

            std::cout << "Choose an item to use:\n";
            for (size_t i = 0; i < inv.size(); ++i) {
                std::cout << (i + 1) << ". " << inv[i].name << " (" << inv[i].description << ")\n";
            }
            std::cout << "> ";
            int itemChoice;
            std::cin >> itemChoice;

            if (itemChoice >= 1 && itemChoice <= (int)inv.size()) {
                const Item& selectedItem = inv[itemChoice - 1];

                if (selectedItem.type == ItemType::POTION) {
                    slowPrint("You use " + selectedItem.name + "...");
                    player.heal(selectedItem.effectAmount);
                    player.getInventory().erase(player.getInventory().begin() + (itemChoice - 1));
                }
                else if (selectedItem.type == ItemType::ARTIFACT || selectedItem.type == ItemType::WEAPON) {
                    slowPrint("You use " + selectedItem.name + "...");
                    tempStrengthBoost += selectedItem.effectAmount; // ?? Boost attack
                    std::cout << "You feel a surge of power! +" << selectedItem.effectAmount << " attack for this fight!\n";
                    player.getInventory().erase(player.getInventory().begin() + (itemChoice - 1));
                }
                else {
                    std::cout << "That item can't be used now.\n";
                }
            }
            else {
                std::cout << "Invalid item choice.\n";
            }
        }
        else if (choice == 4) {
            slowPrint("You attempt to flee...");
            if (std::rand() % 100 < 50) {
                slowPrint("You escaped safely!");
                return;
            }
            else {
                slowPrint("You failed to flee!");
            }
        }
        else {
            std::cout << "Invalid choice.\n";
        }

        if (health > 0) {
            slowPrint(name + " counterattacks!");
            int dmg = std::max(1, attackPower - (std::rand() % 5));
            std::cout << name << " deals " << dmg << " damage!\n";
            player.takeDamage(dmg);
        }
    }

    if (health <= 0) {
        slowPrint(name + " collapses into nothingness...\n");
        int xpEarned = 20 + std::rand() % 15;
        player.gainXP(xpEarned);
        std::cout << "You earned " << xpEarned << " XP!\n";
    }
}
