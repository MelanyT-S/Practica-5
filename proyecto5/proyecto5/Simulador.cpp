#include "Simulador.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// Distancia del punto (x, y) al obstaculo (0 si está dentro).
static double distanciaAObstaculo(double x, double y, const Obstaculo *o) {
    double cx = min(max(x, o->get_ejex()), o->get_ejex() + o->get_Lado());
    double cy = min(max(y, o->get_ejey()), o->get_ejey() + o->get_Lado());
    return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
}
//cosntructor y destructor
Simulador::Simulador(double ancho, double alto, double dt, double g)
    : ancho(ancho), alto(alto), tiempo(dt), tiemporeal(0), gravedad(g), archivo("salida.txt") {
    if (dt <= 0) throw invalid_argument("dt debe ser positivo");
    if (ancho <= 0 || alto <= 0) throw invalid_argument("dimensiones invalidas");
    if (!archivo.abrir()) throw runtime_error("No se pudo abrir el archivo de salida");
}
Simulador::~Simulador() {
    for (Particula *p : particula) delete p;
    for (Obstaculo *o : obstaculo) delete o;
    archivo.cerrar();
}
// Validaciones
void Simulador::validarParticula(const Particula *p) const {
    double x = p->get_ejex(), y = p->get_ejey(), r = p->getRadio();
    if (p->getMasa() <= 0 || r <= 0)
        throw invalid_argument("la masa y el radio deben ser positivos");
    if (x - r < 0 || x + r > ancho || y - r < 0 || y + r > alto)
        throw invalid_argument("la particula se sale de la caja");
    for (Particula *q : particula)
        if (q->get_id() == p->get_id())
            throw invalid_argument("ya existe una particula con ese id");
    for (size_t k = 0; k < obstaculo.size(); k++)
        if (distanciaAObstaculo(x, y, obstaculo[k]) < r)
            throw invalid_argument(string("la particula toca el obstaculo ") + to_string(k + 1));
    for (Particula *q : particula) {
        if (!q->get_activa()) continue;
        double d = hypot(x - q->get_ejex(), y - q->get_ejey());
        if (d < r + q->getRadio())
            throw invalid_argument(string("la particula se traslapa con la particula ") + to_string(q->get_id()));
    }
}
void Simulador::validarObstaculo(const Obstaculo *o) const {
    double x = o->get_ejex(), y = o->get_ejey(), l = o->get_Lado();
    if (l <= 0) throw invalid_argument("el lado debe ser positivo");
    if (o->get_elastico() < 0 || o->get_elastico() > 1)
        throw invalid_argument("el coeficiente de restitucion debe estar entre 0 y 1");
    if (x < 0 || y < 0 || x + l > ancho || y + l > alto)
        throw invalid_argument("el obstaculo se sale de la caja");
    for (size_t k = 0; k < obstaculo.size(); k++) {
        const Obstaculo *q = obstaculo[k];
        bool separados = x + l <= q->get_ejex() || q->get_ejex() + q->get_Lado() <= x ||
                         y + l <= q->get_ejey() || q->get_ejey() + q->get_Lado() <= y;
        if (!separados)
            throw invalid_argument(string("el obstaculo se traslapa con el obstaculo ") + to_string(k + 1));
    }
    for (Particula *p : particula)
        if (p->get_activa() && distanciaAObstaculo(p->get_ejex(), p->get_ejey(), o) < p->getRadio())
            throw invalid_argument(string("el obstaculo toca la particula ") + to_string(p->get_id()));
}
//Agregar y eliminar
void Simulador::agregarParticula(Particula *p) {
    if (!p) throw invalid_argument("particula nula");
    try {
        validarParticula(p);
    } catch (...) {
        delete p;
        throw;
    }
    particula.push_back(p);
}
void Simulador::agregarObstaculo(Obstaculo *o) {
    if (!o) throw invalid_argument("obstaculo nulo");
    try {
        validarObstaculo(o);
    } catch (...) {
        delete o;
        throw;
    }
    obstaculo.push_back(o);
}
bool Simulador::eliminarParticula(int id) {
    for (size_t i = 0; i < particula.size(); i++) {
        if (particula[i]->get_id() == id) {
            delete particula[i];
            particula.erase(particula.begin() + i);
            return true;
        }
    }
    return false;
}
bool Simulador::eliminarObstaculo(int indice) {
    if (indice < 1 || indice > (int)obstaculo.size()) return false;
    delete obstaculo[indice - 1];
    obstaculo.erase(obstaculo.begin() + (indice - 1));
    return true;
}
int Simulador::cantidadParticulas() const {
    int n = 0;
    for (Particula *p : particula)
        if (p->get_activa()) n++;
    return n;
}
int Simulador::cantidadObstaculos() const { return (int)obstaculo.size(); }
double Simulador::getTiempoReal() const { return tiemporeal; }
void Simulador::reiniciar() {
    for (Particula *p : particula) delete p;
    for (Obstaculo *o : obstaculo) delete o;
    particula.clear();
    obstaculo.clear();
    tiemporeal = 0;
    archivo.cerrar();
    if (!archivo.abrir()) throw runtime_error("No se pudo abrir el archivo de salida");
}
// Imprimir
void Simulador::imprimirSistema() const {
    cout << "Caja: " << ancho << " x " << alto
         << "   dt: " << tiempo << "   gravedad: " << gravedad
         << "   tiempo simulado: " << tiemporeal << " s" << endl;
    cout << "Obstaculos (esquina inferior izquierda, lado, restitucion):" << endl;
    for (size_t k = 0; k < obstaculo.size(); k++)
        cout << "  " << k + 1 << ". (" << obstaculo[k]->get_ejex() << ", " << obstaculo[k]->get_ejey()
             << ")  lado " << obstaculo[k]->get_Lado()
             << "  e = " << obstaculo[k]->get_elastico() << endl;
    cout << "Particulas (posicion, velocidad, masa, radio):" << endl;
    int absorbidas = 0;
    for (Particula *p : particula) {
        if (!p->get_activa()) { absorbidas++; continue; }
        cout << "  id " << p->get_id() << ": (" << p->get_ejex() << ", " << p->get_ejey()
             << ")  v = (" << p->getVelocidadx() << ", " << p->getVelocidady()
             << ")  m = " << p->getMasa() << "  r = " << p->getRadio() << endl;
    }
    if (absorbidas > 0) cout << "  (" << absorbidas << " particula(s) absorbida(s) por fusion)" << endl;
}
// Dibujo en texto: '#'=obstaculo, 1-9 y A-Z=particula(su id),(0,0)
void Simulador::imprimirGrafico() const {
    const int columnas = 70;//matriz
    int filas = (int)(alto / ancho * columnas * 0.5);//acomodar a altura acci
    filas = max(5, min(40, filas));
    vector<string> mapa(filas, string(columnas, ' '));
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            double x = (c + 0.5) * ancho / columnas;
            double y = alto - (f + 0.5) * alto / filas;//invierte
            for (Obstaculo *o : obstaculo)
                if (o->contienePunto(x, y)) mapa[f][c] = '#';
        }
    }
    for (Particula *p : particula) {
        if (!p->get_activa()) continue;
        int c = (int)(p->get_ejex() / ancho * columnas);
        int f = (int)((alto - p->get_ejey()) / alto * filas);
        c = max(0, min(columnas - 1, c));
        f = max(0, min(filas - 1, f));
        int id = p->get_id();
        char simbolo = (id < 10) ? char('0' + id) : (id < 36 ? char('A' + id - 10) : '*');
        mapa[f][c] = simbolo;
    }
    cout << "+" << string(columnas, '-') << "+" << endl;
    for (int f = 0; f < filas; f++) cout << "|" << mapa[f] << "|" << endl;
    cout << "+" << string(columnas, '-') << "+" << endl;
    cout << "# obstaculo   1-9, A-Z = id de la particula   (t = " << tiemporeal << " s)" << endl;
}
// BUCLE
void Simulador::iniciarSimulacion(double tiempoTotal) {
    if (tiemporeal == 0) registrarDatos();    // estado inicial (t = 0)
    while (tiemporeal < tiempoTotal) {
        actualizarEstado();                   // 1. mover
        verificarColisiones();                // 2. colisiones
        tiemporeal += tiempo;
        registrarDatos();                     // 3. guardar
    }
}
void Simulador::actualizarEstado() {
    for (Particula *p : particula)
        if (p->get_activa()) p->mover(tiempo, gravedad);
}
// Choque con un obstáculo
// Aquí se determina si hay contacto y con cuál lado del cuadrado, usando solo
// los getters de Obstaculo (get_ejex, get_ejey, get_Lado, get_elastico) y contienePunto.
bool Simulador::colisionarObstaculo(Particula *p, Obstaculo *o) {
    double ox = o->get_ejex(), oy = o->get_ejey(), l = o->get_Lado();
    double px = p->get_ejex(), py = p->get_ejey(), r = p->getRadio();
    // Punto del cuadrado más cercano al centro de la partícula
    double cx = min(max(px, ox), ox + l);
    double cy = min(max(py, oy), oy + l);
    double dx = px - cx, dy = py - cy;
    if (dx * dx + dy * dy >= r * r) return false;
    double nx = 0, ny = 0;
    if (!o->contienePunto(px, py)) {
        if (fabs(dx) >= fabs(dy)) nx = (dx < 0) ? -1 : 1;   // izquierda / derecha
        else                      ny = (dy < 0) ? -1 : 1;   // abajo / arriba
    } else {
        double izq = px - ox, der = ox + l - px;
        double aba = py - oy, arr = oy + l - py;
        double m = min(min(izq, der), min(aba, arr));
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
    // choque elástico (paredes) y choque inelástico(obstáculos)
    for (Particula *p : particula) {
        if (!p->get_activa()) continue;
        // Paredes: rebotarPared no avisa,se compara la velocidad antes y después
        double vx = p->getVelocidadx(), vy = p->getVelocidady();
        p->rebotarPared(ancho, alto);
        if (p->getVelocidadx() != vx || p->getVelocidady() != vy)
            archivo.guardarcolicion(tiemporeal, "PARED", p->get_id(), -1);
        // Obstáculos
        for (size_t k = 0; k < obstaculo.size(); k++)
            if (colisionarObstaculo(p, obstaculo[k]))
                archivo.guardarcolicion(tiemporeal, "OBSTACULO", p->get_id(), (int)k);
    }
    //Partícula contra partícula completamente inelástica
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
                archivo.guardarcolicion(tiemporeal, "PARTICULAS", idA, idB);
            }
        }
    }
}
void Simulador::registrarDatos() {
    vector<Particula *> activas;
    for (Particula *p : particula)
        if (p->get_activa()) activas.push_back(p);
    archivo.guardarpaso(tiemporeal, activas);
}