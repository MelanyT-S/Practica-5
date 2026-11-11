#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo {
private:
    double ejex;
    double ejey;
    double lado;
    double Resvote; // Coeficiente de restitucion (e) para el calculo de perdida de energia

public:
    // Constructor para definir la posicion inicial (esquina) y el tamano del cuadrado
    Obstaculo(double x, double y, double l, double elastico);

    // Funcion para verificar si un punto se encuentra dentro del area del obstaculo
    bool contienePunto(double puntox, double puntoy) const;

    // Funcion para identificar exactamente cual cara del cuadrado recibio el impacto
    int LadoColision(double puntox, double puntoy, double r) const;

    // Getters
    double get_ejex() const;
    double get_ejey() const;
    double get_Lado() const;
    double get_elastico() const;
};

#endif // OBSTACULO_H