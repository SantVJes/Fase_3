#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>

struct HashEntry {
    std::string word;
    int frequency;
    int posting;
    bool isOccupied;

    HashEntry() : word(""), frequency(0), posting(-1), isOccupied(false) {}
};

class HashTable {
private:
    std::vector<HashEntry> table;
    int capacity;
    int size;
    long long collisions;

    unsigned long hashFunction(const std::string& str) const;

public:
    HashTable(int cap = 50021); // Tamaño óptimo basado en número primo grande
    bool insert(const std::string& word, int postingPos);
    int getCollisions() const;
    void exportToASCII(const std::string& filename) const;
    int getCapacity() const { return capacity; }
    int getSize() const { return size; }
};

#endif