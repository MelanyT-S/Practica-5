#include "Obstaculo.h"

// Constructor del obstaculo cuadrado
Obstaculo::Obstaculo(double x, double y, double l, double elastico)
    : ejex(x), ejey(y), lado(l), Resvote(elastico) {}

// Comprueba si unas coordenadas caen dentro de los limites del cuadrado
bool Obstaculo::contienePunto(double puntox, double puntoy) const {
    return (puntox >= ejex && puntox <= ejex + lado &&
            puntoy >= ejey && puntoy <= ejey + lado);
}

// Evalua por cual de las 4 caras ocurre la colision para elrebote inelastico
int Obstaculo::LadoColision(double puntox, double puntoy, double r) const {
    // Retorna un codigo numerico segun el lado impactado:
    // 1 = Izquierda, 2 = Derecha, 3 = Abajo, 4 = Arriba
    if (puntox - r <= ejex) return 1;
    if (puntox + r >= ejex + lado) return 2;
    if (puntoy - r <= ejey) return 3;
    if (puntoy + r >= ejey + lado) return 4;
    return 0;
}

// Getters de la clase Obstaculo
double Obstaculo::get_ejex() const { return ejex; }
double Obstaculo::get_ejey() const { return ejey; }
double Obstaculo::get_Lado() const { return lado; }
double Obstaculo::get_elastico() const { return Resvote; }