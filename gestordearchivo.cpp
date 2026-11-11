#include "GestorDeArchivo.h"
#include <iostream>

GestorDeArchivo::GestorDeArchivo(std::string nombreArchivo)
    : nombre(nombreArchivo) {}

bool GestorDeArchivo::abrir() {
    // Abre el archivo usando c_str() para compatibilidad total con C++
    archivo.open(nombre.c_str());
    // Retorna true si el archivo se abrio con exito, false si hubo un fallo
    return archivo.is_open();
}

void GestorDeArchivo::guardarpaso(double tiempo, const std::vector<Particula*>& particulas) {
    if (archivo.is_open()) {
        // Formato limpio por columnas: tiempo, id, x, y, vx, vy
        // Esto es ideal para que luego Python (con matplotlib) lea el archivo rapidamente y grafique las trayectorias
        for (size_t i = 0; i < particulas.size(); ++i) {
            if (particulas[i]->get_activa()) {
                archivo << tiempo << " "
                        << particulas[i]->get_id() << " "
                        << particulas[i]->get_ejex() << " "
                        << particulas[i]->get_ejey() << " "
                        << particulas[i]->getVelocidadx() << " "
                        << particulas[i]->getVelocidady() << "\n";
            }
        }
    }
}

void GestorDeArchivo::guardarcolicion(double tiempo, std::string tipo, int idA, int idB) {
    if (archivo.is_open()) {
        // Registra los eventos criticos dentro de la simulacion con su respectivo sello de tiempo
        archivo << "[COLISION] T=" << tiempo
                << " | Tipo: " << tipo
                << " | ID_A: " << idA
                << " | ID_B: " << idB << "\n";
    }
}

void GestorDeArchivo::cerrar() {
    if (archivo.is_open()) {
        archivo.close();
    }
}