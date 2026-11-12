#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Simulador.h"
using namespace std;

// Lee un numero (acepta coma o punto) y repite hasta que sea valido
double leerNumero(const string &texto)
{
    string linea;
    while (true) {
        cout << texto;
        if (!(cin >> linea)) throw runtime_error("entrada cerrada");
        replace(linea.begin(), linea.end(), ',', '.');
        try {
            size_t pos;
            double v = stod(linea, &pos);
            if (pos == linea.size()) return v;
        } catch (const exception &) {}
        cout << "valor no valido, escribe un numero" << endl;
    }
}

// Caso por defecto
void cargarCasoPorDefecto(Simulador &sim, int &siguienteId)
{
    sim.reiniciar();

    // Obstaculo(x, y, lado, restitucion)
    sim.agregarObstaculo(new Obstaculo(25, 10, 10, 0.6));
    sim.agregarObstaculo(new Obstaculo(65, 10, 10, 0.6));
    sim.agregarObstaculo(new Obstaculo(25, 40, 10, 0.6));
    sim.agregarObstaculo(new Obstaculo(65, 40, 10, 0.6));

    // Particula(id, x, y, vx, vy, masa, radio)
    sim.agregarParticula(new Particula(1, 10, 30, 14, 3, 2.0, 2.0));
    sim.agregarParticula(new Particula(2, 90, 30, -12, -2, 1.0, 1.5));
    sim.agregarParticula(new Particula(3, 50, 10, 2, 9, 1.5, 1.8));
    sim.agregarParticula(new Particula(4, 50, 50, -3, -8, 1.0, 1.5));
    sim.agregarParticula(new Particula(5, 15, 55, 8, -5, 0.5, 1.2));

    siguienteId = 6;
}

int main()
{
    try {
        Simulador sim(100, 60, 0.01);
        int siguienteId;
        cargarCasoPorDefecto(sim, siguienteId);

        bool salir = false;
        char opcion;
        double x, y, vx, vy, masa, radio, lado, e, tiempo;
        int id, indice;
        cout << "Simulacion de colisiones (caso por defecto cargado)" << endl;
        while (!salir) {
            cout << endl << "que quieres hacer?" << endl;
            cout << " 1. ver el sistema,2. ver el grafico,3. agregar particula,4. eliminar particula" << endl;
            cout << " 5. agregar obstaculo,6. eliminar obstaculo, 7. ejecutar simulacion,8. restaurar caso por defecto" << endl;
            cout << " 0. salir" << endl;
            if (!(cin >> opcion)) break;
            try {
                switch (opcion) {
                case '1':
                    sim.imprimirSistema();
                    break;
                case '2':
                    sim.imprimirGrafico();
                    break;
                case '3':
                    cout << "nueva particula con id " << siguienteId << endl;
                    x = leerNumero("posicion x: ");
                    y = leerNumero("posicion y: ");
                    vx = leerNumero("velocidad vx: ");
                    vy = leerNumero("velocidad vy: ");
                    masa = leerNumero("masa: ");
                    radio = leerNumero("radio: ");
                    sim.agregarParticula(new Particula(siguienteId, x, y, vx, vy, masa, radio));
                    cout << "particula " << siguienteId << " agregada" << endl;
                    siguienteId++;
                    break;
                case '4':
                    id = (int)leerNumero("id de la particula a eliminar: ");
                    if (sim.eliminarParticula(id)) cout << "particula eliminada" << endl;
                    else cout << "no existe esa particula" << endl;
                    break;
                case '5':
                    x = leerNumero("x de la esquina inferior izquierda: ");
                    y = leerNumero("y de la esquina inferior izquierda: ");
                    lado = leerNumero("lado: ");
                    e = leerNumero("coeficiente de restitucion (0 a 1): ");
                    sim.agregarObstaculo(new Obstaculo(x, y, lado, e));
                    cout << "obstaculo agregado" << endl;
                    break;
                case '6':
                    indice = (int)leerNumero("numero del obstaculo a eliminar: ");
                    if (sim.eliminarObstaculo(indice)) cout << "obstaculo eliminado" << endl;
                    else cout << "no existe ese obstaculo" << endl;
                    break;
                case '7':
                    if (sim.cantidadParticulas() == 0) {
                        cout << "no hay particulas para simular" << endl;
                        break;
                    }
                    if (sim.cantidadParticulas() < 4 || sim.cantidadObstaculos() != 4)
                        cout << "aviso: el enunciado pide al menos 4 particulas y 4 obstaculos" << endl;
                    tiempo = leerNumero("simular hasta t = (s): ");
                    if (tiempo <= sim.getTiempoReal()) {
                        cout << "ya se simulo hasta t = " << sim.getTiempoReal()
                             << " s, usa un tiempo mayor o restaura el caso (8)" << endl;
                        break;
                    }
                    sim.iniciarSimulacion(tiempo);
                    cout << "simulacion terminada, resultados en salida.txt" << endl;
                    break;
                case '8':
                    cargarCasoPorDefecto(sim, siguienteId);
                    cout << "caso por defecto restaurado" << endl;
                    break;
                case '0':
                    salir = true;
                    break;
                default:
                    cout << "opcion no valida" << endl;
                }
            } catch (const exception &ex) {
                cout << "error: " << ex.what() << endl;
            }
        }
    } catch (const exception &ex) {
        cerr << "error: " << ex.what() << endl;
        return 1;
    }
    return 0;
}
