#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>
#include <limits>

// Typewriter effect
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
    : name(name), health(hp), attackPower(atk), defense(def) {}

// Combat
void Enemy::fight(Player& player) {
    std::vector<std::string> intros;
    std::vector<std::string> quotes;

    if (name == "Wraith") {
        intros = {
            "From the mist drifts a Wraith, its form barely human...\n",
            "A chill wind heralds the arrival of a Wraith...\n",
            "You sense a presence—then the Wraith materializes!\n"
        };
        quotes = {
            "Wraith: \"Your soul tastes sweetest when you despair...\"\n\n",
            "Wraith: \"I will consume your hope, bone by bone...\"\n\n",
            "Wraith: \"There is no light for you here...\"\n\n"
        };
    } else if (name == "Faceless One") {
        intros = {
            "A towering Faceless One looms, its visage blank...\n",
            "You hear distant footsteps—then the Faceless One appears.\n",
            "Shadows twist into the shape of a Faceless One...\n"
        };
        quotes = {
            "Faceless One: \"I have taken more faces than you have lifetimes...\"\n\n",
            "Faceless One: \"Your identity is mine to claim...\"\n\n",
            "Faceless One: \"Can you survive without a name?\"\n\n"
        };
    } else if (name == "Crawling Horror") {
        intros = {
            "Something slithers at your feet—a Crawling Horror emerges!\n",
            "A sickening skittering—then the Crawling Horror crawls forth.\n",
            "You smell rot—then a Crawling Horror bursts into view.\n"
        };
        quotes = {
            "Crawling Horror: \"I will drag you into the abyss beneath these halls...\"\n\n",
            "Crawling Horror: \"Your flesh will feed my hunger...\"\n\n",
            "Crawling Horror: \"No corner is safe from my claws...\"\n\n"
        };
    } else if (name == "Whispering Shadow") {
        intros = {
            "Shadows swirl and coalesce into a Whispering Shadow...\n",
            "A soft voice beckons—you see the Whispering Shadow.\n",
            "Darkness itself forms into the shape of a Whispering Shadow.\n"
        };
        quotes = {
            "Whispering Shadow: \"Come closer... let me show you your darkest fear...\"\n\n",
            "Whispering Shadow: \"Your mind is mine to unravel...\"\n\n",
            "Whispering Shadow: \"Can you hear the voices behind you?\"\n\n"
        };
    } else {
        intros = { "A strange entity reveals itself...\n" };
        quotes = { "" };
    }

    slowPrint(pick(intros));
    slowPrint(pick(quotes));

    // Battle Loop
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

        if (choice == 1) {
            slowPrint(pick({
                "You swing your blade in a wide arc...\n",
                "You drive your sword forward...\n",
                "You slash downward with brutal force...\n",
                "You feint left, then strike fast...\n",
                "You lunge forward with a crushing blow...\n"
            }));
            int dmg = std::rand() % 10 + 5;
            dmg = std::max(0, dmg - (defense / 2));
            std::cout << "You deal " << dmg << " damage!\n";
            health -= dmg;
        } else if (choice == 2) {
            slowPrint(pick({
                "You channel dark energy around your hands...\n",
                "You summon forbidden power from beyond...\n",
                "You unleash an arcane blast of fury...\n"
            }));
            int skillDmg = std::rand() % 15 + 10;
            skillDmg = std::max(0, skillDmg - (defense / 3));
            std::cout << "Skill hits for " << skillDmg << " damage!\n";
            health -= skillDmg;
        } else if (choice == 3) {
            std::cin.ignore();
            slowPrint(pick({
                "You rummage through your pack...\n",
                "You fumble for a potion...\n"
            }));
            player.showInventory();
            std::cout << "Enter item number > ";
            std::string input;
            std::getline(std::cin, input);
            int itemIndex = std::stoi(input) - 1;

            if (itemIndex >= 0 && itemIndex < static_cast<int>(player.getInventory().size())) {
                auto& it = player.getInventory()[itemIndex];
                if (it.type == ItemType::POTION) {
                    slowPrint("You apply the " + it.name + "...\n");
                    player.heal(it.effectAmount);
                    player.getInventory().erase(player.getInventory().begin() + itemIndex);
                } else if (it.name == "Adrenaline Shot") {
                    slowPrint("You inject the Adrenaline Shot!\n");
                    player.addTempStrength(it.effectAmount);
                    player.getInventory().erase(player.getInventory().begin() + itemIndex);
                } else {
                    std::cout << "That item can't be used now.\n";
                }
            } else {
                std::cout << "Invalid selection.\n";
            }
        } else if (choice == 4) {
            slowPrint(pick({
                "You try to escape...\n",
                "You turn and run...\n",
                "You dash into the darkness...\n"
            }));
            if (std::rand() % 100 < 50) {
                slowPrint("You escape successfully!\n");
                player.resetStrength(); // Temporary buffs removed
                return;
            } else {
                slowPrint("You fail to flee!\n");
            }
        } else {
            std::cout << "Invalid choice.\n";
        }

        // Enemy counterattack
        if (health > 0) {
            slowPrint(pick({
                name + " lunges at you...\n",
                name + " slashes through the dark...\n",
                name + " lets out a roar and strikes...\n"
            }));
            int dmg = std::max(1, attackPower - (std::rand() % 5));
            std::cout << name << " deals " << dmg << " damage!\n";
            player.takeDamage(dmg);
        }
    }

    // Reset strength buff
    player.resetStrength();

    // Death result
    if (health <= 0) {
        slowPrint(pick({
            "With a final shriek, the " + name + " dissolves...\n",
            "The " + name + " collapses into shadows...\n",
            "The " + name + " vanishes with a scream...\n"
        }));
        int xpEarned = 0;
        if (name == "Wraith") xpEarned = 20;
        else if (name == "Faceless One") xpEarned = 30;
        else if (name == "Crawling Horror") xpEarned = 25;
        else if (name == "Whispering Shadow") xpEarned = 35;
        else xpEarned = 10;

        player.gainXP(xpEarned);
        player.addMonsterKill();
        std::cout << "You earned " << xpEarned << " XP!\n";
    }
}
