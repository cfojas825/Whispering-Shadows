#ifndef GAME_H
#define GAME_H

#include <vector>
#include <utility>
#include "player.h"
#include "map.h"
#include "npc.h"

class Game {
private:
    Player player;
    Map world;
    bool isRunning;

    int regionIndex = 0;
    std::vector<std::pair<int, int>> regions = {
        {12, 8},    // Ancient Catacombs
        {15, 12},   // Derelict Asylum
        {10, 6}     // Shadow Labyrinth
    };

    void mainMenu();
    void startNewGame();
    void loadGame();
    void gameLoop();
    void handleEvent();
    void saveGame();

public:
    Game();
    void run();
};

#endif // GAME_H
