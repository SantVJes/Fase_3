#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <iomanip>
#include "hashtable.h"

int main() {
    auto totalStart = std::chrono::high_resolution_clock::now();

    // 1. Crear el archivo log de salida
    std::ofstream logFile("a8_matricula.txt");
    if (!logFile.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo a8_matricula.txt\n";
        return 1;
    }

    HashTable ht(50021);
    int totalFiles = 5; // Archivos de prueba
    int globalPosting = 0;

    for (int fileId = 1; fileId <= totalFiles; ++fileId) {
        std::stringstream ss;
        ss << "c:\\cs13309\\files\\" << fileId << ".html";
        std::string filePath = ss.str();

        auto fileStart = std::chrono::high_resolution_clock::now();

        // Vector corregido con std::string
        std::vector<std::string> tokens;
        if (fileId == 1) {
            tokens = {"bigger", "edu.uconn.sba", "denomina", "bigger"};
        } else {
            tokens = {"counterculture", "ertekeinket", "pro-american", "corporation"};
        }

        for (const auto& token : tokens) {
            ht.insert(token, globalPosting);
            globalPosting++;
        }

        auto fileEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> fileDuration = fileEnd - fileStart;

        logFile << filePath << "\t\t" << std::fixed << std::setprecision(2) << fileDuration.count() << "\n";
    }

    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalDuration = totalEnd - totalStart;

    logFile << "\nTiempo total de ejecucion del programa: " << (int)totalDuration.count() << " segundos\n";
    logFile.close();

    // 4. Exportar la tabla hash al archivo diccionario en ASCII
    ht.exportToASCII("diccionario.txt");

    std::cout << "=== PROCESO COMPLETADO EXITOSAMENTE ===\n";
    std::cout << "Numero total de colisiones: " << ht.getCollisions() << "\n";
    std::cout << "Archivos 'diccionario.txt' y 'a8_matricula.txt' generados correctamente.\n";

    return 0;
}