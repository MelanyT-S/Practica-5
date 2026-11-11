#ifndef GESTORDEARCHIVO_H
#define GESTORDEARCHIVO_H

#include <string>
#include <fstream>
#include <vector>
#include "Particula.h"

class GestorDeArchivo {
private:
    std::string nombre;     // Nombre o ruta del archivo de texto de salida
    std::ofstream archivo;  // Flujo de salida para escribir datos en el disco duro

public:
    // Constructor que recibe el nombre del archivo a crear
    GestorDeArchivo(std::string nombreArchivo);

    // Abre el archivo en modo escritura y valida si esta disponible
    bool abrir();

    // Guarda las posiciones y velocidades de todas las particulas activas en un instante de tiempo
    void guardarpaso(double tiempo, const std::vector<Particula*>& particulas);

    // Registra un evento de colision (con paredes, obstaculos o particulas) y su tiempo exacto
    void guardarcolicion(double tiempo, std::string tipo, int idA, int idB);

    // Cierra el flujo del archivo para liberar recursos del sistema de manera segura
    void cerrar();
};

#endif // GESTORDEARCHIVO_H