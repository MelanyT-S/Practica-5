#include "Simulador.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
//constructor
Simulador::Simulador(double ancho, double alto, double dt, double g)
    : ancho(ancho), alto(alto), tiempo(dt), tiemporeal(0), gravedad(g), archivo("salida.txt") {
    if (dt <= 0) throw std::invalid_argument("dt debe ser positivo");
    if (ancho <= 0 || alto <= 0) throw std::invalid_argument("dimensiones invalidas");
    if (!archivo.abrir()) throw std::runtime_error("No se pudo abrir el archivo de salida");
}
//destructor
Simulador::~Simulador() {
    for (auto *p : particula) delete p;
    for (auto *o : obstaculo) delete o;
    archivo.cerrar();
}

void Simulador::agregarParticula(Particula *p) {
    if (!p) throw std::invalid_argument("particula nula");
    particula.push_back(p);
}

void Simulador::agregarObstaculo(Obstaculo *o) {
    if (!o) throw std::invalid_argument("obstaculo nulo");
    obstaculo.push_back(o);
}
//bucle principal
void Simulador::iniciarSimulacion(double tiempoTotal) {
    registrarDatos();
    while (tiemporeal < tiempoTotal) {
        actualizarEstado();
        verificarColisiones();
        tiemporeal += tiempo;
        registrarDatos();
    }
}
void Simulador::actualizarEstado() {
    for (auto *p : particula)
        if (p->get_activa()) p->mover(tiempo, gravedad);
}
bool Simulador::colisionarObstaculo(Particula *p, Obstaculo *o) {
    double ox = o->get_ejex(), oy = o->get_ejey(), l = o->get_Lado();
    double px = p->get_ejex(), py = p->get_ejey(), r = p->getRadio();
    double cx = std::min(std::max(px, ox), ox + l);
    double cy = std::min(std::max(py, oy), oy + l);
    double dx = px - cx, dy = py - cy;
    if (dx * dx + dy * dy >= r * r) return false;
    double nx = 0, ny = 0;
    if (!o->contienePunto(px, py)) {
        if (std::fabs(dx) >= std::fabs(dy)) nx = (dx < 0) ? -1 : 1;
        else                                ny = (dy < 0) ? -1 : 1;
    } else {
        double izq = px - ox, der = ox + l - px;
        double aba = py - oy, arr = oy + l - py;
        double m = std::min(std::min(izq, der), std::min(aba, arr));
        if (m == izq) nx = -1; else if (m == der) nx = 1;
        else if (m == aba) ny = -1; else ny = 1;
    }
    if (nx != 0) p->set_ejex((nx > 0 ? ox + l : ox) + nx * r);
    if (ny != 0) p->set_ejey((ny > 0 ? oy + l : oy) + ny * r);
    double vn = p->getVelocidadx() * nx + p->getVelocidady() * ny;
    if (vn >= 0) return false;
    double e = o->get_elastico();
    p->setVelocidadx(p->getVelocidadx() - (1.0 + e) * vn * nx);
    p->setVelocidady(p->getVelocidady() - (1.0 + e) * vn * ny);
    return true;
}
void Simulador::verificarColisiones() {
    //paredes(elastica) obstaculo  (inelastica)
    for (auto *p : particula) {
        if (!p->get_activa()) continue;
        double vx = p->getVelocidadx(), vy = p->getVelocidady();
        p->rebotarPared(ancho, alto);
        if (p->getVelocidadx() != vx || p->getVelocidady() != vy)
            archivo.guardarColision(tiemporeal, "PARED", p->get_id(), -1);
        for (size_t k = 0; k < obstaculo.size(); k++)
            if (colisionarObstaculo(p, obstaculo[k]))
                archivo.guardarColision(tiemporeal, "OBSTACULO", p->get_id(), (int)k);
    }
    //completamente inelastica
    for (size_t i = 0; i < particula.size(); i++) {
        if (!particula[i]->get_activa()) continue;
        for (size_t j = i + 1; j < particula.size(); j++) {
            if (!particula[j]->get_activa() || !particula[i]->get_activa()) continue;
            Particula *a = particula[i];
            Particula *b = particula[j];
            double dx = b->get_ejex() - a->get_ejex();
            double dy = b->get_ejey() - a->get_ejey();
            double rr = a->getRadio() + b->getRadio();
            if (dx * dx + dy * dy < rr * rr) {
                int idA = a->get_id(), idB = b->get_id();
                a->fusionarCon(b);
                archivo.guardarColision(tiemporeal, "PARTICULAS", idA, idB);
            }
        }
    }
}

void Simulador::registrarDatos() {
    for (auto *p : particula)
        if (p->get_activa())
            archivo.guardarPaso(tiemporeal, p->get_id(), p->get_ejex(), p->get_ejey());
}
