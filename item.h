#ifndef ITEM_H
#define ITEM_H

#include <string>

enum class ItemType { POTION, WEAPON, KEY, ARTIFACT };
enum class Rarity { COMMON, RARE, EPIC };

struct Item {
    std::string name;
    std::string description;
    ItemType type;
    int effectAmount;   // e.g. HP restored
    int value;          // currency or power
    Rarity rarity;      // Common, Rare, Epic

    Item(std::string n = "", std::string d = "", ItemType t = ItemType::POTION,
        int amt = 0, int val = 0, Rarity r = Rarity::COMMON)
        : name(n), description(d), type(t), effectAmount(amt), value(val), rarity(r) {
    }
};

#endif // ITEM_H
