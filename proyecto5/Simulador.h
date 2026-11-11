#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include "Particula.h"
#include "Obstaculo.h"
#include "GestorDeArchivo.h"

class Simulador {
private:
    double ancho;
    double alto;
    double tiempo;       // paso de tiempo (dt)
    double tiemporeal;   // tiempo transcurrido de la simulación
    double gravedad;     // g que recibe Particula::mover (0 = sin gravedad)
    std::vector<Particula *> particula;
    std::vector<Obstaculo *> obstaculo;
    GestorDeArchivo archivo;

    // Rebote inelástico contra un obstáculo. Devuelve true si hubo rebote.
    bool colisionarObstaculo(Particula *p, Obstaculo *o);

public:
    Simulador(double ancho, double alto, double dt, double g = 0.0);
    ~Simulador();
    Simulador(const Simulador &) = delete;
    Simulador &operator=(const Simulador &) = delete;

    void agregarParticula(Particula *p);
    void agregarObstaculo(Obstaculo *o);
    void iniciarSimulacion(double tiempoTotal);   // bucle principal
    void actualizarEstado();                      // mueve las partículas
    void verificarColisiones();                   // paredes, obstáculos, partículas
    void registrarDatos();                        // escribe en el archivo
};

#endif
