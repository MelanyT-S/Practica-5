#include "Particula.h"

// Constructor de la clase Particula
Particula::Particula(int id, double x, double y, double vx, double vy, double masa, double r)
    : id(id), ejex(x), ejey(y), velocidadx(vx), velocidady(vy), masa(masa), radio(r), activa(true) {}

// Actualizacion cinematica en tiempo discreto (paso dt)
void Particula::mover(double dt, double g) {
    // Movimiento Rectilineo Uniforme (MRU) en el eje X: x = x0 + vx * dt
    ejex = ejex + velocidadx * dt;

    // Movimiento Rectilineo Uniforme Acelerado (MRUA / caida libre) en el eje Y: y = y0 + vy * dt - 0.5 * g * dt^2
    ejey = ejey + velocidady * dt - 0.5 * g * dt * dt;

    // Actualizacion de la velocidad vertical restando el efecto de la gravedad
    velocidady = velocidady - g * dt;
}

// Logica de rebote elastico con las paredes de la caja
void Particula::rebotarPared(double ancho, double alto) {
    // Colision con la pared izquierda o derecha (eje X)
    if (ejex - radio <= 0) {
        ejex = radio; // Correccion de posicion para evitar atascos
        velocidadx = -velocidadx; // Inversion de la velocidad horizontal
    } else if (ejex + radio >= ancho) {
        ejex = ancho - radio;
        velocidadx = -velocidadx;
    }

    // Colision con la pared inferior o superior (eje Y)
    if (ejey - radio <= 0) {
        ejey = radio;
        velocidady = -velocidady; // Inversion de la velocidad vertical
    } else if (ejey + radio >= alto) {
        ejey = alto - radio;
        velocidady = -velocidady;
    }
}

// Colision inelastica perfecta: fusion de dos particulas por conservacion del momento lineal
void Particula::fusionarCon(Particula* otra) {
    double nuevaMasa = this->masa + otra->getMasa();

    // Ley de conservacion del momento lineal: p_inicial = p_final => v_nueva = (m1*v1 + m2*v2) / (m1+m2)
    double nuevoVx = (this->masa * this->velocidadx + otra->getMasa() * otra->getVelocidadx()) / nuevaMasa;
    double nuevoVy = (this->masa * this->velocidady + otra->getMasa() * otra->getVelocidady()) / nuevaMasa;

    // Actualizacion de las propiedades del cuerpo resultante
    this->masa = nuevaMasa;
    this->velocidadx = nuevoVx;
    this->velocidady = nuevoVy;
    this->radio = this->radio + 2.0; // Incremento visual opcional del tamano

    // Se desactiva la otra particula para que deje de ser procesada individualmente
    otra->set_activa(false);
}

// Definicion de Getters
double Particula::get_ejex() const { return ejex; }
double Particula::get_ejey() const { return ejey; }
double Particula::getVelocidadx() const { return velocidadx; }
double Particula::getVelocidady() const { return velocidady; }
double Particula::getMasa() const { return masa; }
double Particula::getRadio() const { return radio; }
int Particula::get_id() const { return id; }
bool Particula::get_activa() const { return activa; }

// Definicion de Setters
void Particula::set_ejex(double x) { ejex = x; }
void Particula::set_ejey(double y) { ejey = y; }
void Particula::setVelocidadx(double vx) { velocidadx = vx; }
void Particula::setVelocidady(double vy) { velocidady = vy; }
void Particula::set_activa(bool a) { activa = a; }