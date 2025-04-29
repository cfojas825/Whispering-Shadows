#include "player.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Base XP to level 2
static constexpr int BASE_XP = 100;

// ANSI colors for inventory
static constexpr const char* WHITE = "\033[37m";
static constexpr const char* BLUE = "\033[34m";
static constexpr const char* MAGENTA = "\033[35m";
static constexpr const char* RESET = "\033[0m";

Player::Player()
    : name("Unknown"), characterClass(PlayerClass::SURVIVOR),
    experience(0), xpToNextLevel(BASE_XP), level(1),
    maxHealth(80), currentHealth(80),
    maxSanity(100), currentSanity(100),
    strength(10), defense(10), perception(10), willpower(10),
    posX(0), posY(0), monstersKilled(0)
{
}

Player::Player(std::string n, PlayerClass c)
    : name(n), characterClass(c),
    experience(0), xpToNextLevel(BASE_XP), level(1),
    posX(0), posY(0), monstersKilled(0)
{
    switch (c) {
    case PlayerClass::OCCULTIST:
        maxHealth = 60; strength = 8; defense = 8; perception = 15; willpower = 20; break;
    case PlayerClass::HUNTER:
        maxHealth = 100; strength = 15; defense = 12; perception = 10; willpower = 10; break;
    default: // SURVIVOR
        maxHealth = 80; strength = 10; defense = 10; perception = 12; willpower = 15;
    }
    currentHealth = maxHealth;
    currentSanity = maxSanity = 100;
}

void Player::checkLevelUp() {
    while (experience >= xpToNextLevel) {
        experience -= xpToNextLevel;
        level++;
        xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5f);
        maxHealth += 5;
        strength += 1;
        defense += 1;
        currentHealth = maxHealth;
        std::cout << "\n*** LEVEL UP! Level " << level << " ***\n";
    }
}

void Player::gainXP(int xp) {
    experience += xp;
    std::cout << "Gained " << xp << " XP (" << experience << "/" << xpToNextLevel << ")\n";
    checkLevelUp();
}

int Player::getLevel() const { return level; }
int Player::getXP() const { return experience; }
int Player::getXPToNext() const { return xpToNextLevel; }
std::string Player::getName() const { return name; }
int Player::getCurrentHealth() const { return currentHealth; }
int Player::getCurrentSanity() const { return currentSanity; }
std::vector<Item>& Player::getInventory() { return inventory; }

void Player::showClassStats() {
    std::cout << "[1] OCCULTIST: HP60 STR8 DEF8 PER15 WIL20\n"
        << "[2] HUNTER:    HP100 STR15 DEF12 PER10 WIL10\n"
        << "[3] SURVIVOR:  HP80 STR10 DEF10 PER12 WIL15\n";
}

void Player::showStats() const {
    std::cout << name << " (Lvl " << level << ") XP:" << experience << "/" << xpToNextLevel << "\n"
        << "HP:" << currentHealth << "/" << maxHealth
        << "  San:" << currentSanity << "/" << maxSanity << "\n"
        << "STR:" << strength << " DEF:" << defense
        << " PER:" << perception << " WIL:" << willpower << "\n";
}

void Player::showInventory() const {
    if (inventory.empty()) {
        std::cout << "Inventory empty.\n";
        return;
    }
    for (size_t i = 0; i < inventory.size(); ++i) {
        const auto& it = inventory[i];
        const char* col = (it.rarity == Rarity::COMMON ? WHITE :
            it.rarity == Rarity::RARE ? BLUE : MAGENTA);
        std::cout << i + 1 << ". " << col << it.name << RESET
            << " (" << it.description << ") [" <<
            (it.rarity == Rarity::COMMON ? "Common" :
                it.rarity == Rarity::RARE ? "Rare" : "Epic")
            << "]\n";
    }
}

void Player::takeDamage(int dmg) {
    currentHealth = std::max(0, currentHealth - dmg);
    std::cout << "You took " << dmg << " damage.\n";
}

void Player::heal(int amount) {
    currentHealth = std::min(maxHealth, currentHealth + amount);
    std::cout << "You healed " << amount << " HP.\n";
}

bool Player::isAlive() const { return currentHealth > 0; }

int Player::getX() const { return posX; }
int Player::getY() const { return posY; }
void Player::setPosition(int x, int y) { posX = x; posY = y; }

int Player::getMonstersKilled() const { return monstersKilled; }
void Player::addMonsterKill() { monstersKilled++; }

void Player::addItem(const Item& item) {
    inventory.push_back(item);
    std::cout << "Got " << item.name << "!\n";
}

void Player::save(const std::string& fn) const {
    std::ofstream out(fn);
    out << name << "\n" << static_cast<int>(characterClass) << "\n"
        << maxHealth << " " << currentHealth << "\n"
        << experience << " " << xpToNextLevel << " " << level << "\n"
        << posX << " " << posY << "\n";
}

void Player::load(const std::string& fn) {
    std::ifstream in(fn);
    int ci;
    in >> name >> ci
        >> maxHealth >> currentHealth
        >> experience >> xpToNextLevel >> level
        >> posX >> posY;
    characterClass = static_cast<PlayerClass>(ci);
}

void Player::dropItem(const std::string& n) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->name == n) {
            inventory.erase(it);
            std::cout << n << " dropped.\n";
            return;
        }
    }
    std::cout << n << " not found.\n";
}

void Player::equipItem(const std::string& n) {
    std::cout << n << " equipped (placeholder).\n";
}

// 🔥 NEW Getter function!
int Player::getMaxHealth() const {
    return maxHealth;
}
void Player::addTempStrength(int amount) {
    strength += amount;
    std::cout << "You feel a surge of power! (+" << amount << " Strength)\n";
}
