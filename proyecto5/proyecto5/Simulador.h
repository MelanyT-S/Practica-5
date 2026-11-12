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
    double gravedad;     // g que recibe la particula (0 = sin gravedad)
    std::vector<Particula *> particula;
    std::vector<Obstaculo *> obstaculo;
    GestorDeArchivo archivo;

    // rebote inelástico contra un obstáculo en caso de rebote tru
    bool colisionarObstaculo(Particula *p, Obstaculo *o);
    // validacion en caso de un error
    void validarParticula(const Particula *p) const;
    void validarObstaculo(const Obstaculo *o) const;
public:
    Simulador(double ancho, double alto, double dt, double g = 0.0);
    ~Simulador();
    Simulador(const Simulador &) = delete;
    Simulador &operator=(const Simulador &) = delete;
    // El Simulador se queda con el puntero.
    void agregarParticula(Particula *p);
    void agregarObstaculo(Obstaculo *o);
    bool eliminarParticula(int id);
    bool eliminarObstaculo(int indice);
    int cantidadParticulas() const;
    int cantidadObstaculos() const;
    double getTiempoReal() const;
    void imprimirSistema() const;
    void imprimirGrafico() const;
    void reiniciar();
    void iniciarSimulacion(double tiempoTotal);
    void actualizarEstado();
    void verificarColisiones();
    void registrarDatos();
};
#endif
