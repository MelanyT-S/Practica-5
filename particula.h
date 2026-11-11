#ifndef PARTICULA_H
#define PARTICULA_H

class Particula {
private:
    int id;
    double ejex;
    double ejey;
    double velocidadx;
    double velocidady;
    double masa;
    double radio;
    bool activa;

public:
    // Constructor para inicializar la particula con sus propiedades fisicas
    Particula(int id, double x, double y, double vx, double vy, double masa, double r);

    // Metodo para actualizar la posicion y velocidad en tiempo discreto
    void mover(double dt, double g);

    // Metodo para manejar el rebote elastico con los limites de la caja
    void rebotarPared(double ancho, double alto);

    // Metodo para fusionar dos particulas usando la conservacion del momento lineal
    void fusionarCon(Particula* otra);

    // Getters para consultar el estado interno de la particula
    double get_ejex() const;
    double get_ejey() const;
    double getVelocidadx() const;
    double getVelocidady() const;
    double getMasa() const;
    double getRadio() const;
    int get_id() const;
    bool get_activa() const;

    // Setters para modificar propiedades si es necesario
    void set_ejex(double x);
    void set_ejey(double y);
    void setVelocidadx(double vx);
    void setVelocidady(double vy);
    void set_activa(bool a);
};

#endif // PARTICULA_H