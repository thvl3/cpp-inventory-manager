#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <string>
#include <vector>

#include "Item.h"

// Owns the inventory and exposes the operations the menu drives.
// Kept separate from main.cpp so the storage and I/O logic can be
// unit-tested or reused without dragging in the console UI.
class InventoryManager {
public:
    bool addItem(const Item& item);
    bool removeItem(const std::string& name);
    bool updateQuantity(const std::string& name, int newQuantity);
    bool updatePrice(const std::string& name, double newPrice);

    // Linear search by name (case-insensitive). Returns -1 if missing.
    int findIndex(const std::string& name) const;

    void listAll() const;
    std::vector<Item> searchByName(const std::string& query) const;

    void printCategorySummary() const;
    double totalInventoryValue() const;
    int totalItemCount() const;
    std::size_t size() const { return items_.size(); }

    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);

private:
    std::vector<Item> items_;

    static std::string toLower(const std::string& s);
};

#endif
