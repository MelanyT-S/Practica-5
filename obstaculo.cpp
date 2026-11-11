#include "Obstaculo.h"
#include <cmath>

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
    // 1. Encuentra el punto mas cercano del cuadrado al centro de la particula (circulo)
    double cercanoX = puntox;
    double cercanoY = puntoy;

    if (puntox < ejex) {
        cercanoX = ejex;
    } else if (puntox > ejex + lado) {
        cercanoX = ejex + lado;
    }

    if (puntoy < ejey) {
        cercanoY = ejey;
    } else if (puntoy > ejey + lado) {
        cercanoY = ejey + lado;
    }
    // 2. Calcula la distancia al cuadrado entre el centro y ese punto cercano
    double distX = puntox - cercanoX;
    double distY = puntoy - cercanoY;
    double distanciaSq = distX * distX + distY * distY;

    // 3. Valida si la distancia es menor o igual al radio al cuadrado (colision real)
    if (distanciaSq <= r * r) {

        // 4. Utiliza std::fabs para determinar con exactitud por cual cara ocurrio el impacto[cite: 11]
        double dIzq = std::fabs(puntox - ejex);
        double dDer = std::fabs(puntox - (ejex + lado));
        double dInf = std::fabs(puntoy - ejey);
        double dSup = std::fabs(puntoy - (ejey + lado));

        double minVal = dIzq;
        int ladoCol = 1; // 1: Lado izquierdo

        if (dDer < minVal) {
            minVal = dDer;
            ladoCol = 2; // 2: Lado derecho
        }
        if (dInf < minVal) {
            minVal = dInf;
            ladoCol = 3; // 3: Lado inferior
        }
        if (dSup < minVal) {
            ladoCol = 4; // 4: Lado superior
        }

        return ladoCol;
    }

    // Retorna 0 si no hay contacto fisico real con el obstaculo
    return 0;
}

// Getters de la clase Obstaculo
double Obstaculo::get_ejex() const { return ejex; }
double Obstaculo::get_ejey() const { return ejey; }
double Obstaculo::get_Lado() const { return lado; }
double Obstaculo::get_elastico() const { return Resvote; }