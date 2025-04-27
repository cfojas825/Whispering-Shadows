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

    // Fill grid: W=water, M=monster spawn, I=item, T=tree, X=wall, E=empty, B=boss
    void initRandomTerrain();

public:
    Map();                // default 12×8 map
    Map(int w, int h);    // custom dimensions

    int getWidth()  const;
    int getHeight() const;

    void display(const Player& player) const;
    void movePlayer(Player& player, int dx, int dy);
    void encounter(Player& player);
    void checkNPCInteraction(Player& player);

    void save(const std::string& filename) const;
    void load(const std::string& filename);
};

#endif // MAP_H
