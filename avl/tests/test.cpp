#include "trees/avl.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

//función para leer claves desde un archivo binario
int* readKeysFromFile(const std::string& filename, int* n_keys) {
    std::ifstream fin(filename, std::ios::binary | std::ios::ate);
    if (!fin.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        *n_keys = 0;
        return nullptr;
    }

    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    if (size % 4 != 0) {
        std::cerr << "El archivo tiene un tamaño incorrecto para enteros de 4 bytes." << std::endl;
        *n_keys = 0;
        return nullptr;
    }

    *n_keys = size / 4;
    int* keys = new int[*n_keys];
    fin.read(reinterpret_cast<char*>(keys), size);
    fin.close();
    
    std::cout << "Archivo " << filename << " leído correctamente con " << *n_keys << " claves." << std::endl;
    
    return keys;
}

//función para leer queries desde un archivo binario
std::vector<int> readQueriesFromFile(const std::string& filename) {
    std::ifstream fin(filename, std::ios::binary);
    std::vector<int> queries;
    char val[4];
    while (fin.read(val, 4)) {
        queries.push_back(*reinterpret_cast<int*>(val));
    }
    fin.close();
    std::cout << "Archivo de queries leído correctamente con " << queries.size() << " queries." << std::endl;
    return queries;
}

int main(int nargs, char** vargs) {
    //ruta base de la carpeta de datos
    std::string data_folder = "/home/edauandes/t3EDA/data_trees/";

    // Lista de archivos de claves
    std::vector<std::string> files = {
        data_folder + "keys_1024.bin",
        data_folder + "keys_32768.bin",
        data_folder + "keys_1048576.bin",
        data_folder + "keys_33554432.bin",
        data_folder + "keys_sorted_1024.bin",
        data_folder + "keys_sorted_32768.bin",
        data_folder + "keys_sorted_1048576.bin",
        data_folder + "keys_sorted_33554432.bin"
    };
    
    //cargar queries
    std::vector<int> queries = readQueriesFromFile(data_folder + "queries_1000.bin");

    //procesar cada archivo en la lista
    for (const std::string& filename : files) {
        std::cout << "Procesando archivo: " << filename << std::endl;
        
        int n_data = 0;
        int* data = readKeysFromFile(filename, &n_data);
        if (data == nullptr) {
            std::cerr << "Error al leer los datos del archivo: " << filename << std::endl;
            continue;
        }

        trees::AVL avl;

        //medir el tiempo de inserción en el árbol AVL
        auto start_insertion = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < n_data; i++) {
            avl.insert(data[i]);
        }
        auto end_insertion = std::chrono::high_resolution_clock::now();
        auto duration_insertion = std::chrono::duration_cast<std::chrono::microseconds>(end_insertion - start_insertion).count();
        std::cout << "Tiempo de inserción para " << filename << ": " << duration_insertion << " microsegundos" << std::endl;

        //medir el tiempo de búsqueda
        int found_count = 0;
        int not_found_count = 0;
        auto start_search = std::chrono::high_resolution_clock::now();
        for (int query : queries) {
            if (avl.find(query) != nullptr) {  
                found_count++;
            } else {
                not_found_count++;
            }
        }
        auto end_search = std::chrono::high_resolution_clock::now();
        auto duration_search = std::chrono::duration_cast<std::chrono::microseconds>(end_search - start_search).count();

        //imprimir los resultados de búsqueda
        std::cout << "Tiempo de búsqueda para " << filename << ": " << duration_search << " microsegundos" << std::endl;
        std::cout << "Resultados de búsqueda - Encontrados: " << found_count << ", No encontrados: " << not_found_count << std::endl;

        delete[] data;
    }

    return 0;
}
