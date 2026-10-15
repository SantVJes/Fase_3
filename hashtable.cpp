#include "hashtable.h"
#include <fstream>
#include <iostream>

HashTable::HashTable(int cap) : capacity(cap), size(0), collisions(0) {
    table.resize(capacity);
}

unsigned long HashTable::hashFunction(const std::string& str) const {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool HashTable::insert(const std::string& word, int postingPos) {
    unsigned long hashVal = hashFunction(word);
    int idx = hashVal % capacity;
    int i = 0;

    while (table[idx].isOccupied && table[idx].word != word) {
        collisions++;
        i++;
        idx = (hashVal + i) % capacity;
        if (i >= capacity) {
            return false;
        }
    }

    if (!table[idx].isOccupied) {
        table[idx].word = word;
        table[idx].frequency = 1;
        table[idx].posting = postingPos;
        table[idx].isOccupied = true;
        size++;
    } else {
        table[idx].frequency++;
    }
    return true;
}

int HashTable::getCollisions() const {
    return collisions;
}

void HashTable::exportToASCII(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error al abrir el archivo diccionario para exportar.\n";
        return;
    }

    for (int i = 0; i < capacity; ++i) {
        if (table[i].isOccupied) {
            outFile << table[i].word << ";" 
                    << table[i].frequency << ";" 
                    << table[i].posting << "\n";
        }
    }
    outFile.close();
}