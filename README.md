# Overview

**CSE 310 — Module 1 (Language: C++)**

This project is a command-line inventory manager written in C++. It lets the user add, view, search, update, and remove inventory items, summarize stock by category, and save the inventory to a plain-text file so it can be reloaded the next time the program runs.

My purpose for writing this software was to deepen my understanding of core C++ syntax and program structure: header/implementation files, classes with constructors and accessors, the STL containers `std::vector` and `std::map`, file streams (`ifstream` / `ofstream`), and defensive input validation in a long-running menu loop.

Demo: https://youtu.be/1pDZu7Ln_R8



# Development Environment

- macOS (Darwin)
- Visual Studio Code
- Apple Clang / `g++` (C++17)
- GNU Make
- Git + GitHub CLI

**Language:** C++17
**Libraries:** Standard Template Library only — `<iostream>`, `<fstream>`, `<sstream>`, `<iomanip>`, `<string>`, `<vector>`, `<map>`, `<limits>`, `<cctype>`. No third-party dependencies.

# Build & Run

```sh
make          # compile
make run      # compile and launch
make clean    # remove build artifacts
```

On startup the program tries to load `inventory.txt` from the working directory. A sample `inventory.txt` is included in the repo so you can see the program with data on the first run. On exit it asks whether to save back to the same file.

# Useful Websites

- [cppreference.com](https://en.cppreference.com/w/) — authoritative reference for STL containers and `<iomanip>` formatting.
- [LearnCpp.com](https://www.learncpp.com/) — refresher on header organization, constructors, and references.
- [GNU Make Manual](https://www.gnu.org/software/make/manual/make.html) — multi-file build rules.
- [Stack Overflow: robust integer parsing in C++](https://stackoverflow.com/questions/194465/) — pattern for `std::stoi` with full-string consumption check.

# Future Work

- Replace the linear `findIndex` lookup with an index by lowercase name for O(1) lookups on large inventories.
- Persist data as JSON instead of a pipe-delimited text file so the format extends cleanly when new fields are added.
- Add a `low-stock` report that flags items below a configurable reorder threshold.
- Add unit tests for `Item::fromFileLine` and `InventoryManager` using a lightweight framework such as doctest.
