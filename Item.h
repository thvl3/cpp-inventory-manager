#ifndef ITEM_H
#define ITEM_H

#include <string>

// Represents a single inventory record. Kept small and copyable so it
// can live directly inside a std::vector without heap indirection.
class Item {
public:
    Item();
    Item(const std::string& name,
         int quantity,
         double price,
         const std::string& category);

    const std::string& name() const { return name_; }
    int quantity() const { return quantity_; }
    double price() const { return price_; }
    const std::string& category() const { return category_; }

    void setQuantity(int quantity) { quantity_ = quantity; }
    void setPrice(double price) { price_ = price; }
    void setCategory(const std::string& category) { category_ = category; }

    double totalValue() const { return price_ * quantity_; }

    // Pipe-delimited so names with commas survive a save/load round trip.
    std::string toFileLine() const;
    static bool fromFileLine(const std::string& line, Item& out);

    void printRow() const;

private:
    std::string name_;
    int quantity_;
    double price_;
    std::string category_;
};

#endif
