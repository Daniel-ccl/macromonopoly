#ifndef SIMULACION_H
#define SIMULACION_H

#include <vector>
#include <string>
#include <random>
#include "raylib.h"
#include "tablero.h"
#include "matriz.h"

struct Jugador {
    Color color;
    int posicion;
    int propiedades;
    int casas;
    std::vector<int> propiedadesCompradas;
    int estado_actual = 0;

    void mover() {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        if (estado_actual < 0 || estado_actual >= (int)MATRIZ.size()) return; // seguridad

        const auto& fila = MATRIZ[estado_actual];
        std::discrete_distribution<> dist(fila.begin(), fila.end());
        estado_actual = dist(gen);
    }
};

void iniciarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2);
void actualizarSimulacion(float deltaTime);
void dibujarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2);
bool simulacionTerminada();
void dibujarResumen();

#endif

