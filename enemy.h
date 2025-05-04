#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "player.h"

class Enemy {
private:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Enemy(const std::string& name, int hp, int atk, int def);
    void fight(Player& player);
};

#endif // ENEMY_H
