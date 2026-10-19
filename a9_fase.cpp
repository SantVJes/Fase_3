#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

// Función para limpiar etiquetas HTML de una cadena
string stripHtmlTags(const string& html) {
    string result;
    bool inTag = false;
    for (char c : html) {
        if (c == '<') {
            inTag = true;
        } else if (c == '>') {
            inTag = false;
            result += ' '; // Espacio para separar palabras al remover tags
        } else if (!inTag) {
            result += c;
        }
    }
    return result;
}

// Función para normalizar texto: minúsculas y conservar solo letras/dígitos alfanuméricos básicos
string cleanAndLowerToken(const string& token) {
    string cleaned = "";
    for (char c : token) {
        if (isalnum(static_cast<unsigned char>(c))) {
            cleaned += tolower(static_cast<unsigned char>(c));
        }
    }
    return cleaned;
}

// Cargar la Stop List en un unordered_set para búsqueda eficiente O(1)
unordered_set<string> loadStopList(const string& filename) {
    unordered_set<string> stopList;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Advertencia: No se pudo abrir el archivo de stop list (" << filename << "). Asegúrate de que esté en formato de texto plano." << endl;
        return stopList;
    }
    string word;
    while (file >> word) {
        stopList.insert(cleanAndLowerToken(word));
    }
    return stopList;
}

int main() {
    // Configuración de rutas
    // Nota: El PDF de la stop list debe guardarse como archivo de texto plano con este nombre.
    string stopListFile = "Actividad9_stoplist.txt"; 
    int totalFiles = 5; 
    string basePath = "files/";

    // Criterio para frecuencias bajas: eliminar palabras con frecuencia menor a este umbral (ej. < 2 elimina las que aparecen solo una vez)
    int minFrequency = 2;

    // 1. Cargar la Stop List
    unordered_set<string> stopList = loadStopList(stopListFile);
    
    // Estructura para almacenar el diccionario global: Palabra -> Frecuencia total
    map<string, int> globalDictionary;

    // Archivo de registro de tiempos requerido (a9_matricula.txt)
    // REEMPLAZA "matricula" por tu número de matrícula real (ej. a9_123456.txt)
    ofstream logFile("a9_matricula.txt");
    if (!logFile.is_open()) {
        cerr << "Error al crear el archivo de registro de tiempos." << endl;
        return 1;
    }

    auto programStart = chrono::high_resolution_clock::now();

    // 2. Procesar cada archivo HTML
    for (int i = 1; i <= totalFiles; ++i) {
        string filePath = basePath + to_string(i) + ".html";
        
        auto fileStart = chrono::high_resolution_clock::now();
        
        ifstream htmlFile(filePath);
        if (!htmlFile.is_open()) {
            // Si el archivo no existe en la ruta de prueba, registramos 0.00 y continuamos
            logFile << filePath << "\t0.00" << endl;
            continue;
        }

        stringstream buffer;
        buffer << htmlFile.rdbuf();
        string rawContent = buffer.str();
        htmlFile.close();

        // Limpiar etiquetas HTML
        string textWithoutTags = stripHtmlTags(rawContent);
        stringstream ss(textWithoutTags);
        string rawToken;

        // Tokenización y Filtrado
        while (ss >> rawToken) {
            string token = cleanAndLowerToken(rawToken);

            // Filtros requeridos:
            // - Longitud mayor a 1 (elimina letras sueltas y números de 1 dígito)
            // - Que no esté en la stop list
            // - Que no esté vacía
            if (token.length() > 1 && stopList.find(token) == stopList.end()) {
                globalDictionary[token]++;
            }
        }

        auto fileEnd = chrono::high_resolution_clock::now();
        chrono::duration<double> fileDuration = fileEnd - fileStart;

        // Registrar tiempo individual formateado con 2 decimales
        logFile << filePath << "\t" << fixed << setprecision(2) << fileDuration.count() << endl;
    }

    auto programEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> totalDuration = programEnd - programStart;

    // Registrar tiempo total de ejecución
    logFile << "\nTiempo total de ejecucion del programa: " << (int)totalDuration.count() << " segundos" << endl;
    logFile.close();

    // 3. Filtrar tokens con frecuencias bajas (menores a minFrequency)
    for (auto it = globalDictionary.begin(); it != globalDictionary.end(); ) {
        if (it->second < minFrequency) {
            it = globalDictionary.erase(it);
        } else {
            ++it;
        }
    }

    // 4. Exportar el diccionario consolidado y filtrado
    ofstream outFile("consolidado_a9.txt");
    for (const auto& pair : globalDictionary) {
        outFile << pair.first << " : " << pair.second << endl;
    }
    outFile.close();

    cout << "¡Procesamiento finalizado con éxito!" << endl;
    cout << "- Archivo de tiempos 'a9_matricula.txt' generado." << endl;
    cout << "- Diccionario consolidado 'consolidado_a9.txt' generado (con filtros de stop list, longitud > 1 y frecuencia >= " << minFrequency << ")." << endl;

    return 0;
}