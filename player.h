#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "item.h"

enum class PlayerClass { OCCULTIST, HUNTER, SURVIVOR };

class Player {
private:
    std::string name;
    PlayerClass characterClass;
    int maxHealth;
    int currentHealth;
    int maxSanity;
    int currentSanity;
    int baseStrength;  
    int strength;
    int defense;
    int perception;
    int willpower;
    int experience;
    int xpToNextLevel;
    int level;
    int posX, posY;
    int monstersKilled;
    std::vector<Item> inventory;

    void checkLevelUp();

public:
    Player();
    Player(std::string name, PlayerClass classType);

    static void showClassStats();

    std::string getName() const;
    int getCurrentHealth() const;
    int getCurrentSanity() const;
    int getLevel() const;
    int getXP() const;
    int getXPToNext() const;
    std::vector<Item>& getInventory();
    int getMaxHealth() const;

    void takeDamage(int dmg);
    void heal(int amount);
    void showStats() const;
    void showInventory() const;
    void addItem(const Item& item);
    void gainXP(int xp);

    int getMonstersKilled() const;
    void addMonsterKill();

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
    bool isAlive() const;

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    void dropItem(const std::string& itemName);
    void equipItem(const std::string& itemName);
    void addTempStrength(int amount);
    void resetStrength();
};

#endif // PLAYER_H
