#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "player.h"
#include "npc.h"

class Map {
private:
    int width, height;
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<int, int>> npcPositions;
    std::vector<NPC> npcs;

    void initRandomTerrain();

public:
    Map();                // default 12×8 map
    Map(int w, int h);    // custom size map

    int getWidth() const;
    int getHeight() const;

    void display(const Player& player) const;
    void movePlayer(Player& player, int dx, int dy);
    void encounter(Player& player);
    void checkNPCInteraction(Player& player);

    void save(const std::string& filename) const;
    void load(const std::string& filename);
};

#endif // MAP_H
