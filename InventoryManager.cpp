#include "InventoryManager.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

std::string InventoryManager::toLower(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return out;
}

int InventoryManager::findIndex(const std::string& name) const {
    const std::string target = toLower(name);
    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (toLower(items_[i].name()) == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool InventoryManager::addItem(const Item& item) {
    if (item.name().empty()) return false;
    if (findIndex(item.name()) != -1) return false;
    items_.push_back(item);
    return true;
}

bool InventoryManager::removeItem(const std::string& name) {
    int idx = findIndex(name);
    if (idx < 0) return false;
    items_.erase(items_.begin() + idx);
    return true;
}

bool InventoryManager::updateQuantity(const std::string& name, int newQuantity) {
    if (newQuantity < 0) return false;
    int idx = findIndex(name);
    if (idx < 0) return false;
    items_[idx].setQuantity(newQuantity);
    return true;
}

bool InventoryManager::updatePrice(const std::string& name, double newPrice) {
    if (newPrice < 0) return false;
    int idx = findIndex(name);
    if (idx < 0) return false;
    items_[idx].setPrice(newPrice);
    return true;
}

static void printHeader() {
    std::cout << std::left
              << std::setw(20) << "Name"
              << std::setw(10) << "Qty"
              << std::setw(10) << "Price"
              << std::setw(15) << "Category"
              << "Total"
              << std::endl;
    std::cout << std::string(70, '-') << std::endl;
}

void InventoryManager::listAll() const {
    if (items_.empty()) {
        std::cout << "Inventory is empty." << std::endl;
        return;
    }
    printHeader();
    for (const Item& item : items_) {
        item.printRow();
    }
    std::cout << std::string(70, '-') << std::endl;
    std::cout << "Items: " << totalItemCount()
              << "   Total value: $"
              << std::fixed << std::setprecision(2) << totalInventoryValue()
              << std::endl;
}

std::vector<Item> InventoryManager::searchByName(const std::string& query) const {
    std::vector<Item> matches;
    const std::string needle = toLower(query);
    if (needle.empty()) return matches;
    for (const Item& item : items_) {
        if (toLower(item.name()).find(needle) != std::string::npos) {
            matches.push_back(item);
        }
    }
    return matches;
}

void InventoryManager::printCategorySummary() const {
    if (items_.empty()) {
        std::cout << "Inventory is empty." << std::endl;
        return;
    }
    // map keeps categories in alphabetical order, which is exactly what
    // we want for a deterministic summary printout.
    std::map<std::string, int> countByCategory;
    std::map<std::string, double> valueByCategory;

    for (const Item& item : items_) {
        countByCategory[item.category()] += item.quantity();
        valueByCategory[item.category()] += item.totalValue();
    }

    std::cout << std::left
              << std::setw(20) << "Category"
              << std::setw(10) << "Units"
              << "Value" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& entry : countByCategory) {
        std::cout << std::left
                  << std::setw(20) << entry.first
                  << std::setw(10) << entry.second
                  << "$" << std::fixed << std::setprecision(2)
                  << valueByCategory[entry.first]
                  << std::endl;
    }
}

double InventoryManager::totalInventoryValue() const {
    double total = 0.0;
    for (const Item& item : items_) {
        total += item.totalValue();
    }
    return total;
}

int InventoryManager::totalItemCount() const {
    int total = 0;
    for (const Item& item : items_) {
        total += item.quantity();
    }
    return total;
}

bool InventoryManager::saveToFile(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) return false;
    for (const Item& item : items_) {
        out << item.toFileLine() << '\n';
    }
    return out.good();
}

bool InventoryManager::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return false;

    std::vector<Item> loaded;
    std::string line;
    int lineNumber = 0;
    int skipped = 0;
    while (std::getline(in, line)) {
        ++lineNumber;
        if (line.empty()) continue;
        Item parsed;
        if (Item::fromFileLine(line, parsed)) {
            loaded.push_back(parsed);
        } else {
            ++skipped;
            std::cerr << "  warning: skipped malformed line " << lineNumber
                      << ": " << line << std::endl;
        }
    }

    items_ = std::move(loaded);
    if (skipped > 0) {
        std::cerr << "  loaded with " << skipped << " skipped line(s)." << std::endl;
    }
    return true;
}
