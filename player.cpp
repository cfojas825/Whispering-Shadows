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
      baseStrength(10), strength(10), // Updated
      defense(10), perception(10), willpower(10),
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
        maxHealth = 60; baseStrength = strength = 8; defense = 8; perception = 15; willpower = 20; break;
    case PlayerClass::HUNTER:
        maxHealth = 100; baseStrength = strength = 15; defense = 12; perception = 10; willpower = 10; break;
    default: // SURVIVOR
        maxHealth = 80; baseStrength = strength = 10; defense = 10; perception = 12; willpower = 15;
    }
    currentHealth = maxHealth;
    currentSanity = maxSanity = 100;
}

void Player::resetStrength() {
    strength = baseStrength;
}
