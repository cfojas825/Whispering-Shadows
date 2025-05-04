// ===== Enemy.cpp =====
#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>
#include <limits>

// Typewriter-style printer
static void slowPrint(const std::string& text, int delayMs = 35) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

// Random string picker
static const std::string& pick(const std::vector<std::string>& list) {
    return list[std::rand() % list.size()];
}

// Constructor
Enemy::Enemy(const std::string& name, int hp, int atk, int def)
    : name(name), health(hp), attackPower(atk), defense(def)
{
}

// Combat sequence
void Enemy::fight(Player& player) {
    // Existing combat setup code unchanged...

    // === Battle Loop ===
    while (health > 0 && player.isAlive()) {
        std::cout << "\n" << name << " HP: " << health << "\n";
        std::cout << player.getName() << " - HP: " << player.getCurrentHealth() << "\n";
        std::cout << "1. Attack\n2. Use Skill\n3. Use Item\n4. Flee\n> ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input, try again.\n";
            continue;
        }

    }

    player.resetStrength(); // Reset temporary buffs after combat

    
}
