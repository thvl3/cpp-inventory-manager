// Command-line inventory manager. Demonstrates C++ basics required for
// CSE 310 Module 1: variables, expressions, conditionals, loops, functions,
// a class with constructors, STL containers (vector and map), and file I/O
// (the stretch challenge).
//
// Build:  make
// Run:    ./inventory
//
// On startup the program tries to load inventory.txt from the working
// directory. On exit it offers to save back to the same file.

#include <iostream>
#include <limits>
#include <string>

#include "InventoryManager.h"
#include "Item.h"

namespace {

const std::string kDefaultDataFile = "inventory.txt";

// Reads a full line from std::cin, including spaces. Returns false if the
// stream is closed (Ctrl-D / EOF) so the caller can break out of the loop
// instead of spinning on a dead stream.
bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    if (!std::getline(std::cin, out)) {
        return false;
    }
    return true;
}

// Reads an integer in the range [minValue, maxValue]. Re-prompts on bad
// input rather than throwing, which keeps the menu loop friendly.
bool readInt(const std::string& prompt, int& out, int minValue, int maxValue) {
    while (true) {
        std::string line;
        if (!readLine(prompt, line)) return false;
        try {
            std::size_t consumed = 0;
            int value = std::stoi(line, &consumed);
            if (consumed != line.size()) {
                std::cout << "  Please enter a whole number." << std::endl;
                continue;
            }
            if (value < minValue || value > maxValue) {
                std::cout << "  Please enter a value between "
                          << minValue << " and " << maxValue << "." << std::endl;
                continue;
            }
            out = value;
            return true;
        } catch (...) {
            std::cout << "  Please enter a whole number." << std::endl;
        }
    }
}

bool readDouble(const std::string& prompt, double& out, double minValue) {
    while (true) {
        std::string line;
        if (!readLine(prompt, line)) return false;
        try {
            std::size_t consumed = 0;
            double value = std::stod(line, &consumed);
            if (consumed != line.size()) {
                std::cout << "  Please enter a number." << std::endl;
                continue;
            }
            if (value < minValue) {
                std::cout << "  Value must be >= " << minValue << "." << std::endl;
                continue;
            }
            out = value;
            return true;
        } catch (...) {
            std::cout << "  Please enter a number." << std::endl;
        }
    }
}

void printMenu() {
    std::cout << "\n========== Inventory Manager ==========" << std::endl;
    std::cout << "1. Add item" << std::endl;
    std::cout << "2. List all items" << std::endl;
    std::cout << "3. Search by name" << std::endl;
    std::cout << "4. Update quantity" << std::endl;
    std::cout << "5. Update price" << std::endl;
    std::cout << "6. Remove item" << std::endl;
    std::cout << "7. Category summary" << std::endl;
    std::cout << "8. Save to file" << std::endl;
    std::cout << "9. Load from file" << std::endl;
    std::cout << "0. Quit" << std::endl;
    std::cout << "========================================" << std::endl;
}

void doAdd(InventoryManager& inv) {
    std::string name;
    if (!readLine("Item name: ", name) || name.empty()) {
        std::cout << "  Cancelled." << std::endl;
        return;
    }
    int quantity = 0;
    if (!readInt("Quantity: ", quantity, 0, std::numeric_limits<int>::max())) return;
    double price = 0.0;
    if (!readDouble("Price: ", price, 0.0)) return;
    std::string category;
    if (!readLine("Category: ", category)) return;
    if (category.empty()) category = "Uncategorized";

    if (inv.addItem(Item(name, quantity, price, category))) {
        std::cout << "  Added \"" << name << "\"." << std::endl;
    } else {
        std::cout << "  Could not add item (duplicate name?)." << std::endl;
    }
}

void doSearch(const InventoryManager& inv) {
    std::string query;
    if (!readLine("Search query: ", query) || query.empty()) {
        std::cout << "  Cancelled." << std::endl;
        return;
    }
    std::vector<Item> matches = inv.searchByName(query);
    if (matches.empty()) {
        std::cout << "  No matches." << std::endl;
        return;
    }
    std::cout << "  " << matches.size() << " match(es):" << std::endl;
    for (const Item& item : matches) {
        item.printRow();
    }
}

void doUpdateQuantity(InventoryManager& inv) {
    std::string name;
    if (!readLine("Item name: ", name) || name.empty()) return;
    int newQuantity = 0;
    if (!readInt("New quantity: ", newQuantity, 0, std::numeric_limits<int>::max())) return;
    if (inv.updateQuantity(name, newQuantity)) {
        std::cout << "  Updated." << std::endl;
    } else {
        std::cout << "  Item not found." << std::endl;
    }
}

void doUpdatePrice(InventoryManager& inv) {
    std::string name;
    if (!readLine("Item name: ", name) || name.empty()) return;
    double newPrice = 0.0;
    if (!readDouble("New price: ", newPrice, 0.0)) return;
    if (inv.updatePrice(name, newPrice)) {
        std::cout << "  Updated." << std::endl;
    } else {
        std::cout << "  Item not found." << std::endl;
    }
}

void doRemove(InventoryManager& inv) {
    std::string name;
    if (!readLine("Item name to remove: ", name) || name.empty()) return;
    if (inv.removeItem(name)) {
        std::cout << "  Removed." << std::endl;
    } else {
        std::cout << "  Item not found." << std::endl;
    }
}

void doSave(const InventoryManager& inv) {
    std::string path;
    if (!readLine("Save to file [" + kDefaultDataFile + "]: ", path)) return;
    if (path.empty()) path = kDefaultDataFile;
    if (inv.saveToFile(path)) {
        std::cout << "  Saved " << inv.size() << " item(s) to " << path << "." << std::endl;
    } else {
        std::cout << "  Could not write to " << path << "." << std::endl;
    }
}

void doLoad(InventoryManager& inv) {
    std::string path;
    if (!readLine("Load from file [" + kDefaultDataFile + "]: ", path)) return;
    if (path.empty()) path = kDefaultDataFile;
    if (inv.loadFromFile(path)) {
        std::cout << "  Loaded " << inv.size() << " item(s) from " << path << "." << std::endl;
    } else {
        std::cout << "  Could not open " << path << "." << std::endl;
    }
}

}  // namespace

int main() {
    InventoryManager inv;

    // Best-effort load on startup. A missing file is fine for first run.
    if (inv.loadFromFile(kDefaultDataFile)) {
        std::cout << "Loaded " << inv.size() << " item(s) from "
                  << kDefaultDataFile << "." << std::endl;
    } else {
        std::cout << "No existing " << kDefaultDataFile
                  << " found. Starting with empty inventory." << std::endl;
    }

    while (true) {
        printMenu();
        int choice = 0;
        if (!readInt("Choose: ", choice, 0, 9)) break;

        switch (choice) {
            case 1: doAdd(inv); break;
            case 2: inv.listAll(); break;
            case 3: doSearch(inv); break;
            case 4: doUpdateQuantity(inv); break;
            case 5: doUpdatePrice(inv); break;
            case 6: doRemove(inv); break;
            case 7: inv.printCategorySummary(); break;
            case 8: doSave(inv); break;
            case 9: doLoad(inv); break;
            case 0:
                std::cout << "Save before quitting? (y/N): ";
                {
                    std::string answer;
                    if (std::getline(std::cin, answer) &&
                        !answer.empty() &&
                        (answer[0] == 'y' || answer[0] == 'Y')) {
                        if (inv.saveToFile(kDefaultDataFile)) {
                            std::cout << "Saved to " << kDefaultDataFile
                                      << "." << std::endl;
                        } else {
                            std::cout << "Save failed." << std::endl;
                        }
                    }
                }
                std::cout << "Goodbye." << std::endl;
                return 0;
            default:
                std::cout << "  Invalid choice." << std::endl;
                break;
        }
    }
    return 0;
}
