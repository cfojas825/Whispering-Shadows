#include "map.h"
#include "enemy.h"
#include "item.h"
#include "npc.h"
#include "player.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// ANSI colors & clearScreen
static constexpr const char* RESET = "\033[0m";
static constexpr const char* RED = "\033[31m";
static constexpr const char* GREEN = "\033[32m";
static constexpr const char* YELLOW = "\033[33m";
static constexpr const char* MAGENTA = "\033[35m";
static constexpr const char* BLUE = "\033[34m";
static constexpr const char* WHITE = "\033[37m";
static constexpr const char* GRAY = "\033[90m";

static void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void Map::initRandomTerrain() {
    std::srand((unsigned)std::time(nullptr));
    grid.assign(height, std::vector<char>(width, 'E'));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r = std::rand() % 100;
            if (r < 8) grid[y][x] = 'W'; // water
            else if (r < 20) grid[y][x] = 'M'; // monster spawn
            else if (r < 30) grid[y][x] = 'I'; // item
            else if (r < 42) grid[y][x] = 'T'; // tree
            else if (r < 48) grid[y][x] = 'X'; // wall
            else grid[y][x] = 'E'; // empty
        }
    }

    // Boss at center
    int bx = width / 2, by = height / 2;
    grid[by][bx] = 'B';

    // Place NPCs
    npcs.clear();
    npcPositions.clear();
    npcs.emplace_back("Old Hermit", 2, 2, "Slay a Wraith haunting the village.", 50);
    npcPositions.emplace_back(2, 2);
    npcs.emplace_back("Lost Girl", 9, 6, "Bring me the Silver Ring.", 75);
    npcPositions.emplace_back(9, 6);
}

Map::Map() : width(12), height(8) {
    initRandomTerrain();
}

Map::Map(int w, int h) : width(w), height(h) {
    initRandomTerrain();
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }

void Map::display(const Player& player) const {
    clearScreen();
    std::cout << "\n--- Map ---\n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == player.getX() && y == player.getY()) {
                std::cout << YELLOW << 'P' << RESET << ' ';
                continue;
            }

            bool drewNPC = false;
            for (size_t i = 0; i < npcPositions.size(); ++i) {
                if (x == npcPositions[i].first && y == npcPositions[i].second) {
                    std::cout << GREEN << 'N' << RESET << ' ';
                    drewNPC = true;
                    break;
                }
            }
            if (drewNPC) continue;

            char t = grid[y][x];
            switch (t) {
            case 'B': std::cout << RED << 'B' << RESET << ' '; break;
            case 'M': std::cout << MAGENTA << 'M' << RESET << ' '; break;
            case 'I': std::cout << BLUE << 'I' << RESET << ' '; break;
            case 'W': std::cout << BLUE << '~' << RESET << ' '; break;
            case 'T': std::cout << GREEN << '^' << RESET << ' '; break;
            case 'X': std::cout << WHITE << '#' << RESET << ' '; break;
            default:  std::cout << GRAY << '.' << RESET << ' '; break;
            }
        }
        std::cout << "\n";
    }
}

void Map::movePlayer(Player& player, int dx, int dy) {
    int nx = player.getX() + dx;
    int ny = player.getY() + dy;
    if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
        std::cout << "You can't go that way.\n";
        return;
    }

    player.setPosition(nx, ny);
    display(player);

    char tile = grid[ny][nx];

    if (tile == 'I') {
        int drop = std::rand() % 2;
        if (drop == 0) {
            Item potion("Bloody Bandage", "Restores 20 HP.", ItemType::POTION, 20);
            player.addItem(potion);
        }
        else {
            Item adrenaline("Adrenaline Shot", "Boosts strength by 10.", ItemType::ARTIFACT, 10);
            player.addItem(adrenaline);
        }
        grid[ny][nx] = 'E';
    }
    else if (tile == 'M') {
        grid[ny][nx] = 'E';
        int type = std::rand() % 4;
        Enemy e = [&]() {
            switch (type) {
            case 0: return Enemy("Wraith", 30, 5, 2);
            case 1: return Enemy("Faceless One", 40, 7, 3);
            case 2: return Enemy("Crawling Horror", 25, 9, 1);
            default: return Enemy("Whispering Shadow", 35, 6, 4);
            }
            }();
        e.fight(player);
    }
    else {
        encounter(player);
    }

    checkNPCInteraction(player);
}

void Map::encounter(Player& player) {
    int chance = std::rand() % 100;
    if (chance < 40) {
        // Randomized monster like M tile
        int type = std::rand() % 4;
        Enemy e = [&]() {
            switch (type) {
            case 0: return Enemy("Wraith", 30, 5, 2);
            case 1: return Enemy("Faceless One", 40, 7, 3);
            case 2: return Enemy("Crawling Horror", 25, 9, 1);
            default: return Enemy("Whispering Shadow", 35, 6, 4);
            }
            }();
        e.fight(player);
    }
    else if (chance < 60) {
        player.addItem(Item("Bloody Bandage", "Restores 20 HP.", ItemType::POTION, 20));
    }
    else {
        std::cout << "It's eerily quiet...\n";
    }
}

void Map::checkNPCInteraction(Player& player) {
    for (size_t i = 0; i < npcPositions.size(); ++i) {
        if (player.getX() == npcPositions[i].first && player.getY() == npcPositions[i].second) {
            npcs[i].interact(player);
        }
    }
}

void Map::save(const std::string& filename) const {
    std::ofstream out(filename);
    for (auto& r : grid) {
        for (char c : r) out << c;
        out << "\n";
    }
}

void Map::load(const std::string& filename) {
    std::ifstream in(filename);
    grid.clear();
    std::string line;
    while (std::getline(in, line)) {
        grid.emplace_back(line.begin(), line.end());
    }
}
