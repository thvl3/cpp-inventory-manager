#include "Item.h"

#include <iomanip>
#include <iostream>
#include <sstream>

Item::Item()
    : name_(""), quantity_(0), price_(0.0), category_("Uncategorized") {}

Item::Item(const std::string& name,
           int quantity,
           double price,
           const std::string& category)
    : name_(name),
      quantity_(quantity),
      price_(price),
      category_(category) {}

std::string Item::toFileLine() const {
    std::ostringstream oss;
    oss << name_ << '|'
        << quantity_ << '|'
        << std::fixed << std::setprecision(2) << price_ << '|'
        << category_;
    return oss.str();
}

bool Item::fromFileLine(const std::string& line, Item& out) {
    // Expect 4 pipe-delimited fields. Reject anything else so a corrupted
    // line cannot silently land in the inventory as garbage.
    std::string fields[4];
    int fieldIndex = 0;
    std::string current;

    for (char c : line) {
        if (c == '|') {
            if (fieldIndex >= 3) return false;
            fields[fieldIndex++] = current;
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    if (fieldIndex != 3) return false;
    fields[3] = current;

    try {
        int quantity = std::stoi(fields[1]);
        double price = std::stod(fields[2]);
        if (quantity < 0 || price < 0) return false;
        out = Item(fields[0], quantity, price, fields[3]);
        return true;
    } catch (...) {
        return false;
    }
}

void Item::printRow() const {
    std::cout << std::left
              << std::setw(20) << name_
              << std::setw(10) << quantity_
              << "$" << std::fixed << std::setprecision(2)
              << std::setw(9) << price_
              << std::setw(15) << category_
              << "$" << std::fixed << std::setprecision(2) << totalValue()
              << std::endl;
}
