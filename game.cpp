#include "game.h"
#include "enemy.h" 

#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>

// ANSI colors
static constexpr const char* RED = "\033[31m";
static constexpr const char* RESET = "\033[0m";

// Clear screen
static void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

// Typewriter text
static void slowPrint(const std::string& text, int delayMs = 35) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << "\n";
}

// Heartbeat effect
static void heartbeat(int beats = 3, int delayMs = 300) {
    for (int i = 0; i < beats; ++i) {
        std::cout << RED << "<3" << RESET << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        std::cout << "\b \b" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << "\n";
}

Game::Game() : isRunning(true) {}

void Game::mainMenu() {
    clearScreen();
    int choice;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << RED;
    slowPrint("...Silence...");
    heartbeat(4, 200);
    slowPrint("A distant drip echoes in the darkness...");
    heartbeat(2, 150);
    slowPrint("Your breath fogs before you as the air grows cold...");
    slowPrint("Suddenly, the lights flicker and die...");
    std::cout << RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    std::cout << RED << "\n=== Whispering Shadows ===\n" << RESET
              << "1. New Game\n"
              << "2. Load Game\n"
              << "3. Quit\n> ";
    std::cin >> choice;

    switch (choice) {
        case 1: startNewGame(); break;
        case 2: loadGame();     break;
        case 3: isRunning = false; return;
        default: std::cout << RED << "Invalid option." << RESET << "\n";
    }
}

void Game::startNewGame() {
    std::string name;
    int classChoice, regionChoice;

    std::cout << "Enter your character's name: ";
    std::cin >> name;

    clearScreen();
    std::cout << "Welcome to Whispering Shadows, " << name << "!\n\n";
    std::cout << "Controls:\n";
    std::cout << "  - Move: W/A/S/D\n";
    std::cout << "  - Open Inventory: I\n";
    std::cout << "  - View Map: M\n";
    std::cout << "  - Save Game: P\n";
    std::cout << "  - Quit Game: Q\n\n";
    std::cout << "In battle:\n";
    std::cout << "  - 1: Attack\n";
    std::cout << "  - 2: Use Skill\n";
    std::cout << "  - 3: Use Item\n";
    std::cout << "  - 4: Flee\n\n";
    std::cout << "Survive, complete quests, and defeat the final horror!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();

    clearScreen();
    std::cout << "Choose class: 1 Occultist  2 Hunter  3 Survivor > ";
    std::cin >> classChoice;

    std::cout << "\nChoose region:\n"
              << "1. Ancient Catacombs (12×8)\n"
              << "2. Derelict Asylum (15×12)\n"
              << "3. Shadow Labyrinth (10×6)\n> ";
    std::cin >> regionChoice;

    player = Player(name, static_cast<PlayerClass>(classChoice - 1));
    if (regionChoice >= 1 && regionChoice <= static_cast<int>(regions.size())) {
        int w = regions[regionChoice - 1].first;
        int h = regions[regionChoice - 1].second;
        world = Map(w, h);
        regionIndex = regionChoice - 1;
    } else {
        world = Map(regions[0].first, regions[0].second);
        regionIndex = 0;
    }

    gameLoop();
}

void Game::loadGame() {
    player.load("savefile.txt");
    world.load("mapfile.txt");
    gameLoop();
}

void Game::saveGame() {
    player.save("savefile.txt");
    world.save("mapfile.txt");
    std::cout << RED << "Game saved.\n" << RESET;
}

void Game::gameLoop() {
    const int requiredLevel = 5;
    while (player.isAlive() && isRunning) {
        int cx = world.getWidth() / 2;
        int cy = world.getHeight() / 2;

        if (player.getX() == cx && player.getY() == cy) {
            if (player.getLevel() < requiredLevel) {
                std::cout << RED
                          << "A sealed door bars your path -- reach level "
                          << requiredLevel << " first.\n" << RESET;
                player.setPosition(cx, cy - 1);
                continue;
            }

            int level = player.getLevel();
            int hp = 200 + static_cast<int>(level * 30);  // ✅ cast to avoid overflow warning
            int atk = 15 + static_cast<int>(level * 3);   // ✅ same here
            Enemy boss("Eldritch Horror", hp, atk, level);
            std::cout << RED;
            slowPrint("You enter the final chamber...");
            std::cout << RESET;
            boss.fight(player);
            std::cout << (player.isAlive() ? "\nYOU WIN!\n" : "\nGAME OVER\n");
            isRunning = false;
            return;
        }

        handleEvent();
    }

    if (!player.isAlive())
        std::cout << RED << "You have perished...\n" << RESET;
}

void Game::handleEvent() {
    char cmd;
    std::cout << RED << "\nMove (W/S/D/A), I=Inv, M=Map, P=Save, Q=Quit > " << RESET;
    std::cin >> cmd;
    cmd = std::toupper(cmd);

    int dx = 0, dy = 0;
    if (cmd == 'W') dy = -1;
    else if (cmd == 'S') dy = 1;
    else if (cmd == 'A') dx = -1;
    else if (cmd == 'D') dx = 1;
    else if (cmd == 'I') { player.showInventory(); return; }
    else if (cmd == 'M') { world.display(player); return; }
    else if (cmd == 'P') { saveGame(); return; }
    else if (cmd == 'Q') { isRunning = false; return; }
    else {
        std::cout << "Invalid input.\n";
        return;
    }

    int nx = player.getX() + dx;
    int ny = player.getY() + dy;
    int w = world.getWidth();
    int h = world.getHeight();

    if (nx < 0 || nx >= w || ny < 0 || ny >= h) {
        regionIndex = (regionIndex + 1) % regions.size();
        int rw = regions[regionIndex].first;
        int rh = regions[regionIndex].second;
        world = Map(rw, rh);
        int px = (nx < 0 ? rw - 1 : (nx >= w ? 0 : nx));
        int py = (ny < 0 ? rh - 1 : (ny >= h ? 0 : ny));
        player.setPosition(px, py);
        world.display(player);
    } else {
        world.movePlayer(player, dx, dy);
    }
}

void Game::run() {
    while (isRunning) mainMenu();
}
